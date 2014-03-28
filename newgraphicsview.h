#ifndef NEWGRAPHICSVIEW_H
#define NEWGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "FibonacciHeap.h"
#include <vector>

class Node;

struct pixelNode {
    int linkCost[3][3];
    int state;
    double totalCost;
    pixelNode* prevNode;
    int column, row;
    Node* fibNode;
};

struct pathCords {
    int* p;
    int pLen;
    QGraphicsItem* whichQtItem;
};


class NewGraphicsView : public QGraphicsView
{
public:
    NewGraphicsView(QWidget *parent = NULL);
    void openAct();
    void showPixelNode();
    void showCostGraph();
    void showPathTree();
    bool showMinPath();
    void switchAnimation();
    void savePicture();

protected:
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    bool _pan, _hasimage, _animation, _minpath, _ignore_last_seed;
    int _panStartX, _panStartY, maxD;
    QGraphicsScene* Scene;
    QGraphicsPathItem* lastPath;
    QGraphicsPathItem* lastPathOPT;
    QGraphicsItem* lastItem;
    QImage image;
    cv::Mat cvImage, cvImagePN, cvImageCG, cvImagePT, cvImageSP, cvImageBG;
    pixelNode *imgNodes;
    pathCords* pathNow;
    pathCords* pathNowOPT;
    std::vector<pathCords> allPaths;

    void putAnchor(QPointF point);
    void updateFromCvToQt();
    void cvOpenCam();
    void cvOpenVideo();
    void calPixelNode();
    void calCostGraph();
    void calPathTree(int x, int y);
    pixelNode* drawCVPath(int x, int y);
    void drawQTPath(int x, int y);
    void drawQTPathOPT(int x, int y);
    void updateBG(pixelNode* seed);
    void backToStart();
};

#endif // NEWGRAPHICSVIEW_H
