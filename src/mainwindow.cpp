#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "appmanager.h"
#include <QStackedWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // 设置窗口属性
    setWindowTitle("Talkbox ");
    setMinimumSize(800, 600);
    resize(1000, 700);
    
    // 创建堆叠窗口作为中央窗口
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);
    
    // 创建应用程序管理器
    m_appManager = new AppManager(m_stackedWidget, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
