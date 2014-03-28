#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    picView = new NewGraphicsView(this);
    setCentralWidget(picView);

    setWindowTitle(tr("Image Scisscor"));
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    cv::destroyAllWindows();
    close();
}

void MainWindow::on_actionOpen_triggered()
{
    picView->openAct();
}

void MainWindow::on_actionPixelNode_triggered()
{
    picView->showPixelNode();
}

void MainWindow::on_actionCostGraph_triggered()
{
    picView->showCostGraph();
}

void MainWindow::on_actionPathTree_triggered()
{
    picView->showPathTree();
}

void MainWindow::on_actionMinPath_triggered()
{
    if(!picView->showMinPath()) this->ui->actionMinPath->setChecked(false);
}

void MainWindow::on_actionPathTreeAni_triggered()
{
    picView->switchAnimation();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F)
    {
        emit this->ui->actionMinPath->trigger();
    }
}

void MainWindow::on_actionSave_triggered()
{
    picView->savePicture();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("Usage"),
                       tr("<h4>Key ShortCut: </h4>"
                          "<p>F: switch between Edit Mode and Draw Mode</p>"
                          "<p>D: delete the chosen line segment</p>"
                          "<p>B: back to the start seed point</p>"
                          ));

}

void MainWindow::on_actionAbout_2_triggered()
{
    QMessageBox::about(this, tr("About"),
                       tr("<h4>Image Scissor: Have Fun :)</h4>"
                          "<p>Version: 0.1</p>"
                          "<p>Author: Zheng Rui</p>"
                          ));
}
