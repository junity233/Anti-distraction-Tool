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
    MainWindow(QWidget* parent = Q_NULLPTR);

private slots:
    void startButtonClicked();
    void timerTimeout();

private:
    Ui::MainWindowClass ui;

    QTimer timer;
    time_t remainTime;//剩余时间，单位ms
    QStringList processNames;
    QSettings settings;

    QLabel* remainTimeLabel;

    bool isStart = false;

    /**
     * @brief 结束用户指定的进程
    */
    void killProcess();

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