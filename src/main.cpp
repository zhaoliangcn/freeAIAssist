#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "winsock2.h"
#include <windows.h>
#endif
#include <QtNetwork/QSslSocket>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    if (QSslSocket::supportsSsl()) {
        qDebug() << "SSL is supported!";
        std::cerr << "SSL is supported!" << std::endl;
    } else {
        qDebug() << "SSL is NOT supported!";
        std::cerr << "SSL is NOT supported!" << std::endl;
    }

#ifdef _WIN32
    //隐藏一个控制台窗口，使得在之后用popen来启shell窗口的时候，不显示黑窗口，或者避免黑窗口一闪而过的情况
    AllocConsole();    //为调用进程分配一个新的控制台
    ShowWindow(GetConsoleWindow(), SW_HIDE);    //隐藏自己创建的控制台
#endif 
    // 设置应用程序图标
    QIcon appIcon(":/icons/aiassist.png"); // 替换为实际的图标文件名
    a.setWindowIcon(appIcon);

    MainWindow w;
    w.show();
    return a.exec();
}
