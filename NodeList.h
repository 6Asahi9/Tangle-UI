#ifndef NODELIST_H
#define NODELIST_H
#pragma once
#include "Node.h"

class NodeList
{
public:
    Node* head;

    NodeList() : head(nullptr) {}

    void addNode(Node* newNode) {
        if (!head) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while(temp->next)
            temp = temp->next;
        temp->next = newNode;
    }

    void insertAfter(Node* prevNode, Node* newNode) {
        if (!prevNode) return;
        newNode->next = prevNode->next;
        prevNode->next = newNode;
    }
};

#endif // NODELIST_H
