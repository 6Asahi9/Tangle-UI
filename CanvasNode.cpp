#include "CanvasNode.h"
#include <QPainter>
#include "Wire.h"
#include <QGraphicsScene>
#include <QFileInfo>
static constexpr qreal HOOK_OFFSET = 12.0;   
static constexpr qreal HOOK_HITBOX = 15.0;   

CanvasNode::CanvasNode(const QString& text)
    : m_text(text)
{
    label = QFileInfo(text).baseName();
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
}

QRectF CanvasNode::boundingRect() const
{
    return QRectF(-HOOK_OFFSET - HOOK_HITBOX,
                  -HOOK_HITBOX,
                  m_width + 2*(HOOK_OFFSET + HOOK_HITBOX),
                  m_height + 2*HOOK_HITBOX);
}

QPainterPath CanvasNode::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, m_width, m_height);
    path.addEllipse(hookPosition(Left), HOOK_HITBOX, HOOK_HITBOX);
    path.addEllipse(hookPosition(Right), HOOK_HITBOX, HOOK_HITBOX);
    return path;
}


QPointF CanvasNode::hookPosition(HookType hook) const
{
    if (hook == Left)
        return QPointF(-HOOK_OFFSET, m_height / 2.0);
    else // Right
        return QPointF(m_width + HOOK_OFFSET, m_height / 2.0);
}

void CanvasNode::paint(QPainter* painter,
                       const QStyleOptionGraphicsItem*,
                       QWidget*)
{
    // node body
    painter->setBrush(QColor(60, 60, 60));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawRoundedRect(0, 0, m_width, m_height, 6, 6);

    painter->setPen(Qt::white);
    painter->drawText(0, 0, m_width, m_height, Qt::AlignCenter, label);

    // hooks 
    QPointF leftHook = hookPosition(Left);
    painter->setBrush(Qt::green);
    painter->drawEllipse(leftHook, m_hookRadius, m_hookRadius);

    QPointF rightHook = hookPosition(Right);
    painter->setBrush(Qt::red);
    painter->drawEllipse(rightHook, m_hookRadius, m_hookRadius);
}

QString CanvasNode::getPath() const { return m_text; }

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
    const qreal r = HOOK_HITBOX;

    QRectF leftBox(hookPosition(Left) - QPointF(r, r),
                   QSizeF(r*2, r*2));

    QRectF rightBox(hookPosition(Right) - QPointF(r, r),
                    QSizeF(r*2, r*2));

    if (leftBox.contains(pos))
        return Left;
    if (rightBox.contains(pos))
        return Right;

    return None;
}

CanvasNode::~CanvasNode()
{
    if (leftConnection)
        leftConnection->rightConnection = nullptr;

    if (rightConnection)
        rightConnection->leftConnection = nullptr;
}
