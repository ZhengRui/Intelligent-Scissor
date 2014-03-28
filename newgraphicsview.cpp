#include "newgraphicsview.h"
#include "FibonacciHeap.h"
#include "asmOpenCV.h"

#include <QtWidgets>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <cmath>
#include <unistd.h>

using namespace std;

NewGraphicsView::NewGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //Set-up the scene
    Scene = new QGraphicsScene(this);
    Scene->setBackgroundBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
    setScene(Scene);

    //Set-up the view
//    setSceneRect(0, 0, 1000, 1000);

    //Use ScrollHand Drag Mode to enable Panning
    setDragMode(ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    viewport()->setCursor(Qt::ArrowCursor);
    _pan = false;
    _hasimage = false;
    _minpath = false;
    _animation = false;
    _ignore_last_seed = true;
    image = QImage();
    imgNodes = NULL;
    lastPath = NULL;
    lastPathOPT = NULL;
    lastItem = NULL;
    maxD = 0;
}

void NewGraphicsView::wheelEvent(QWheelEvent* event)
{
    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
    // Don't call superclass handler here
    // as wheel is normally used for moving scrollbars
}

void NewGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        _pan = true;
        _panStartX = event->x();
        _panStartY = event->y();
        viewport()->setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    } else if (event->button() == Qt::LeftButton)
    {
        if (_hasimage)
        {
            QPointF imgPt = mapToScene(event->pos());
            QPoint imgPix = imgPt.toPoint();
         //   qDebug() << image.size();
         //   cout << imgPt.x() << ", " << imgPt.y() << " -> " << imgPix.x() << ", " << imgPix.y() << endl;
            if (imgPix.x() >=0 && imgPix.x() < image.size().width() && imgPix.y() >=0 && imgPix.y() < image.size().height())
            {
                if(_minpath)
                {
                    if (!cvImageSP.empty()){
                        if (QApplication::keyboardModifiers() && Qt::ControlModifier)
                        {
                         //   qDebug() << lastPathOPT;
                            lastPathOPT->setPen(QPen(QBrush(QColor(59, 143, 23, 255), Qt::Dense1Pattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                            lastPathOPT = NULL;


                            pixelNode* optNode = drawCVPath(imgPix.x(),imgPix.y());
                            if (!_ignore_last_seed)
                            {
                                updateBG(optNode);
                                allPaths.push_back(*pathNowOPT);
                            //    cout << "**** insert path: " << pathNowOPT << ", Paths Number: " << allPaths.size() << endl;
                            }
                            calPathTree(optNode->column, optNode->row);
                        } else {
                         //   qDebug() << lastPath;
                            lastPath = NULL;
                            if(!_ignore_last_seed)
                            {
                                updateBG(&imgNodes[imgPix.y() * cvImage.cols + imgPix.x()]);
                                allPaths.push_back(*pathNow);
                            //    cout << "**** insert path: " << pathNow << ", Paths Number: " << allPaths.size() << endl;
                            }
                            calPathTree(imgPix.x(), imgPix.y());
                        }
                    } else {
                        calPathTree(imgPix.x(), imgPix.y());
                    }
                    _ignore_last_seed = false;
                } else {
                    calPathTree(imgPix.x(), imgPix.y());
                    QGraphicsItem* selPath = Scene->itemAt(imgPt, QGraphicsView::transform());

                    if (lastItem != NULL && lastItem->type() == 2)
                    {
                        QGraphicsDropShadowEffect* pre_Eff = new QGraphicsDropShadowEffect(this);
                        pre_Eff->setColor(QColor(59,143,23,255));
                        pre_Eff->setBlurRadius(0);
                        pre_Eff->setOffset(0);
                        lastItem->setGraphicsEffect(pre_Eff);
                    }

                    if (selPath->type() == 2)
                    {
                        QGraphicsDropShadowEffect* picked_Eff = new QGraphicsDropShadowEffect(this);
                        picked_Eff->setColor(QColor(255,0,0,255));
                        picked_Eff->setOffset(0);
                        picked_Eff->setBlurRadius(2);
                        selPath->setGraphicsEffect(picked_Eff);
                        lastItem = selPath;
                    }
                }
            }
         }
    }
    event->ignore();
}

void NewGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        _pan = false;
        viewport()->setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void NewGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (_pan)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        event->accept();
        return;
    }

    QGraphicsView::mouseMoveEvent(event);

    QPointF imgPt = mapToScene(event->pos());
    QPoint imgPix = imgPt.toPoint();
    if (imgPix.x() >=0 && imgPix.x() < image.size().width() && imgPix.y() >=0 && imgPix.y() < image.size().height())
    {
        if (_minpath && !_ignore_last_seed)
        {
            drawCVPath(imgPix.x(), imgPix.y());
            drawQTPathOPT(imgPix.x(), imgPix.y());
            drawQTPath(imgPix.x(), imgPix.y());
        }
    }
 //   event->ignore();
}

void NewGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F)
    {
        event->ignore();
    } else if (event->key() == Qt::Key_D) {
        if (!cvImagePT.empty() && !_minpath && lastItem != NULL && lastItem->type() == 2)
        {
            cvImageBG =cvImageCG.clone();
            std::vector<pathCords>::iterator iter;

        //    cout << "------------------------" << endl << "Paths Number: " << allPaths.size() << endl;
            for(iter = allPaths.begin(); iter != allPaths.end();)
            {
                pathCords path = *iter;
                if(path.whichQtItem == lastItem)
                {
                //    cout << "Delete Path: " << path.whichQtItem << endl;
                    iter = allPaths.erase(iter);
                } else {
                    for(int i=0; i<path.pLen-1; i++)
                    {
                        cv::line(cvImageBG, cv::Point(3*path.p[2*i+1], 3*path.p[2*i]), cv::Point(3*path.p[2*i+3], 3*path.p[2*i+2]), CV_RGB(59,143,23), 2, CV_AA);
                    }
                    iter++;
                }
            }



            cvImageSP = cvImageBG.clone();
            imshow("Shortest Path", cvImageSP);

        //    cout << "+++++++++++++++++" << endl << "Items Number: " << Scene->items().size() << endl;
        //    qDebug() << Scene->items();
            Scene->removeItem(lastItem);
        //    cout << "Delete Item: " << lastItem << endl;
            lastItem = NULL;
        }
    } else if (event->key() == Qt::Key_B && _minpath) {
        backToStart();
    }

}



void NewGraphicsView::openAct()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

 //   qDebug() << fileName.toUtf8();

    if (!fileName.isEmpty()) {
        image = QImage(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Scissor"), tr("Cannot load %1.").arg(fileName));
            return;
        }
        Scene->clear();
        allPaths.clear();
        Scene->addPixmap(QPixmap::fromImage(image));
        lastPath = NULL;
        lastPathOPT = NULL;
        lastItem = NULL;
        resetTransform();
        horizontalScrollBar()->setValue(0);
        verticalScrollBar()->setValue(0);
        _pan = false;
        _hasimage = true;
    //    qDebug() << Scene->items();

        cv::destroyAllWindows();
        delete[] imgNodes;

        cvImagePT.release();
        cvImageSP.release();
        cvImageBG.release();

        cvImage= cv::imread(fileName.toUtf8().data());

        if (!cvImage.empty()) {
            int nc = cvImage.cols;
            int nr = cvImage.rows;
            imgNodes = new pixelNode[nr*nc];
            for (int j=0; j<nr; j++)
            {
                for (int i=0; i<nc; i++)
                {
                    imgNodes[j*nc+i].row = j;
                    imgNodes[j*nc+i].column = i;
                    for (int k=0; k<3; k++)
                    {
                        for (int l=0; l<3; l++) imgNodes[j*nc+i].linkCost[k][l] = 100000;
                    }
                }
            }
//            cv::namedWindow("Image in CV");
//            cv::imshow("Image in CV", cvImage);
            calPixelNode();
            calCostGraph();
        } else {
            QMessageBox::information(this, tr("Image Scissor"), tr("Cannot load %1. into openCV").arg(fileName));
            cvImage.release();
            cvImagePN.release();
            cvImageCG.release();
            return;
        }
    }
}

void NewGraphicsView::updateFromCvToQt()
{
    Scene->clear();
    cv::cvtColor(cvImage,cvImage,CV_BGR2RGB);
    Scene->addPixmap(QPixmap::fromImage(QImage((const unsigned char*)(cvImage.data), cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_RGB888)));
}

void NewGraphicsView::cvOpenCam()
{
    cv::VideoCapture cap(0);
    bool bSuccess;

    if (!cap.isOpened())
    {
        cout << "Cannot open the video cam" << endl;
        return;
    }

    cv::namedWindow("Cam In CV", CV_WINDOW_AUTOSIZE);

    while(1)
    {
        bSuccess = cap.read(cvImage);
        if (!bSuccess)
        {
            cout << "Cannot read a frame from video frame" << endl;
            break;
        }

        cv::imshow("Cam In CV", cvImage);
        updateFromCvToQt();

        if (cv::waitKey(30) == 27)
        {
            cout << "Esc key is pressed by user" << endl;
            break;
        }
    }
}

void NewGraphicsView::cvOpenVideo()
{
    cv::VideoCapture cap("/home/zerry/视频/The Canyons - Official Trailer (HD) Lindsay Lohan.mp4");
    bool bSuccess;

    if (!cap.isOpened())
    {
        cout << "Cannot open the video file" << endl;
        return;
    }

    // cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

    cout << "Frame per seconds : " << fps << endl;

    cv::namedWindow("Video In CV", CV_WINDOW_AUTOSIZE);

    while(1)
    {
        bSuccess = cap.read(cvImage);
        if (!bSuccess)
        {
            cout << "Cannot read a frame from video frame" << endl;
            break;
        }

        cv::imshow("Video In CV", cvImage);
        updateFromCvToQt();

        if (cv::waitKey(30) == 27)
        {
            cout << "Esc key is pressed by user" << endl;
            break;
        }
    }

}

void NewGraphicsView::showPixelNode()
{
    if (!cvImagePN.empty())
    {
        cv::namedWindow("Pixel Node", CV_WINDOW_AUTOSIZE);
        cv::imshow("Pixel Node", cvImagePN);
    }
}

void NewGraphicsView::showCostGraph()
{
    if (!cvImageCG.empty())
    {
        cv::namedWindow("Cost Graph", CV_WINDOW_AUTOSIZE);
        cv::imshow("Cost Graph", cvImageCG);
    }
}

void NewGraphicsView::showPathTree()
{
    if (!cvImagePT.empty())
    {
        cv::namedWindow("Path Tree", CV_WINDOW_AUTOSIZE);
        cv::imshow("Path Tree", cvImagePT);
    }
}

bool NewGraphicsView::showMinPath()
{
    if(!cvImagePT.empty())
    {
        _minpath = !_minpath;
        if (_minpath)
        {
            _ignore_last_seed = true;
        } else {
            if (lastPath != NULL)
            {
                Scene->removeItem(lastPath);
            }
            if (lastPathOPT != NULL)
            {
                Scene->removeItem(lastPathOPT);
            }
            cvImageSP = cvImageBG.clone();
            imshow("Shortest Path", cvImageSP);
        }
        lastPath = NULL;
        lastPathOPT = NULL;
        return true;
    }
    return false;
}

void NewGraphicsView::calPixelNode()
{
    if (!cvImage.empty())
    {
        int nr = cvImage.rows;
        int nc = cvImage.cols;
        cvImagePN = cv::Mat::zeros(3 * nr - 2, 3 * nc -2, cvImage.type());
        int nch = cvImage.channels();
        int gap_PN = 2 * nch;
        uchar *ptr_cvImage, *ptr_cvImagePN;

        for (int j=0; j<nr; j++)
        {
            ptr_cvImage = cvImage.ptr(j);
            ptr_cvImagePN = cvImagePN.ptr(3*j);
            for (int i=0; i<nc; i++)
            {
                for (int k=0; k<nch; k++)
                {
                    *ptr_cvImagePN++ = *ptr_cvImage++;
                }
                ptr_cvImagePN += gap_PN;
            }
        }
    }
}

void NewGraphicsView::calCostGraph()
{
    if (!cvImage.empty())
    {
       // cvImageCG = cvImagePN.clone();
        int nr = cvImage.rows;
        int nc = cvImage.cols;
        cvImageCG = cv::Mat::zeros(3 * nr - 2, 3 * nc -2, cvImage.type());
        int nch = cvImage.channels();
        int unit_PN = 3 * nch;
        int i, j, k;
        uchar *prt_cvImage_pre, *prt_cvImage_cur, *prt_cvImage_nex, \
                *prt_cvImageCG, *prt_cvImageCG_pre, *prt_cvImageCG_cur, *prt_cvImageCG_nex;
        maxD = 0;

        for (j=1; j<nr-1; j++)
        {
            prt_cvImage_pre = cvImage.ptr(j-1);
            prt_cvImage_cur = cvImage.ptr(j);
            prt_cvImage_nex = cvImage.ptr(j+1);
            prt_cvImageCG_pre = cvImageCG.ptr(3*j-1);
            prt_cvImageCG_cur = cvImageCG.ptr(3*j);
            prt_cvImageCG_nex = cvImageCG.ptr(3*j+1);
            for (i=1; i<nc-1; i++)
            {
                int Dlink[8] = {};
                for (k=0; k<nch; k++)
                {
                    prt_cvImageCG_cur[unit_PN*i+nch+k] = abs(prt_cvImage_pre[nch*i+k]+prt_cvImage_pre[nch*(i+1)+k]-prt_cvImage_nex[nch*i+k]-prt_cvImage_nex[nch*(i+1)+k])/4.0;           // D(0)
                    Dlink[0] += prt_cvImageCG_cur[unit_PN*i+nch+k] * prt_cvImageCG_cur[unit_PN*i+nch+k];
                    prt_cvImageCG_cur[unit_PN*i-nch+k] = abs(prt_cvImage_pre[nch*i+k]+prt_cvImage_pre[nch*(i-1)+k]-prt_cvImage_nex[nch*i+k]-prt_cvImage_nex[nch*(i-1)+k])/4.0;           // D(4)
                    Dlink[4] += prt_cvImageCG_cur[unit_PN*i-nch+k] * prt_cvImageCG_cur[unit_PN*i-nch+k];
                    prt_cvImageCG_pre[unit_PN*i+k] = abs(prt_cvImage_pre[nch*(i-1)+k]-prt_cvImage_pre[nch*(i+1)+k]+prt_cvImage_cur[nch*(i-1)+k]-prt_cvImage_cur[nch*(i+1)+k])/4.0;       // D(2)
                    Dlink[2] += prt_cvImageCG_pre[unit_PN*i+k] * prt_cvImageCG_pre[unit_PN*i+k];
                    prt_cvImageCG_nex[unit_PN*i+k] = abs(prt_cvImage_nex[nch*(i-1)+k]-prt_cvImage_nex[nch*(i+1)+k]+prt_cvImage_cur[nch*(i-1)+k]-prt_cvImage_cur[nch*(i+1)+k])/4.0;       // D(6)
                    Dlink[6] += prt_cvImageCG_nex[unit_PN*i+k] * prt_cvImageCG_nex[unit_PN*i+k];
                    prt_cvImageCG_pre[unit_PN*i+nch+k] = abs(prt_cvImage_cur[nch*(i+1)+k]-prt_cvImage_pre[nch*i+k])/sqrt(2.);   // D(1)
                    Dlink[1] += prt_cvImageCG_pre[unit_PN*i+nch+k] * prt_cvImageCG_pre[unit_PN*i+nch+k];
                    prt_cvImageCG_pre[unit_PN*i-nch+k] = abs(prt_cvImage_cur[nch*(i-1)+k]-prt_cvImage_pre[nch*i+k])/sqrt(2.);   // D(3)
                    Dlink[3] += prt_cvImageCG_pre[unit_PN*i-nch+k] * prt_cvImageCG_pre[unit_PN*i-nch+k];
                    prt_cvImageCG_nex[unit_PN*i-nch+k] = abs(prt_cvImage_cur[nch*(i-1)+k]-prt_cvImage_nex[nch*i+k])/sqrt(2.);   // D(5)
                    Dlink[5] += prt_cvImageCG_nex[unit_PN*i-nch+k] * prt_cvImageCG_nex[unit_PN*i-nch+k];
                    prt_cvImageCG_nex[unit_PN*i+nch+k] = abs(prt_cvImage_cur[nch*(i+1)+k]-prt_cvImage_nex[nch*i+k])/sqrt(2.);   // D(7)
                    Dlink[7] += prt_cvImageCG_nex[unit_PN*i+nch+k] * prt_cvImageCG_nex[unit_PN*i+nch+k];
                }
                for (k=0; k<8; k++)
                {
                    Dlink[k] = sqrt(sqrt(Dlink[k]*1.0/nch));
                 //   Dlink[k] = sqrt(Dlink[k]*1.0/nch);
                    if (maxD < Dlink[k]) maxD = Dlink[k];
                }
                for (k=0; k<nch; k++)
                {
                    prt_cvImageCG_cur[unit_PN*i+nch+k] = Dlink[0];
                    prt_cvImageCG_cur[unit_PN*i-nch+k] = Dlink[4];
                    prt_cvImageCG_pre[unit_PN*i+k] = Dlink[2];
                    prt_cvImageCG_nex[unit_PN*i+k] = Dlink[6];
                    prt_cvImageCG_pre[unit_PN*i+nch+k] = Dlink[1];
                    prt_cvImageCG_pre[unit_PN*i-nch+k] = Dlink[3];
                    prt_cvImageCG_nex[unit_PN*i-nch+k] = Dlink[5];
                    prt_cvImageCG_nex[unit_PN*i+nch+k] = Dlink[7];
                }
            }
        }

      //  cout << maxD << endl;

        for (j=2; j<3*nr-4; j++)
        {
            prt_cvImageCG = cvImageCG.ptr(j);
            if (j%3)
            {
                for (k=0; k<2*nch; k++)   *prt_cvImageCG++ = 255;
                for (i=2; i<3*nc-4; i=i+3)
                {
                    *prt_cvImageCG = maxD * (maxD - (*prt_cvImageCG)) * sqrt(2.);
                    prt_cvImageCG++;
                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
                    *prt_cvImageCG = maxD * (maxD - (*prt_cvImageCG));
                    prt_cvImageCG++;
                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
                    *prt_cvImageCG = maxD * (maxD - (*prt_cvImageCG)) * sqrt(2.);
                    prt_cvImageCG++;
                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
                }
                for (k=0; k<2*nch; k++)   *prt_cvImageCG++ = 255;
            } else {
                prt_cvImageCG += nch;
                for (k=0; k<nch; k++, prt_cvImageCG++)  *prt_cvImageCG = maxD * (maxD - (*(prt_cvImageCG+nch)));
                for (i=2; i<3*nc-4; i=i+3)
                {
                    *prt_cvImageCG = maxD * (maxD - (*prt_cvImageCG));
                    prt_cvImageCG++;
                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
                    prt_cvImageCG += nch;
                    *prt_cvImageCG = maxD * (maxD - (*prt_cvImageCG));
                    prt_cvImageCG++;
                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
                }
                for (k=0; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG-nch);
            }
        }

//        for (j=2; j<3*nr-4; j++)
//        {
//            prt_cvImageCG = cvImageCG.ptr(j);
//            if (j%3)
//            {
//                for (k=0; k<2*nch; k++)   *prt_cvImageCG++ = 255;
//                for (i=2; i<3*nc-4; i=i+3)
//                {
//                    *prt_cvImageCG = maxD - (*prt_cvImageCG) * sqrt(2.);
//                    prt_cvImageCG++;
//                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
//                    *prt_cvImageCG = maxD - (*prt_cvImageCG);
//                    prt_cvImageCG++;
//                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
//                    *prt_cvImageCG = (maxD - (*prt_cvImageCG)) * sqrt(2.);
//                    prt_cvImageCG++;
//                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
//                }
//                for (k=0; k<2*nch; k++)   *prt_cvImageCG++ = 255;
//            } else {
//                prt_cvImageCG += nch;
//                for (k=0; k<nch; k++, prt_cvImageCG++)  *prt_cvImageCG = maxD - (*(prt_cvImageCG+nch));
//                for (i=2; i<3*nc-4; i=i+3)
//                {
//                    *prt_cvImageCG = maxD - (*prt_cvImageCG);
//                    prt_cvImageCG++;
//                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
//                    prt_cvImageCG += nch;
//                    *prt_cvImageCG = maxD - (*prt_cvImageCG);
//                    prt_cvImageCG++;
//                    for (k=1; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG - 1);
//                }
//                for (k=0; k<nch; k++, prt_cvImageCG++)   *prt_cvImageCG = *(prt_cvImageCG-nch);
//            }
//        }

        for (j=4; j<3*nr-6; j=j+3)
        {
            prt_cvImageCG = cvImageCG.ptr(j);
            prt_cvImageCG_nex = cvImageCG.ptr(j+1);
            prt_cvImageCG += nch;
            prt_cvImageCG_nex += nch;
            for (k=0; k<nch; k++)
            {
                *prt_cvImageCG = *(prt_cvImageCG_nex + nch);
                *prt_cvImageCG_nex = *(prt_cvImageCG + nch);
                prt_cvImageCG++;
                prt_cvImageCG_nex++;
            }
            prt_cvImageCG += unit_PN * (nc - 2);
            prt_cvImageCG_nex += unit_PN * (nc - 2);
            for (k=0; k<nch; k++)
            {
                *prt_cvImageCG = *(prt_cvImageCG_nex - nch);
                *prt_cvImageCG_nex = *(prt_cvImageCG - nch);
                prt_cvImageCG++;
                prt_cvImageCG_nex++;
            }
        }


        prt_cvImageCG = cvImageCG.ptr(0);
        for (i=0; i<nc-1; i++)
        {
            prt_cvImageCG += nch;
            for (k=0; k<2*nch; k++)   *prt_cvImageCG++ = 255;
        }



        prt_cvImageCG = cvImageCG.ptr(1);
        prt_cvImageCG_nex = cvImageCG.ptr(2);
        for (k=0; k<nch; k++)   *prt_cvImageCG++ = 255;
        prt_cvImageCG_nex += 2*nch;
        for (k=0; k<nch; k++) *prt_cvImageCG++ = *prt_cvImageCG_nex++;
        for (i=1; i<nc-1; i++)
        {
            prt_cvImageCG_nex -= 2*nch;
            for (k=0; k<nch; k++)   *prt_cvImageCG++ = *prt_cvImageCG_nex++;
            prt_cvImageCG_nex += nch;
            for (k=0; k<nch; k++)   *prt_cvImageCG++ = *prt_cvImageCG_nex++;
            prt_cvImageCG_nex += nch;
            for (k=0; k<nch; k++)   *prt_cvImageCG++ = *prt_cvImageCG_nex++;
        }
        prt_cvImageCG_nex -= 2*nch;
        for (k=0; k<nch; k++)   *prt_cvImageCG++ = *prt_cvImageCG_nex++;
        for (k=0; k<nch; k++) *prt_cvImageCG++ = 255;



        prt_cvImageCG = cvImageCG.ptr(3*nr-4);
        prt_cvImageCG_pre = cvImageCG.ptr(3*nr-5);
        for (k=0; k<nch; k++)   *prt_cvImageCG++ = 255;
        prt_cvImageCG_pre += 2*nch;
        for (k=0; k<nch; k++) *prt_cvImageCG++ = *prt_cvImageCG_pre++;
        for (i=1; i<nc-1; i++)
        {
            prt_cvImageCG_pre -= 2*nch;
            for (k=0; k<nch; k++)   *prt_cvImageCG++ = *prt_cvImageCG_pre++;
            prt_cvImageCG_pre += nch;
            for (k=0; k<nch; k++)   *prt_cvImageCG++ = *prt_cvImageCG_pre++;
            prt_cvImageCG_pre += nch;
            for (k=0; k<nch; k++)   *prt_cvImageCG++ = *prt_cvImageCG_pre++;
        }
        prt_cvImageCG_pre -= 2*nch;
        for (k=0; k<nch; k++)   *prt_cvImageCG++ = *prt_cvImageCG_pre++;
        for (k=0; k<nch; k++) *prt_cvImageCG++ = 255;



        prt_cvImageCG = cvImageCG.ptr(3*nr-3);
        for (i=0; i<nc-1; i++)
        {
            prt_cvImageCG += nch;
            for (k=0; k<2*nch; k++)   *prt_cvImageCG++ = 255;
        }

        for (j=0; j<3*nr-2; j++)
        {
            prt_cvImageCG = cvImageCG.ptr(j);
            for (i=0; i<3*nc-2; i++)
            {
            //    cout << (j+1)/3 << ", " << (i+1)/3 << ",   " << j%3 << ", " << i%3 << ": " << int(*prt_cvImageCG) <<endl;
            //    cout<< imgNodes[(j+1)/3 * nc + (i+1)/3].row << ", " << imgNodes[(j+1)/3 * nc + (i+1)/3].column << endl;
                imgNodes[(j+1)/3 * nc + (i+1)/3].linkCost[(j%3+1)%3][(i%3+1)%3] = *prt_cvImageCG;
                prt_cvImageCG += nch;
            }
        }
    }
    cvImageBG = cvImageCG.clone();
}


void NewGraphicsView::switchAnimation()
{
    _animation = !_animation;
}

void NewGraphicsView::calPathTree(int x, int y)
{
    if (!cvImage.empty())
    {
        int nr = cvImage.rows;
        int nc = cvImage.cols;
        int nch = cvImage.channels();
      //  cvImagePT = cvImageCG.clone();
        cvImagePT = cv::Mat::zeros(3 * nr - 2, 3 * nc -2, cvImage.type());

      //  uchar *prt_cvImagePT = cvImagePT.ptr(3*y);
      //  *(prt_cvImagePT+3*x*nch+2) = 255;


        for (int j=0; j<nr; j++)
        {
            for (int i=0; i<nc; i++)
            {
                imgNodes[j*nc+i].state = 0;
                imgNodes[j*nc+i].totalCost = 0;
                imgNodes[j*nc+i].prevNode = NULL;
            /*  cout << endl << j << ", " << i <<endl;
                cout << imgNodes[j*nc+i].linkCost[2][2] << " " << imgNodes[j*nc+i].linkCost[2][0] << " " << imgNodes[j*nc+i].linkCost[2][1] << endl << imgNodes[j*nc+i].linkCost[0][2]\
                        << " " << imgNodes[j*nc+i].linkCost[0][0] << " " << imgNodes[j*nc+i].linkCost[0][1] << endl << imgNodes[j*nc+i].linkCost[1][2] << " " << imgNodes[j*nc+i].linkCost[1][0] \
                        << " " << imgNodes[j*nc+i].linkCost[1][1] <<endl; */
            }
        }

        if (_animation)
        {
            cv::namedWindow("Path Tree", CV_WINDOW_AUTOSIZE);
            cv::circle(cvImagePT, cv::Point(3*x, 3*y), 3, CV_RGB(255,0,0), -1);
            cv::imshow("Path Tree", cvImagePT);

        }

        FibonacciHeap* heap = new FibonacciHeap();

        pixelNode *seed = &imgNodes[y*nc+x];
        seed->state = 1;
        heap->insertVertex(new Node(seed, seed->totalCost));

        int counter = 0;
        do
        {

            pixelNode* v = heap->deleteMin()->pixel;
            v->state = 2;
            x = v->column;
            y = v->row;

            if (counter)
            {
                int k = v->prevNode->row - y;
                int l = v->prevNode->column - x;
                uchar* prt_cvImagePT = cvImagePT.ptr(3*y+k)+(3*x+l)*nch;
                *prt_cvImagePT++ = 0;
                *prt_cvImagePT++ = 160;
                *prt_cvImagePT++ = 0;
                prt_cvImagePT = cvImagePT.ptr(3*y+2*k)+(3*x+2*l)*nch;
                *prt_cvImagePT++ = 0;
                *prt_cvImagePT++ = 200;
                *prt_cvImagePT++ = 0;
            }

            int kmin = -1, kmax = 1, lmin = -1, lmax = 1;
            if (y == 0) kmin = 1;
            else if (y == nr-1) kmax = -1;
            if (x == 0) lmin = 1;
            else if (x == nc-1) lmax = -1;

          //  cout << "-----------" << endl;
            for (int k=kmin; k<=kmax; k++)
            {
                for (int l=lmin; l<=lmax; l++)
                {
                    if (k || l)
                    {
                        pixelNode* nbPixel = &imgNodes[(y+k)*nc+(x+l)];
                  //      cout << y << ", " << x << " to " << nbPixel->row << ", " << nbPixel->column <<endl;
                        if (nbPixel->state != 2)
                        {
                            if (nbPixel->state == 0)
                            {
                                nbPixel->state = 1;
                                nbPixel->totalCost = v->totalCost + v->linkCost[k+1][l+1];
                                nbPixel->prevNode = v;

                                Node* w = new Node(nbPixel, nbPixel->totalCost);
                                nbPixel->fibNode = w;
                                heap->insertVertex(w);

                            } else if (nbPixel->totalCost > v->totalCost + v->linkCost[k+1][l+1])
                            {
                                nbPixel->totalCost = v->totalCost + v->linkCost[k+1][l+1];
                                nbPixel->prevNode = v;
                                heap->decreaseKey(nbPixel->totalCost, nbPixel->fibNode);
                            }
                        }
                    }
                }
            }
            if (_animation && !(counter%500))
            {
                cv::imshow("Path Tree", cvImagePT);
                cv::waitKey(5);
            }

        /*    if(heap->isEmpty())    cout << v->row << ", " << v->column << " TotalCost: " << v->totalCost << endl \
                                        << v->prevNode->row << ", " << v->prevNode->column << " TotalCost: " \
                                        << v->prevNode->totalCost << endl; */

            counter++;

        } while (!heap->isEmpty());

        if(_animation)  cv::imshow("Path Tree", cvImagePT);


       // cout << counter << endl;
    }
}

pixelNode* NewGraphicsView::drawCVPath(int x, int y)
{
    if(!cvImagePT.empty() && _minpath)
    {
        cv::namedWindow("Shortest Path", CV_WINDOW_AUTOSIZE);
        cvImageSP = cvImageBG.clone();

        int dr = 3;
        int optLink = 1000;
        pixelNode* optPixel;
        pixelNode* end;
        for (int i=max(x-dr, 0); i<min(x+dr, cvImage.cols); i++)
        {
            for (int j=max(y-dr, 0); j<min(y+dr, cvImage.rows); j++)
            {
                pixelNode* end = &imgNodes[j * cvImage.cols + i];
                int minLink = 1000;
                for (int k=0; k<3; k++)
                {
                    for (int l=0; l<3; l++)
                    {
                        if (end->linkCost[k][l] < minLink) minLink = end->linkCost[k][l];
                    }
                }
                if (minLink < optLink)
                {
                    optLink = minLink;
                    optPixel = end;
                }
            }
        }
        end = optPixel;
        while(end->prevNode != NULL)
        {
            cv::line(cvImageSP, cv::Point(3*end->column, 3*end->row), cv::Point(3*end->prevNode->column, 3*end->prevNode->row), CV_RGB(23,133,143), 2, CV_AA);
            end = end->prevNode;
        }


        end = &imgNodes[y * cvImage.cols + x];

        while(end->prevNode != NULL)
        {
            cv::line(cvImageSP, cv::Point(3*end->column, 3*end->row), cv::Point(3*end->prevNode->column, 3*end->prevNode->row), CV_RGB(59,143,23), 2, CV_AA);
            end = end->prevNode;
        }
        cv::circle(cvImageSP, cv::Point(3*end->column, 3*end->row), 5, CV_RGB(195,92,204), -1);

        cv::imshow("Shortest Path", cvImageSP);
        return optPixel;
    }
    return NULL;
}

void NewGraphicsView::updateBG(pixelNode* seed)
{
    pixelNode* end = seed;
    while(end->prevNode != NULL)
    {
        cv::line(cvImageBG, cv::Point(3*end->column, 3*end->row), cv::Point(3*end->prevNode->column, 3*end->prevNode->row), CV_RGB(59,143,23), 2, CV_AA);
        end = end->prevNode;
    }
}


void NewGraphicsView::putAnchor(QPointF point)
{
    Scene->addEllipse(point.x(), point.y(), 6, 6);
}

void NewGraphicsView::drawQTPath(int x, int y)
{
    if(!cvImagePT.empty() && _minpath)
    {
        QPen pen(QBrush(QColor(59, 143, 23, 255), Qt::Dense1Pattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        QBrush brush;
        QPainterPath newpath;

        pixelNode* end = &imgNodes[y * cvImage.cols + x];
        if(lastPath != NULL)
        {
            Scene->removeItem(lastPath);

//            cout << "--- Destruction -------------------------" << endl;
//            for(int i=0; i<pathNow->pLen; i++)
//            {
//                cout << "(" << pathNow->p[2*i] << ", " << pathNow->p[2*i+1] << ")" << endl;
//            }

            pathNow->whichQtItem = NULL;
            delete [] pathNow->p;
            delete pathNow;
        }

        int len = 0;
        while(end->prevNode != NULL)
        {
            newpath.moveTo(end->column, end->row);
            newpath.lineTo(end->prevNode->column, end->prevNode->row);
            end = end->prevNode;
            len++;
        }
        len++;
//        cout << "~~~ Construction : ~~~ length: " << len << " ~~~ seed: (" << end->row << ", " << end->column << ")" << endl;

        lastPath = Scene->addPath(newpath, pen, brush);
        lastPath->setOpacity(0.8);

        pathNow = new pathCords;
        pathNow->pLen = len;
        pathNow->p = new int[2*len];
        pathNow->whichQtItem = lastPath;

        len = 0;
        end = &imgNodes[y * cvImage.cols + x];
        while(end->prevNode != NULL)
        {
            pathNow->p[2*len] = end->row;
            pathNow->p[2*len+1] = end->column;
            end = end->prevNode;
            len++;
        }
        pathNow->p[2*len] = end->row;
        pathNow->p[2*len+1] = end->column;

//        for(int i=0; i<pathNow->pLen; i++)
//        {
//            cout << "(" << pathNow->p[2*i] << ", " << pathNow->p[2*i+1] << ")" << endl;
//        }
//        cout << "----------------------------" << endl;
    }
}

void NewGraphicsView::drawQTPathOPT(int x, int y)
{
    if(!cvImagePT.empty() && _minpath)
    {
        QPen pen(QBrush(QColor(23, 133, 143, 255), Qt::Dense1Pattern), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        QBrush brush;
        QPainterPath newpath;

        int dr = 3;
        int optLink = 1000;
        pixelNode* optPixel;
        for (int i=max(x-dr, 0); i<min(x+dr, cvImage.cols); i++)
        {
            for (int j=max(y-dr, 0); j<min(y+dr, cvImage.rows); j++)
            {
                pixelNode* end = &imgNodes[j * cvImage.cols + i];
                int minLink = 1000;
                for (int k=0; k<3; k++)
                {
                    for (int l=0; l<3; l++)
                    {
                        if (end->linkCost[k][l] < minLink) minLink = end->linkCost[k][l];
                    }
                }
                if (minLink < optLink)
                {
                    optLink = minLink;
                    optPixel = end;
                }
            }
        }

        pixelNode* end = optPixel;
        if(lastPathOPT != NULL)
        {
            Scene->removeItem(lastPathOPT);
            pathNowOPT->whichQtItem = NULL;
            delete [] pathNowOPT->p;
            delete pathNowOPT;
        }

        int len = 0;
        while(end->prevNode != NULL)
        {
            newpath.moveTo(end->column, end->row);
            newpath.lineTo(end->prevNode->column, end->prevNode->row);
            end = end->prevNode;
            len++;
        }
        len++;

        lastPathOPT = Scene->addPath(newpath, pen, brush);
        lastPathOPT->setOpacity(0.8);

        pathNowOPT = new pathCords;
        pathNowOPT->pLen = len;
        pathNowOPT->p = new int[2*len];
        pathNowOPT->whichQtItem = lastPathOPT;

        len = 0;
        end = optPixel;
        while(end->prevNode != NULL)
        {
            pathNowOPT->p[2*len] = end->row;
            pathNowOPT->p[2*len+1] = end->column;
            end = end->prevNode;
            len++;
        }
        pathNowOPT->p[2*len] = end->row;
        pathNowOPT->p[2*len+1] = end->column;
    }
}


void NewGraphicsView::backToStart()
{
    if(allPaths.size() >= 2)
    {
        pathCords startPath = allPaths[0];
        int xstart = startPath.p[2*startPath.pLen-1];
        int ystart = startPath.p[2*startPath.pLen-2];

        drawQTPath(xstart, ystart);

        lastPath = NULL;
        if(!_ignore_last_seed)
        {
            updateBG(&imgNodes[ystart * cvImage.cols + xstart]);
            allPaths.push_back(*pathNow);
        //    cout << "**** insert path: " << pathNow << ", Paths Number: " << allPaths.size() << endl;
        }

        calPathTree(xstart, ystart);
    }
}

void NewGraphicsView::savePicture()
{

    if(allPaths.size() != 0)
    {
        QPolygon region;

        std::vector<pathCords>::iterator iter;

        for(iter = allPaths.begin(); iter != allPaths.end(); iter++)
        {
            pathCords path = *iter;

            for(int i=path.pLen-1; i>=0; i--)
            {
                QPoint newPixel;
                newPixel = QPoint(path.p[2*i+1], path.p[2*i]);
                region << newPixel;
            }
        }

        QPainterPath path;
        path.addPolygon(region);


        QImage image_cut(image.size(), QImage::Format_ARGB32_Premultiplied);
        image_cut.fill(Qt::transparent);
        QPainter p(&image_cut);
        p.setClipPath(path);
        p.drawImage(0, 0, image);
        p.end();

        QGraphicsScene* newScene = new QGraphicsScene(this);
        newScene->addPixmap(QPixmap::fromImage(image_cut));
        newScene->setBackgroundBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
        setScene(newScene);


        QByteArray fileFormat("png");

        QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                        initialPath,
                                                        tr("%1 Files (*.%2);;All Files (*)")
                                                        .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                        .arg(QString::fromLatin1(fileFormat)));
        if (!fileName.isEmpty()) {
            image_cut.save(fileName, fileFormat.constData());
        }

        newScene->clear();
        setScene(Scene);
    }
}

