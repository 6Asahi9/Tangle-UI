/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actiontesting_for_edit;
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QGraphicsView *graphicsViewCanvas;
    QWidget *tab_2;
    QTextEdit *txtConsole;
    QWidget *matplotlibWidget;
    QWidget *tab_3;
    QPlainTextEdit *plainTextEdit;
    QTreeWidget *treeWidget;
    QTreeWidget *treeWidget_2;
    QPushButton *btnRun;
    QPushButton *btnEngine;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuCustom;
    QMenu *menuSettings;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1207, 789);
        actiontesting_for_edit = new QAction(MainWindow);
        actiontesting_for_edit->setObjectName("actiontesting_for_edit");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(140, 20, 921, 711));
        tabWidget->setMaximumSize(QSize(921, 16777215));
        tabWidget->setAcceptDrops(true);
        tabWidget->setTabPosition(QTabWidget::TabPosition::North);
        tabWidget->setTabBarAutoHide(true);
        tab_1 = new QWidget();
        tab_1->setObjectName("tab_1");
        tab_1->setAcceptDrops(true);
        graphicsViewCanvas = new QGraphicsView(tab_1);
        graphicsViewCanvas->setObjectName("graphicsViewCanvas");
        graphicsViewCanvas->setGeometry(QRect(-5, 1, 921, 671));
        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        txtConsole = new QTextEdit(tab_2);
        txtConsole->setObjectName("txtConsole");
        txtConsole->setGeometry(QRect(3, 4, 911, 341));
        matplotlibWidget = new QWidget(tab_2);
        matplotlibWidget->setObjectName("matplotlibWidget");
        matplotlibWidget->setGeometry(QRect(9, 349, 901, 311));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        plainTextEdit = new QPlainTextEdit(tab_3);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setGeometry(QRect(3, 4, 911, 661));
        tabWidget->addTab(tab_3, QString());
        treeWidget = new QTreeWidget(centralwidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setGeometry(QRect(10, 20, 121, 711));
        treeWidget->setDragEnabled(true);
        treeWidget->setDragDropOverwriteMode(true);
        treeWidget->setDragDropMode(QAbstractItemView::DragDropMode::DragOnly);
        treeWidget_2 = new QTreeWidget(centralwidget);
        treeWidget_2->setObjectName("treeWidget_2");
        treeWidget_2->setGeometry(QRect(1070, 400, 131, 331));
        btnRun = new QPushButton(centralwidget);
        btnRun->setObjectName("btnRun");
        btnRun->setGeometry(QRect(1110, 0, 81, 29));
        btnRun->setStyleSheet(QString::fromUtf8("background-color: #22cc22;\n"
"color: white;\n"
"border-radius: 8px;\n"
"padding: 4px 10px;\n"
"font-weight: bold;\n"
""));
        btnEngine = new QPushButton(centralwidget);
        btnEngine->setObjectName("btnEngine");
        btnEngine->setGeometry(QRect(969, 0, 131, 29));
        btnEngine->setStyleSheet(QString::fromUtf8("background-color: #22cc22;\n"
"color: white;\n"
"border-radius: 8px;\n"
"padding: 4px 10px;\n"
"font-weight: bold;\n"
"\n"
""));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1207, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        menuCustom = new QMenu(menubar);
        menuCustom->setObjectName("menuCustom");
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName("menuSettings");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuCustom->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menuEdit->addAction(actiontesting_for_edit);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actiontesting_for_edit->setText(QCoreApplication::translate("MainWindow", "testing for edit", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QCoreApplication::translate("MainWindow", "Canvas", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Console", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Py File", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "ToolBox", nullptr));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("MainWindow", "testing box", nullptr));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QCoreApplication::translate("MainWindow", "subtest", nullptr));
        treeWidget->setSortingEnabled(__sortingEnabled);

        QTreeWidgetItem *___qtreewidgetitem3 = treeWidget_2->headerItem();
        ___qtreewidgetitem3->setText(0, QCoreApplication::translate("MainWindow", "Projects", nullptr));
        btnRun->setText(QCoreApplication::translate("MainWindow", "RUN", nullptr));
        btnEngine->setText(QCoreApplication::translate("MainWindow", "Choose Engine", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
        menuCustom->setTitle(QCoreApplication::translate("MainWindow", "Custom", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
