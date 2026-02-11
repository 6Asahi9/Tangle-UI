#ifndef NODE_H
#define NODE_H
#pragma once
#include <QString>

class Node
{
public:
    QString text;
    int id;
    Node* next;
    QPointF position;   
    QVector<int> leftHooks; 
    QVector<int> rightHooks;

    Node(const QString &t, int identifier, const QPointF& pos = QPointF()): text(t),
     id(identifier), next(nullptr), position(pos){}
};

#endif // NODE_H
