#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // 不在这里调用 show()，让 MainWindow 构造函数自己决定是否显示
    return a.exec();
}
