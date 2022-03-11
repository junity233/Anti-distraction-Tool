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
    /*������*/
    Ui::MainWindowClass ui;

    QTimer timer;
    time_t remainTime;//ʣ��ʱ�䣬��λms
    QStringList processNames;
    QSettings settings;
    HWND lockedWindow = NULL;

    QLabel* remainTimeLabel;

    bool isStart = false;

    /*˽�к���*/

    /**
     * @brief �����û�ָ���Ľ���
    */
    void killProcess();

    void Stop();
    void Start();

    void StoreSetting();
    void LoadSetting();

    /**
     * @brief ��MessageBox��һ����װ
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