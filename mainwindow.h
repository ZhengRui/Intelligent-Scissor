#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "newgraphicsview.h"




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionPixelNode_triggered();

    void on_actionCostGraph_triggered();

    void on_actionPathTree_triggered();

    void on_actionMinPath_triggered();

    void on_actionPathTreeAni_triggered();

    void on_actionSave_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_2_triggered();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;

    NewGraphicsView *picView;

};

#endif // MAINWINDOW_H
