#include "Wire.h"
#include "CanvasNode.h"

Wire::Wire(CanvasNode* from, CanvasNode* to)
    : fromNode(from), toNode(to)
{
    setZValue(-1); 
    updatePosition();
}

void Wire::updatePosition()
{
    if (!fromNode || !toNode) return;

    QPointF p1 = fromNode->scenePos();
    QPointF p2 = toNode->scenePos();

    setLine(QLineF(p1, p2));
}
