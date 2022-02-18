#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <qtranslator.h>
#include <signal.h>

static MainWindow* w=nullptr;

//signal»Øµ÷
static void terminateSignalHandle(int sign) {
    if(w)
        w->close();
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, terminateSignalHandle);

    QApplication a(argc, argv);

    QTranslator t;
    t.load("zh_cn");

    a.installTranslator(&t);

    w = new MainWindow();
    w->show();

    return a.exec();
}
