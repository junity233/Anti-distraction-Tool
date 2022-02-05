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
    QTime endTime;
    QStringList processNames;
    QLibrary hookLib;
    QSettings settings;

    bool isStart=false;

    void Stop();
    void Start();

    void WriteSetting();
    void LoadSetting();

    void ShowMsgBox(QString msg);

    void InstallMouseHook();
    void UninstallMouseHook();

    void InstallKeyboardHook();
    void UninstallKeyboardHook();

    void InstallHooks();
    void UninstallHooks();
protected:
    void closeEvent(QCloseEvent* event);
};
