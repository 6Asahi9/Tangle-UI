#ifndef CANVASNODE_H
#define CANVASNODE_H

#include <QGraphicsItem>
#include <QString>

class Wire;

class CanvasNode : public QGraphicsItem
{
public:
    CanvasNode(const QString& text);

    Wire* leftWire = nullptr;
    Wire* rightWire = nullptr;

    int nodeId = -1;

    CanvasNode* leftConnection = nullptr;
    CanvasNode* rightConnection = nullptr;

    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override; 
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

    QString text() const { return m_text; }
    QString getText() const { return m_text; }

    enum HookType { None, Left, Right };
    HookType hookAt(const QPointF& pos) const;

    QPointF hookPosition(HookType hook) const {
        if (hook == Left)
            return QPointF(0, m_height / 2);
        else if (hook == Right)
            return QPointF(m_width, m_height / 2);
        return QPointF();
    }

private:
    QString m_text;

    const qreal m_width  = 120;
    const qreal m_height = 60;

    const qreal m_hookRadius = 6; 
};

#endif // CANVASNODE_H
