


// ====================================================================
//
//  A network optimization algorithm using Fibonacci Heap
//
//  Written by: Max Winkler
//
//  Modified by: Zheng Rui
// ====================================================================

#ifndef _FIBONACCI_HEAP_H_
#define _FIBONACCI_HEAP_H_

#include <stdio.h>
#include "newgraphicsview.h"

struct pixelNode;

class Node
{
private:
public:
    Node * parent;
    Node * leftSibling, * rightSibling;
    Node * children; 
    Node * pred;
    pixelNode * pixel;

    Node(pixelNode* pixel, double key);
    Node();

    double key;
    int rank;


    bool addChild(Node * node);
    bool addSibling(Node * node);
    bool remove();
    
    Node* leftMostSibling();
    Node* rightMostSibling();

};

class FibonacciHeap
{
private:
    Node* rootListByRank[100];

    bool link(Node* root);  
    Node * minRoot;

public:

    FibonacciHeap();
    FibonacciHeap(Node *root);

    ~FibonacciHeap();

    bool isEmpty();
    bool insertVertex(Node * node); 
    void decreaseKey(double delta, Node* vertex);

    Node* findMin();
    Node* deleteMin();
};

#endif
