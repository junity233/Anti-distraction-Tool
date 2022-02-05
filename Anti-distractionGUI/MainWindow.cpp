#include "MainWindow.h"
#include <qevent.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include "ProcessTool.h"

typedef BOOL(*DllFunc)();
typedef void (*SetExceptedKeyFunc) (DWORD, BOOL);

static QTime operator + (const QTime& a, const QTime& b) {
    QTime res;
    int s = a.second() + b.second();
    int m = a.minute() + b.minute();
    int h = a.hour() + b.hour();
    int ms = a.msec() + b.msec();

    if (ms >= 1000)
        s += 1, ms -= 1000;
    if (s >= 60)
        m += 1, s -= 60;
    if (m >= 60)
        h += 1, m -= 60;

    res.setHMS(h, m, s, ms);
    return res;
}

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
    hookLib("HookLib"),
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
        ui.exceptedKeyLineEdit->setEnabled(state);
        });
    ui.exceptedKeyLineEdit->setEnabled(ui.lockKeyboardCheckBox->isChecked());

}

void MainWindow::timerTimeout()
{

    if (ui.killTaskCheckBox->isChecked()) {
        ProcessTool processTool;
        processTool.Init();
        ProcessTool::Process process = processTool.GetCurrentProcess();
        while (process) {
            QString name = QString::fromWCharArray(process->szExeFile);
            if (processNames.contains(name.toLower())) {
                ProcessTool::KillProcess(process, ui.exitCodeSpinBox->value());
            }
            process = processTool.GetNextProcess();
        }
    }

#ifdef _DEBUG
    if (GetKeyState(VK_DELETE) & 0x8000)
        this->Stop();
#endif
   

    if (endTime < QTime::currentTime()) {
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
    endTime = QTime::currentTime() + ui.lastTimeEdit->time();

    InstallHooks();

    isStart = true;
    timer.start();
}

void MainWindow::startButtonClicked() {
    WriteSetting();
    ui.centralWidget->setEnabled(false);

    QTime time;
    time.start();

    while (time.elapsed() < (ui.delaySpinBox->value() * 1000)) {
        QCoreApplication::processEvents();
    }

    Start();
}

void MainWindow::WriteSetting()
{
    settings.setValue("last_time", ui.lastTimeEdit->time().toString("hh:mm:ss"));
    settings.setValue("delay", ui.delaySpinBox->value());
    settings.setValue("frequency", ui.frequencySpinBox->value());
    settings.setValue("lockMouse", ui.lockMouseCheckBox->isChecked());
    settings.setValue("lockKeyboard", ui.lockKeyboardCheckBox->isChecked());
    settings.setValue("keysExcepted", ui.exceptedKeyLineEdit->text());
    settings.setValue("killTask", ui.killTaskCheckBox->isChecked());
    settings.setValue("taskNames", ui.taskNameTextEdit->toPlainText());
}

void MainWindow::LoadSetting()
{
    ui.lastTimeEdit->setTime(QTime::fromString(settings.value("last_time", "00:40:00").toString(), "hh:mm:ss"));
    ui.delaySpinBox->setValue(settings.value("delay", 10).toInt());
    ui.frequencySpinBox->setValue(settings.value("frequency", 10).toInt());
    ui.lockMouseCheckBox->setChecked(settings.value("lockMouse", true).toBool());
    ui.lockKeyboardCheckBox->setChecked(settings.value("lockKeyboard", true).toBool());
    ui.exceptedKeyLineEdit->setText(settings.value("keysExcepted").toString());
    ui.killTaskCheckBox->setChecked(settings.value("killTask", true).toBool());
    ui.taskNameTextEdit->setText(settings.value("taskNames").toString());

    ui.encourageText->setText(settings.value("encourageText", "KEEP FINGHTING!").toString());
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

void MainWindow::InstallMouseHook()
{
    DllFunc installMouseHookFunc = (DllFunc)hookLib.resolve("InstallMouseHook");
    if (installMouseHookFunc) {
        if (!installMouseHookFunc())
            ShowMsgBox("Install hook for mouse failed!");
    }
    else ShowMsgBox("Load library failed!");
}

void MainWindow::UninstallMouseHook()
{
    DllFunc uninstallMouseHookFunc = (DllFunc)hookLib.resolve("UninstallMouseHook");
    if (uninstallMouseHookFunc)
        uninstallMouseHookFunc();
    else ShowMsgBox("Load library failed!");
}

void MainWindow::InstallKeyboardHook()
{
    DllFunc installHookFunc = (DllFunc)hookLib.resolve("InstallKeyboardHook");
    SetExceptedKeyFunc setExceptedKeyFunc = (SetExceptedKeyFunc)hookLib.resolve("SetExceptedKey");
    QFunctionPointer resetExceptedKeysFunc = hookLib.resolve("ResetExceptedKeys");

    if (installHookFunc) {
        resetExceptedKeysFunc();

        QStringList keys = ui.exceptedKeyLineEdit->text().split(',');

        for (auto i : keys) {
            if (i.size() == 1 && i[0].isLetterOrNumber())
                setExceptedKeyFunc(i[0].toUpper().toLatin1(), true);
            else if (vkcodeMap.contains(i))
                setExceptedKeyFunc(vkcodeMap[i], true);
            else {
                ShowMsgBox(tr("Unknown key:%1").arg(i));
            }
        }

        if (!installHookFunc())
            ShowMsgBox("Install hook for keyboard failed!");
    }
    else ShowMsgBox("Load library failed!");
}

void MainWindow::UninstallKeyboardHook()
{
    DllFunc uninstallHookFunc = (DllFunc)hookLib.resolve("UninstallKeyboardHook");
    if (uninstallHookFunc)
        uninstallHookFunc();
    else ShowMsgBox("Load library failed!");
}

void MainWindow::InstallHooks()
{
    if (ui.lockKeyboardCheckBox->isChecked())
        InstallKeyboardHook();
    if (ui.lockMouseCheckBox->isChecked())
        InstallMouseHook();
}

void MainWindow::UninstallHooks()
{
    if (ui.lockKeyboardCheckBox->isChecked())
        UninstallKeyboardHook();
    if (ui.lockMouseCheckBox->isChecked())
        UninstallMouseHook();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (isStart) {
        event->ignore();
    }
}
