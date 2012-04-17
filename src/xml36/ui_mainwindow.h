/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Apr 17 18:29:14 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionNew;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionAddAttribute;
    QAction *actionRemoveAttribute;
    QAction *actionEdit;
    QAction *actionExit;
    QAction *actionAddNode;
    QAction *actionRemoveNode;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTreeWidget *xml_tree;
    QTreeWidget *attributes_list;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menuXML;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(842, 535);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/xml.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setWindowIcon(icon);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon2);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon3);
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        actionSaveAs->setEnabled(false);
        actionAddAttribute = new QAction(MainWindow);
        actionAddAttribute->setObjectName(QString::fromUtf8("actionAddAttribute"));
        actionAddAttribute->setEnabled(false);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/attribute_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAddAttribute->setIcon(icon4);
        actionRemoveAttribute = new QAction(MainWindow);
        actionRemoveAttribute->setObjectName(QString::fromUtf8("actionRemoveAttribute"));
        actionRemoveAttribute->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/attribute_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemoveAttribute->setIcon(icon5);
        actionEdit = new QAction(MainWindow);
        actionEdit->setObjectName(QString::fromUtf8("actionEdit"));
        actionEdit->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/pencil.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEdit->setIcon(icon6);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAddNode = new QAction(MainWindow);
        actionAddNode->setObjectName(QString::fromUtf8("actionAddNode"));
        actionAddNode->setEnabled(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/node_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAddNode->setIcon(icon7);
        actionRemoveNode = new QAction(MainWindow);
        actionRemoveNode->setObjectName(QString::fromUtf8("actionRemoveNode"));
        actionRemoveNode->setEnabled(false);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/node_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemoveNode->setIcon(icon8);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(3);
        horizontalLayout->setContentsMargins(3, 3, 3, 3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        xml_tree = new QTreeWidget(splitter);
        xml_tree->setObjectName(QString::fromUtf8("xml_tree"));
        xml_tree->setProperty("showDropIndicator", QVariant(false));
        xml_tree->setAllColumnsShowFocus(true);
        splitter->addWidget(xml_tree);
        attributes_list = new QTreeWidget(splitter);
        attributes_list->setObjectName(QString::fromUtf8("attributes_list"));
        splitter->addWidget(attributes_list);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 842, 25));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menuXML = new QMenu(menuBar);
        menuXML->setObjectName(QString::fromUtf8("menuXML"));
        menuXML->setTearOffEnabled(true);
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setEnabled(true);
        mainToolBar->setIconSize(QSize(16, 16));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menuXML->menuAction());
        menu->addAction(actionNew);
        menu->addAction(actionOpen);
        menu->addAction(actionSave);
        menu->addAction(actionSaveAs);
        menu->addSeparator();
        menu->addAction(actionExit);
        menuXML->addAction(actionAddNode);
        menuXML->addAction(actionRemoveNode);
        menuXML->addAction(actionAddAttribute);
        menuXML->addAction(actionRemoveAttribute);
        menuXML->addAction(actionEdit);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionAddNode);
        mainToolBar->addAction(actionRemoveNode);
        mainToolBar->addAction(actionAddAttribute);
        mainToolBar->addAction(actionRemoveAttribute);
        mainToolBar->addAction(actionEdit);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "xml36", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214 XML \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave->setText(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 XML \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSaveAs->setText(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\260\320\272 ...", 0, QApplication::UnicodeUTF8));
        actionAddAttribute->setText(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\260\321\202\321\200\320\270\320\261\321\203\321\202", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAddAttribute->setToolTip(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\260\321\202\321\200\320\270\320\261\321\203\321\202", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRemoveAttribute->setText(QApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\260\321\202\321\200\320\270\320\261\321\203\321\202", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRemoveAttribute->setToolTip(QApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\260\321\202\321\200\320\270\320\261\321\203\321\202", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionEdit->setText(QApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionEdit->setToolTip(QApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionAddNode->setText(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\203\320\267\320\265\320\273", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAddNode->setToolTip(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\203\320\267\320\265\320\273", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRemoveNode->setText(QApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \321\203\320\267\320\265\320\273", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRemoveNode->setToolTip(QApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \321\203\320\267\320\265\320\273", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        QTreeWidgetItem *___qtreewidgetitem = xml_tree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "\320\243\320\267\320\265\320\273", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = attributes_list->headerItem();
        ___qtreewidgetitem1->setText(1, QApplication::translate("MainWindow", "\320\227\320\275\320\260\321\207\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindow", "\320\220\321\202\321\200\320\270\320\261\321\203\321\202", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        menuXML->setTitle(QApplication::translate("MainWindow", "XML", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
