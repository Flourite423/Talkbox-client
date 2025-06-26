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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLogin;
    QAction *actionLogout;
    QAction *actionExit;
    QAction *actionPreferences;
    QAction *actionFullscreen;
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *centralLayout;
    QTabWidget *mainTabWidget;
    QWidget *chatTab;
    QWidget *forumTab;
    QWidget *settingsTab;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionLogin = new QAction(MainWindow);
        actionLogin->setObjectName("actionLogin");
        actionLogout = new QAction(MainWindow);
        actionLogout->setObjectName("actionLogout");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName("actionPreferences");
        actionFullscreen = new QAction(MainWindow);
        actionFullscreen->setObjectName("actionFullscreen");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QVBoxLayout(centralwidget);
        centralLayout->setObjectName("centralLayout");
        mainTabWidget = new QTabWidget(centralwidget);
        mainTabWidget->setObjectName("mainTabWidget");
        chatTab = new QWidget();
        chatTab->setObjectName("chatTab");
        mainTabWidget->addTab(chatTab, QString());
        forumTab = new QWidget();
        forumTab->setObjectName("forumTab");
        mainTabWidget->addTab(forumTab, QString());
        settingsTab = new QWidget();
        settingsTab->setObjectName("settingsTab");
        mainTabWidget->addTab(settingsTab, QString());

        centralLayout->addWidget(mainTabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 31));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionLogin);
        menuFile->addAction(actionLogout);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionPreferences);
        menuView->addAction(actionFullscreen);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        mainTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Talkbox", nullptr));
        MainWindow->setStyleSheet(QCoreApplication::translate("MainWindow", "QMainWindow {\n"
"    background-color: #f5f5f5;\n"
"    font-family: 'Microsoft YaHei', Arial, sans-serif;\n"
"}\n"
"\n"
"QMenuBar {\n"
"    background-color: #007acc;\n"
"    color: white;\n"
"    border: none;\n"
"}\n"
"\n"
"QMenuBar::item {\n"
"    background: transparent;\n"
"    padding: 8px 16px;\n"
"}\n"
"\n"
"QMenuBar::item:selected {\n"
"    background-color: #005999;\n"
"}\n"
"\n"
"QStatusBar {\n"
"    background-color: #333;\n"
"    color: white;\n"
"}", nullptr));
        actionLogin->setText(QCoreApplication::translate("MainWindow", "\347\231\273\345\275\225", nullptr));
        actionLogout->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272\347\231\273\345\275\225", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
        actionPreferences->setText(QCoreApplication::translate("MainWindow", "\351\246\226\351\200\211\351\241\271", nullptr));
        actionFullscreen->setText(QCoreApplication::translate("MainWindow", "\345\205\250\345\261\217", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
        mainTabWidget->setStyleSheet(QCoreApplication::translate("MainWindow", "QTabWidget::pane {\n"
"    border: 1px solid #ddd;\n"
"    background-color: white;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: #e9ecef;\n"
"    padding: 8px 16px;\n"
"    margin-right: 2px;\n"
"    border-top-left-radius: 4px;\n"
"    border-top-right-radius: 4px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: #007acc;\n"
"    color: white;\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background-color: #0056b3;\n"
"    color: white;\n"
"}", nullptr));
        mainTabWidget->setTabText(mainTabWidget->indexOf(chatTab), QCoreApplication::translate("MainWindow", "\350\201\212\345\244\251", nullptr));
        mainTabWidget->setTabText(mainTabWidget->indexOf(forumTab), QCoreApplication::translate("MainWindow", "\350\256\272\345\235\233", nullptr));
        mainTabWidget->setTabText(mainTabWidget->indexOf(settingsTab), QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "\350\247\206\345\233\276", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
