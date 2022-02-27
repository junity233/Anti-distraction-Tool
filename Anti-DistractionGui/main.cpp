#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <qtranslator.h>
#include <signal.h>

#include "ProcessUtils.h"

static MainWindow* w = Q_NULLPTR;

//进程唯一
#pragma data_seg("Shared")
//已经打开的窗口句柄
volatile HWND hwndRect = 0;
#pragma data_seg()
#pragma comment(linker, "/Section:Shared,RWS")

//signal回调
static void terminateSignalHandle(int sign) {
    if(w)
        w->close();
}

static void checkIfProcessExist() {
    if (hwndRect != 0) {
        SetForegroundWindow(hwndRect);
        ShowWindow(hwndRect, SW_SHOWNORMAL);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, terminateSignalHandle);

    QApplication a(argc, argv);

    a.setWindowIcon(QIcon("icon.ico"));
    checkIfProcessExist();

    QTranslator t;
    t.load("zh_cn");
    a.installTranslator(&t);

    w = new MainWindow();
    w->show();

    hwndRect = (HWND)w->winId();

    return a.exec();
}
