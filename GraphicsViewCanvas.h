#ifndef GRAPHICSVIEWCANVAS_H
#define GRAPHICSVIEWCANVAS_H

#pragma once
#include "CanvasNode.h"
#include <QGraphicsView>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QResizeEvent>
#include "NodeList.h"

class CanvasNode; 
class GraphicsViewCanvas : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsViewCanvas(QWidget *parent = nullptr);
    void zoom(double factor, const QPointF& center);
    CanvasNode* pendingConnection = nullptr;
    NodeList nodes; 
    void writeModelFile();
    void saveNodePtr();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void connectNodes(CanvasNode* A, CanvasNode* B,CanvasNode::HookType fromHook,CanvasNode::HookType toHook);
    void drawForeground(QPainter*, const QRectF&) override;
    bool validateGraph();
    
    CanvasNode::HookType pendingHook = CanvasNode::None;
    QPointF mousePos;       
    void mouseMoveEvent(QMouseEvent* event);  
    void mouseReleaseEvent(QMouseEvent* event);                   


private:
    QString extractTreeWidgetText(const QMimeData* mimeData);
};

#endif // GRAPHICSVIEWCANVAS_H
