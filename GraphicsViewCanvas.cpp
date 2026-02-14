#include "GraphicsViewCanvas.h"
#include <QMimeData>
#include <QGraphicsScene>
#include <QDataStream>
#include "CanvasNode.h"
#include <QScrollBar>
#include <QMouseEvent>
#include <QFile>
#include <QTransform>
#include <QKeyEvent>
// GraphicsViewCanvas::GraphicsViewCanvas(QWidget *parent)
//     : QGraphicsView(parent)
// {
//     setAcceptDrops(true);
//     viewport()->setAcceptDrops(true);

//     QGraphicsScene* sc = new QGraphicsScene(this);
//     setScene(sc);
//     sc->setSceneRect(viewport()->rect());

//     setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//     setDragMode(QGraphicsView::RubberBandDrag);

//     setTransformationAnchor(QGraphicsView::AnchorViewCenter);
//     setResizeAnchor(QGraphicsView::AnchorViewCenter);

// }

GraphicsViewCanvas::GraphicsViewCanvas(QWidget *parent)
    : QGraphicsView(parent),
      pendingConnection(nullptr)
{
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);

    setScene(new QGraphicsScene(this));
    scene()->setSceneRect(-5000, -5000, 10000, 10000);

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::RubberBandDrag);

    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
}

void GraphicsViewCanvas::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText() ||
        event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
        event->accept();
    else
        event->ignore();
}

void GraphicsViewCanvas::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasText() ||
        event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
        event->accept();
    else
        event->ignore();
}

void GraphicsViewCanvas::dropEvent(QDropEvent *event)
{
    QString text = extractTreeWidgetText(event->mimeData());
    if (text.isEmpty()) {
        event->ignore();
        return;
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QPointF scenePos = mapToScene(event->position().toPoint());
#else
    QPointF scenePos = mapToScene(event->pos());
#endif

    CanvasNode* node = new CanvasNode(text);
    static int nextId = 1;
    node->nodeId = nextId++;

    node->setPos(scenePos);
    scene()->addItem(node);
    event->accept();
}

void GraphicsViewCanvas::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    // if (scene())
    //     scene()->setSceneRect(viewport()->rect());
}

void GraphicsViewCanvas::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        const double zoomFactor = 1.15;

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        QPointF scenePos = mapToScene(event->position().toPoint());
#else
        QPointF scenePos = mapToScene(event->pos());
#endif
        if (event->angleDelta().y() > 0)
            scale(zoomFactor, zoomFactor);
        else
            scale(1.0 / zoomFactor, 1.0 / zoomFactor);

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        QPointF newScenePos = mapToScene(event->position().toPoint());
#else
        QPointF newScenePos = mapToScene(event->pos());
#endif
        QPointF delta = newScenePos - scenePos;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        event->accept();
        return;
    }

    QGraphicsView::wheelEvent(event);
}

void GraphicsViewCanvas::zoom(double factor, const QPointF& center)
{
    QPointF oldPos = mapToScene(center.toPoint());

    scale(factor, factor);

    QPointF newPos = mapToScene(center.toPoint());

    QPointF delta = newPos - oldPos;
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + delta.x());
    verticalScrollBar()->setValue(verticalScrollBar()->value() + delta.y());
}

void GraphicsViewCanvas::mousePressEvent(QMouseEvent* event)
{
    QGraphicsItem* item = itemAt(event->pos());
    CanvasNode* node = dynamic_cast<CanvasNode*>(item);
    if (node)
    {
        QPointF posInNode = node->mapFromScene(mapToScene(event->pos()));
        CanvasNode::HookType hook = node->hookAt(posInNode);

        if (hook != CanvasNode::None)
        {
            pendingConnection = node;
            pendingHook = hook;
            node->setFlag(QGraphicsItem::ItemIsMovable, false);
            return;
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsViewCanvas::mouseMoveEvent(QMouseEvent* event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    mousePos = mapToScene(event->position().toPoint());
#else
    mousePos = mapToScene(event->pos());
#endif

    if (pendingConnection)
        scene()->update();

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsViewCanvas::mouseReleaseEvent(QMouseEvent* event)
{
    if (pendingConnection)
    {
        QGraphicsItem* item = itemAt(event->pos());
        CanvasNode* target = dynamic_cast<CanvasNode*>(item);

        if (target && target != pendingConnection)
        {
            QPointF posInTarget = target->mapFromScene(mapToScene(event->pos()));
            CanvasNode::HookType targetHook = target->hookAt(posInTarget);

            connectNodes(pendingConnection, target,
                         pendingHook, targetHook);
        }

        pendingConnection->setFlag(QGraphicsItem::ItemIsMovable, true);
        pendingConnection = nullptr;
        pendingHook = CanvasNode::None;
        scene()->update();
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsViewCanvas::connectNodes(CanvasNode* A, CanvasNode* B,
                                      CanvasNode::HookType fromHook,
                                      CanvasNode::HookType toHook)
{
    if (!A || !B || A == B) return;

    // Only Right -> Left allowed
    if (fromHook != CanvasNode::Right ||
        toHook   != CanvasNode::Left)
        return;

    if (A->rightConnection || B->leftConnection)
        return;
    if (B->rightConnection == A)
        return;
    A->rightConnection = B;
    B->leftConnection = A;

    scene()->update();
}

void GraphicsViewCanvas::drawForeground(QPainter *painter, const QRectF &)
{
    painter->setRenderHint(QPainter::Antialiasing);

    for (QGraphicsItem* item : scene()->items())
    {
        CanvasNode* node = dynamic_cast<CanvasNode*>(item);
        if (!node || !node->rightConnection || 
            !scene()->items().contains(node->rightConnection))
            continue;

        QPointF start = node->scenePos() + node->hookPosition(CanvasNode::Right);
        QPointF end   = node->rightConnection->scenePos() + node->rightConnection->hookPosition(CanvasNode::Left);

        QPointF delta = end - start;
        QPointF ctrl1 = start + QPointF(delta.x() * 0.3, 0);
        QPointF ctrl2 = end   - QPointF(delta.x() * 0.3, 0);

        QPainterPath path(start);
        path.cubicTo(ctrl1, ctrl2, end);

        painter->setPen(QPen(Qt::yellow, 2));
        painter->drawPath(path);
    }

    if (pendingConnection)
    {
        QPointF start = pendingConnection->scenePos()
                      + pendingConnection->hookPosition(pendingHook);

        QPointF end = mousePos;

        QPointF delta = end - start;
        QPointF ctrl1 = start + QPointF(delta.x() * 0.3, 0);
        QPointF ctrl2 = end - QPointF(delta.x() * 0.3, 0);

        QPainterPath path(start);
        path.cubicTo(ctrl1, ctrl2, end);

        painter->setPen(QPen(Qt::yellow, 2, Qt::DashLine));
        painter->drawPath(path);
    }
}

bool GraphicsViewCanvas::validateGraph()
{
    bool ok = true;

    for (QGraphicsItem* item : scene()->items())
    {
        CanvasNode* node = dynamic_cast<CanvasNode*>(item);
        if (!node) continue;

        if (!node->leftConnection && !node->rightConnection)
            ok = false;
    }

    return ok;
}

void GraphicsViewCanvas::writeModelFile()
{
    QFile outFile("model.py");
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&outFile);

    // find start nodes (no parent)
    QVector<CanvasNode*> starts;

    for (QGraphicsItem* item : scene()->items())
    {
        CanvasNode* node = dynamic_cast<CanvasNode*>(item);
        if (node && !node->leftConnection)
            starts.push_back(node);
    }

    // optional: top-to-bottom order
    std::sort(starts.begin(), starts.end(),
              [](CanvasNode* a, CanvasNode* b)
              { return a->scenePos().y() < b->scenePos().y(); });

    // traverse chains
    for (CanvasNode* start : starts)
    {
        CanvasNode* cur = start;

        while (cur)
        {
            QString path = "toolbox/" + cur->getText() + ".txt";

            QFile inFile(path);
            if (inFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&inFile);
                out << in.readAll() << "\n";
                inFile.close();
            }
            else
            {
                out << "# missing file: " << path << "\n";
            }

            cur = cur->rightConnection;
        }
    }

    outFile.close();
}

void GraphicsViewCanvas::saveNodePtr()
{
    QFile file("node.ptr");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&file);

    for (QGraphicsItem* item : scene()->items())
    {
        CanvasNode* node = dynamic_cast<CanvasNode*>(item);
        if (!node) continue;

        out << node->nodeId << " "
            << node->scenePos().x() << " "
            << node->scenePos().y() << " ";

        out << (node->leftConnection ?
                node->leftConnection->nodeId : -1) << " ";

        out << (node->rightConnection ?
                node->rightConnection->nodeId : -1) << "\n";
    }

    file.close();
}

QString GraphicsViewCanvas::extractTreeWidgetText(const QMimeData* mimeData)
{
    if (mimeData->hasText())
        return mimeData->text();

    if (!mimeData->hasFormat("application/x-qabstractitemmodeldatalist"))
        return QString();

    QByteArray encoded = mimeData->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int, QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        if (roleDataMap.contains(Qt::DisplayRole))
            return roleDataMap[Qt::DisplayRole].toString();
    }

    return QString();
}

void GraphicsViewCanvas::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return &&
        event->modifiers() & Qt::ControlModifier)
    {
        writeModelFile();
        return;
    }

    QGraphicsView::keyPressEvent(event);
}
