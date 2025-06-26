/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLogout;
    QAction *actionAbout;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *chatTab;
    QHBoxLayout *chatLayout;
    QWidget *contactWidget;
    QWidget *chatWidget;
    QWidget *groupTab;
    QHBoxLayout *groupLayout;
    QWidget *groupManagerWidget;
    QWidget *groupChatWidget;
    QWidget *forumTab;
    QHBoxLayout *forumLayout;
    QWidget *forumListWidget;
    QWidget *forumDetailWidget;
    QMenuBar *menubar;
    QMenu *menuUser;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QLabel *userLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
        actionLogout = new QAction(MainWindow);
        actionLogout->setObjectName("actionLogout");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        chatTab = new QWidget();
        chatTab->setObjectName("chatTab");
        chatLayout = new QHBoxLayout(chatTab);
        chatLayout->setObjectName("chatLayout");
        contactWidget = new QWidget(chatTab);
        contactWidget->setObjectName("contactWidget");
        contactWidget->setMaximumSize(QSize(300, 16777215));

        chatLayout->addWidget(contactWidget);

        chatWidget = new QWidget(chatTab);
        chatWidget->setObjectName("chatWidget");

        chatLayout->addWidget(chatWidget);

        tabWidget->addTab(chatTab, QString());
        groupTab = new QWidget();
        groupTab->setObjectName("groupTab");
        groupLayout = new QHBoxLayout(groupTab);
        groupLayout->setObjectName("groupLayout");
        groupManagerWidget = new QWidget(groupTab);
        groupManagerWidget->setObjectName("groupManagerWidget");
        groupManagerWidget->setMaximumSize(QSize(400, 16777215));

        groupLayout->addWidget(groupManagerWidget);

        groupChatWidget = new QWidget(groupTab);
        groupChatWidget->setObjectName("groupChatWidget");

        groupLayout->addWidget(groupChatWidget);

        tabWidget->addTab(groupTab, QString());
        forumTab = new QWidget();
        forumTab->setObjectName("forumTab");
        forumLayout = new QHBoxLayout(forumTab);
        forumLayout->setObjectName("forumLayout");
        forumListWidget = new QWidget(forumTab);
        forumListWidget->setObjectName("forumListWidget");
        forumListWidget->setMaximumSize(QSize(400, 16777215));

        forumLayout->addWidget(forumListWidget);

        forumDetailWidget = new QWidget(forumTab);
        forumDetailWidget->setObjectName("forumDetailWidget");

        forumLayout->addWidget(forumDetailWidget);

        tabWidget->addTab(forumTab, QString());

        horizontalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 31));
        menuUser = new QMenu(menubar);
        menuUser->setObjectName("menuUser");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        userLabel = new QLabel(statusbar);
        userLabel->setObjectName("userLabel");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuUser->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuUser->addAction(actionLogout);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Talkbox - \350\201\212\345\244\251\350\275\257\344\273\266", nullptr));
        actionLogout->setText(QCoreApplication::translate("MainWindow", "\347\231\273\345\207\272", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(chatTab), QCoreApplication::translate("MainWindow", "\350\201\212\345\244\251", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(groupTab), QCoreApplication::translate("MainWindow", "\347\276\244\347\273\204", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(forumTab), QCoreApplication::translate("MainWindow", "\350\256\272\345\235\233", nullptr));
        menuUser->setTitle(QCoreApplication::translate("MainWindow", "\347\224\250\346\210\267", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
        userLabel->setText(QCoreApplication::translate("MainWindow", "\346\234\252\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
