#include "MainWindow.h"
#include <qevent.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include "framework.h"
#include "ProcessUtils.h"

static QMap<QString, DWORD> vkcodeMap{
    {"f1",VK_F1},
    {"f2",VK_F2},
    {"f3",VK_F3},
    {"f4",VK_F4},
    {"f5",VK_F5},
    {"f6",VK_F6},
    {"f7",VK_F7},
    {"f8",VK_F8},
    {"f9",VK_F9},
    {"f10",VK_F10},
    {"f11",VK_F11},
    {"f12",VK_F12},
    {"f13",VK_F13},
    {"f14",VK_F14},
    {"f15",VK_F15},
    {"f16",VK_F16},
    {"f17",VK_F17},
    {"f18",VK_F18},
    {"f19",VK_F19},
    {"f20",VK_F20},
    {"f21",VK_F21},
    {"f22",VK_F22},
    {"f23",VK_F23},
    {"f24",VK_F24},

    {"delete",VK_DELETE},
    {"esc",VK_ESCAPE},
    {"tab",VK_TAB},
    {"print",VK_PRINT},
    {"backspace",VK_BACK},
    {"capslook",VK_CAPITAL},
    {"shift",VK_SHIFT},
    {"space",VK_SPACE},
    {"enter",VK_RETURN},
    {"alt",VK_MENU},
    {"numlock",VK_NUMLOCK},
    {"snapshot",VK_SNAPSHOT},

    {"up",VK_UP},
    {"down",VK_DOWN},
    {"left",VK_LEFT},
    {"right",VK_RIGHT},

    {"num0",VK_NUMPAD0},
    {"num1",VK_NUMPAD1},
    {"num2",VK_NUMPAD2},
    {"num3",VK_NUMPAD3},
    {"num4",VK_NUMPAD4},
    {"num5",VK_NUMPAD5},
    {"num6",VK_NUMPAD6},
    {"num7",VK_NUMPAD7},
    {"num8",VK_NUMPAD8},
    {"num9",VK_NUMPAD9},

    {"+",VK_ADD},
    {"-",VK_SUBTRACT},
    {"*",VK_MULTIPLY},
    {"/",VK_DIVIDE},

    {"oem;",VK_OEM_1},
    {"oem=",VK_OEM_PLUS},
    {"oem-",VK_OEM_MINUS},
    {"oem/",VK_OEM_2},
    {"oem`",VK_OEM_3},
    {"oem[",VK_OEM_4},
    {"oem\\",VK_OEM_5},
    {"oem]",VK_OEM_6},
    {"oem'",VK_OEM_7},
    {"oem,",VK_OEM_COMMA},
    {"oem.",VK_OEM_PERIOD},

};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    timer(this),
    settings("config.ini",QSettings::IniFormat)
{
    ui.setupUi(this);
    LoadSetting();

    connect(&timer, &QTimer::timeout, this, &MainWindow::timerTimeout);


    connect(ui.killTaskCheckBox, &QCheckBox::stateChanged, [this](int state) {
        ui.taskNameTextEdit->setEnabled(state);
        ui.exitCodeSpinBox->setEnabled(state);
        });

    ui.taskNameTextEdit->setEnabled(ui.killTaskCheckBox->isChecked());
    ui.exitCodeSpinBox->setEnabled(ui.killTaskCheckBox->isChecked());

    connect(ui.lockKeyboardCheckBox, &QCheckBox::stateChanged, [this](int state) {
        ui.excludedKeysLineEdit->setEnabled(state);
        });
    ui.excludedKeysLineEdit->setEnabled(ui.lockKeyboardCheckBox->isChecked());
}

void MainWindow::timerTimeout()
{

    if (ui.killTaskCheckBox->isChecked()) {
        ProcessUtils processTool;
        processTool.Init();
        ProcessUtils::Process process = processTool.GetCurrentProcess();
        while (process) {
            QString name = QString::fromWCharArray(process->szExeFile);
            if (processNames.contains(name.toLower())) {
                ProcessUtils::KillProcess(process, ui.exitCodeSpinBox->value());
            }
            process = processTool.GetNextProcess();
        }
    }
    //Debug下可使用delete键退出，release下删除
#ifdef _DEBUG
    if (GetKeyState(VK_DELETE) & 0x8000)
        this->Stop();
#endif
   

    if (lastTime >= ui.lastTimeEdit->time()) {
        this->Stop();
    }
}

void MainWindow::Stop()
{
    timer.stop();
    
    UninstallHooks();

    isStart = false;
    ui.centralWidget->setEnabled(true);
}

void MainWindow::Start()
{
    processNames = ui.taskNameTextEdit->toPlainText().split(',');
    for (int i = 0; i < processNames.length(); i++) {
        processNames[i] = processNames[i].toLower();
    }

    timer.setInterval(1000 / ui.frequencySpinBox->value());

    lastTime.restart();

    InstallHooks();

    isStart = true;
    timer.start();
}

void MainWindow::startButtonClicked() {
    StoreSetting();
    ui.centralWidget->setEnabled(false);

    QTime time;
    time.start();

    while (time.elapsed() < (ui.delaySpinBox->value() * 1000)) {
        QCoreApplication::processEvents();
    }

    Start();
}

void MainWindow::StoreSetting()
{
    settings.setValue("last_time", ui.lastTimeEdit->time().toString("hh:mm:ss"));
    settings.setValue("delay", ui.delaySpinBox->value());
    settings.setValue("frequency", ui.frequencySpinBox->value());
    settings.setValue("lock_mouse", ui.lockMouseCheckBox->isChecked());
    settings.setValue("lock_keyboard", ui.lockKeyboardCheckBox->isChecked());
    settings.setValue("excluded_keys", ui.excludedKeysLineEdit->text());
    settings.setValue("kill_task", ui.killTaskCheckBox->isChecked());
    settings.setValue("task_name", ui.taskNameTextEdit->toPlainText());
}

void MainWindow::LoadSetting()
{
    ui.lastTimeEdit->setTime(QTime::fromString(settings.value("last_time", "00:40:00").toString(), "hh:mm:ss"));
    ui.delaySpinBox->setValue(settings.value("delay", 10).toInt());
    ui.frequencySpinBox->setValue(settings.value("frequency", 10).toInt());
    ui.lockMouseCheckBox->setChecked(settings.value("lock_mouse", true).toBool());
    ui.lockKeyboardCheckBox->setChecked(settings.value("lock_keyboard", true).toBool());
    ui.excludedKeysLineEdit->setText(settings.value("excluded_keys").toString());
    ui.killTaskCheckBox->setChecked(settings.value("kill_task", true).toBool());
    ui.taskNameTextEdit->setText(settings.value("task_name").toString());

    ui.encourageText->setText(settings.value("slogan", "KEEP FINGHTING!").toString());
    ui.encourageText->setFont(QFont(settings.value("font", "Arial Black").toString(),20, -1, true));
}

void MainWindow::ShowMsgBox(QString msg)
{
    QMessageBox::warning(
        this,
        tr("Note"),
        msg
    );
}

void MainWindow::LockMouse(){
    if (!InstallMouseHook())
        ShowMsgBox("Install hook for mouse failed!");
}

void MainWindow::UnlockMouse()
{
    if (!UninstallMouseHook())
        ShowMsgBox("Uninstall hook for mouse failed!");
}

void MainWindow::LockKeyboard()
{

    ResetExceptedKeys();

    QStringList keys = ui.excludedKeysLineEdit->text().split(',');

    for (auto i : keys) {
        if (i.size() == 1 && i[0].isLetterOrNumber())
            SetExceptedKey(i[0].toUpper().toLatin1(), true);
        else if (vkcodeMap.contains(i))
            SetExceptedKey(vkcodeMap[i], true);
        else {
            ShowMsgBox(tr("Unknown key:%1").arg(i));
        }
    }

    if (InstallMouseHook())
        ShowMsgBox("Install hook for keyboard failed!");
}

void MainWindow::UnlockKeyboard()
{
    if (!UninstallKeyboardHook())
        ShowMsgBox("Load library failed!");
}

void MainWindow::InstallHooks()
{
    if (ui.lockMouseCheckBox->isChecked())
        LockMouse();

    if (ui.lockKeyboardCheckBox->isChecked())
        LockKeyboard();

    SetPectProcID(GetCurrentProcessId());
    InstallProcPectHook();
}

void MainWindow::UninstallHooks()
{
    if (ui.lockKeyboardCheckBox->isChecked())
        UnlockKeyboard();
    if (ui.lockMouseCheckBox->isChecked())
        UnlockMouse();

    SetPectProcID(NULL);
    UnnstallProcPectHook();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    //开始后拒绝关闭
    if (isStart) {
        event->ignore();
    }
    StoreSetting();
}
