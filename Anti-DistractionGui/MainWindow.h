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
    void selectWindowButtonClicked();

private:
    /*变量们*/
    Ui::MainWindowClass ui;

    QTimer timer;
    time_t remainTime;//剩余时间，单位ms
    QStringList processNames;
    QSettings settings;
    HWND lockedWindow = NULL;

    QLabel* remainTimeLabel;

    bool isStart = false;

    /*私有函数*/

    /**
     * @brief 结束用户指定的进程
    */
    void killProcess();

    void Stop();
    void Start();

    void StoreSetting();
    void LoadSetting();

    /**
     * @brief 对MessageBox的一个封装
     * @param msg 
    */
    void ShowMsgBox(QString msg);

    bool LockMouse();
    bool UnlockMouse();

    bool LockKeyboard();
    bool UnlockKeyboard();
    
    bool InstallHooks();
    bool UninstallHooks();

protected:
    void closeEvent(QCloseEvent* event);
};