#include "Wire.h"
#include "CanvasNode.h"
#include <QPen>
#include <QPainterPath>

Wire::Wire(CanvasNode* from, CanvasNode* to)
    : fromNode(from), toNode(to)
{
    setZValue(-1); 
    setPen(QPen(Qt::yellow, 2));
    setFlag(QGraphicsItem::ItemIsSelectable);
    updatePosition();
}

void Wire::updatePosition()
{
    if (!fromNode || !toNode) return;

    QPointF p1 = fromNode->scenePos();
    QPointF p2 = toNode->scenePos();

    setLine(QLineF(p1, p2));
}
QPainterPath Wire::shape() const
{
    QPainterPath path;
    path.moveTo(line().p1());
    path.lineTo(line().p2());

    QPainterPathStroker s;
    s.setWidth(20);   // ← clickable thickness
    return s.createStroke(path);
}
