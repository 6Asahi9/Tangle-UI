#ifndef WIRE_H
#define WIRE_H
#include <QPainterPath>
#include <QGraphicsLineItem>

class CanvasNode;

class Wire : public QGraphicsLineItem
{
public:
    Wire(CanvasNode* from, CanvasNode* to);

    CanvasNode* fromNode;
    CanvasNode* toNode;

    void updatePosition();
    QPainterPath shape() const override;
};

#endif
