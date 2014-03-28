/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionPixelNode;
    QAction *actionCostGraph;
    QAction *actionPathTree;
    QAction *actionMinPath;
    QAction *actionPathTreeAni;
    QAction *actionSave;
    QAction *actionAbout;
    QAction *actionAbout_2;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/document-new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icons/system-shutdown-panel.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon1);
        actionPixelNode = new QAction(MainWindow);
        actionPixelNode->setObjectName(QStringLiteral("actionPixelNode"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/icons/edit-find.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPixelNode->setIcon(icon2);
        actionCostGraph = new QAction(MainWindow);
        actionCostGraph->setObjectName(QStringLiteral("actionCostGraph"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/icons/gnome-sudoku.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCostGraph->setIcon(icon3);
        actionPathTree = new QAction(MainWindow);
        actionPathTree->setObjectName(QStringLiteral("actionPathTree"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/icons/dead-tree-20.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPathTree->setIcon(icon4);
        actionMinPath = new QAction(MainWindow);
        actionMinPath->setObjectName(QStringLiteral("actionMinPath"));
        actionMinPath->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/icons/format-text-direction-ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMinPath->setIcon(icon5);
        actionPathTreeAni = new QAction(MainWindow);
        actionPathTreeAni->setObjectName(QStringLiteral("actionPathTreeAni"));
        actionPathTreeAni->setCheckable(true);
        actionPathTreeAni->setEnabled(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/icons/small_tree.gif"), QSize(), QIcon::Normal, QIcon::Off);
        actionPathTreeAni->setIcon(icon6);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/icons/document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon7);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAbout_2 = new QAction(MainWindow);
        actionAbout_2->setObjectName(QStringLiteral("actionAbout_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuAbout->addAction(actionAbout);
        menuAbout->addAction(actionAbout_2);
        mainToolBar->addAction(actionPixelNode);
        mainToolBar->addAction(actionCostGraph);
        mainToolBar->addAction(actionPathTree);
        mainToolBar->addAction(actionMinPath);
        mainToolBar->addAction(actionPathTreeAni);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "&Open", 0));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionExit->setText(QApplication::translate("MainWindow", "E&xit", 0));
        actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        actionPixelNode->setText(QApplication::translate("MainWindow", "PixelNode", 0));
#ifndef QT_NO_TOOLTIP
        actionPixelNode->setToolTip(QApplication::translate("MainWindow", "Show Pixel Node", 0));
#endif // QT_NO_TOOLTIP
        actionCostGraph->setText(QApplication::translate("MainWindow", "CostGraph", 0));
#ifndef QT_NO_TOOLTIP
        actionCostGraph->setToolTip(QApplication::translate("MainWindow", "Show Cost Graph", 0));
#endif // QT_NO_TOOLTIP
        actionPathTree->setText(QApplication::translate("MainWindow", "PathTree", 0));
#ifndef QT_NO_TOOLTIP
        actionPathTree->setToolTip(QApplication::translate("MainWindow", "Show Path Tree", 0));
#endif // QT_NO_TOOLTIP
        actionMinPath->setText(QApplication::translate("MainWindow", "MinPath", 0));
#ifndef QT_NO_TOOLTIP
        actionMinPath->setToolTip(QApplication::translate("MainWindow", "Show Minimum Path", 0));
#endif // QT_NO_TOOLTIP
        actionPathTreeAni->setText(QApplication::translate("MainWindow", "PathTreeAni", 0));
#ifndef QT_NO_TOOLTIP
        actionPathTreeAni->setToolTip(QApplication::translate("MainWindow", "Path Tree Generating Animation", 0));
#endif // QT_NO_TOOLTIP
        actionSave->setText(QApplication::translate("MainWindow", "&Save", 0));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "Usage", 0));
#ifndef QT_NO_TOOLTIP
        actionAbout->setToolTip(QApplication::translate("MainWindow", "Usage", 0));
#endif // QT_NO_TOOLTIP
        actionAbout_2->setText(QApplication::translate("MainWindow", "About", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0));
        menuAbout->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
