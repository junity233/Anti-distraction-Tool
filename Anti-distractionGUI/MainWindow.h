#pragma once

#include <QtWidgets/QMainWindow>
#include <qlibrary.h>
#include <qtimer.h>
#include <qsettings.h>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private slots:
    void startButtonClicked();
    void timerTimeout();

private:
    Ui::MainWindowClass ui;

    QTimer timer;
    QTime lastTime;//记录已经开始了多久
    QStringList processNames;
    QSettings settings;

    bool isStart=false;

    void Stop();
    void Start();

    void StoreSetting();
    void LoadSetting();

    void ShowMsgBox(QString msg);

    void LockMouse();
    void UnlockMouse();

    void LockKeyboard();
    void UnlockKeyboard();

    void InstallHooks();
    void UninstallHooks();

protected:
    void closeEvent(QCloseEvent* event);
};
