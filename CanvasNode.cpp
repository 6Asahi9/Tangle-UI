#include "CanvasNode.h"
#include <QPainter>
#include "Wire.h"
#include <QGraphicsScene>

CanvasNode::CanvasNode(const QString& text)
    : m_text(text)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
}

QRectF CanvasNode::boundingRect() const
{
    return QRectF(-m_hookRadius, 0, m_width + 2*m_hookRadius, m_height);
}

QPainterPath CanvasNode::shape() const
{
    QPainterPath path;
    qreal margin = 10; // smaller hitbox so hooks are easier to grab
    path.addRect(margin, margin, m_width - 2*margin, m_height - 2*margin);
    return path;
}

void CanvasNode::paint(QPainter* painter,
                       const QStyleOptionGraphicsItem*,
                       QWidget*)
{
    // Node body
    painter->setBrush(QColor(60, 60, 60));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawRoundedRect(0, 0, m_width, m_height, 6, 6);

    painter->setPen(Qt::white);
    painter->drawText(0, 0, m_width, m_height, Qt::AlignCenter, m_text);

    // Hooks
    QPointF leftHook = hookPosition(Left);
    painter->setBrush(Qt::green);
    painter->drawEllipse(leftHook, m_hookRadius, m_hookRadius);

    QPointF rightHook = hookPosition(Right);
    painter->setBrush(Qt::red);
    painter->drawEllipse(rightHook, m_hookRadius, m_hookRadius);
}

QVariant CanvasNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene())
    {
        if (leftWire) leftWire->updatePosition();
        if (rightWire) rightWire->updatePosition();

        scene()->update();
    }

    return QGraphicsItem::itemChange(change, value);
}

CanvasNode::HookType CanvasNode::hookAt(const QPointF& pos) const
{
    const qreal hitboxRadius = m_hookRadius * 3.0;

    QRectF leftBox(hookPosition(Left) - QPointF(hitboxRadius, hitboxRadius),
                   QSizeF(hitboxRadius*2, hitboxRadius*2));
    QRectF rightBox(hookPosition(Right) - QPointF(hitboxRadius, hitboxRadius),
                    QSizeF(hitboxRadius*2, hitboxRadius*2));

    if (leftBox.contains(pos))
        return Left;
    if (rightBox.contains(pos))
        return Right;

    return None;
}

