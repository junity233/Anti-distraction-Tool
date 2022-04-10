#include "MainWindow.h"
#include <qevent.h>
#include <qdebug.h>
#include <qmessagebox.h>

#include "framework.h"
#include "ProcessUtils.h"
#include "SelectWindowDialog.h"

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

static QString GetWindowsLastErrorMsg()
{
    LPWSTR lpMsgBuf = NULL;
    QString res;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_SYSTEM_DEFAULT, SUBLANG_SYS_DEFAULT),
        (LPWSTR)&lpMsgBuf,
        0,
        NULL
    );

    res = QString::fromWCharArray(lpMsgBuf);
    LocalFree(lpMsgBuf);

    return res;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    timer(this),
    settings("config.ini", QSettings::IniFormat),
    browser(this)
{
    ui.setupUi(this);
    remainTimeLabel = new QLabel(this);

    ui.statusBar->addWidget(remainTimeLabel);

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


    connect(ui.lockWindowCheckBox, &QCheckBox::stateChanged, [this](int state) {
        ui.windowTitleLineEdit->setEnabled(state);
        });
    ui.windowTitleLineEdit->setEnabled(ui.lockWindowCheckBox->isChecked());
}

void MainWindow::killProcess()
{
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


void MainWindow::timerTimeout()
{

    if (ui.killTaskCheckBox->isChecked())
        killProcess();

    //Debug下可使用delete键退出，release下删除
#ifdef _DEBUG
    if (GetKeyState(VK_DELETE) & 0x8000)
        this->Stop();
#endif

    if (ui.lockWindowCheckBox->isChecked()&&lockedWindow!=NULL) {
        if (!IsZoomed(lockedWindow)) {
            SetForegroundWindow(lockedWindow);
            ShowWindow(lockedWindow, SW_MAXIMIZE);
            //SetWindowPos(lockedWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        }

    }


    if (remainTime <= 0)
        this->Stop();
    else
        remainTime -= 1000 / ui.frequencySpinBox->value();

    remainTimeLabel->setText(tr("Remain time:%1 s").arg(remainTime / 1000));
}
void MainWindow::selectWindowButtonClicked()
{
    lockedWindow = SelectWindowDialog::SelectWindow(this);
    ui.windowTitleLineEdit->setText(
        lockedWindow == NULL ?
        tr("No Window Selected") :
        SelectWindowDialog::GetWindowTitle(lockedWindow)
    );

}
void MainWindow::Stop()
{
    timer.stop();

    UninstallHooks();

    if (ui.focusBrowserCheckBox->isChecked()) {
        browser.hide();
    }

    isStart = false;
    ui.centralWidget->setEnabled(true);

    remainTimeLabel->clear();
}

void MainWindow::Start()
{
    processNames = ui.taskNameTextEdit->toPlainText().split(',');
    for (int i = 0; i < processNames.length(); i++) {
        processNames[i] = processNames[i].toLower();
    }

    timer.setInterval(1000 / ui.frequencySpinBox->value());

    QTime lastTime = ui.lastTimeEdit->time();
    remainTime = (lastTime.hour() * 3600 + lastTime.minute() * 60 + lastTime.second()) * 1000;


    if (!InstallHooks())
        return;

    if (ui.focusBrowserCheckBox->isChecked())
    {
        browser.setAllowedDomains(ui.allowedDomainsEdit->text());
        browser.setUrl(ui.startUrlEdit->text());
#ifdef _DEBUG
        browser.show();
#else
        browser.showFullScreen();
#endif
    }

    ui.centralWidget->setEnabled(false);
    isStart = true;
    timer.start();
}

void MainWindow::startButtonClicked() {
    StoreSetting();

    /*先等待一段时间*/
    QTime time;
    time.start();

    while (time.elapsed() < (ui.delaySpinBox->value() * 1000))
        QCoreApplication::processEvents();

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
    settings.setValue("lock_window", ui.lockWindowCheckBox->isChecked());
    settings.setValue("task_name", ui.taskNameTextEdit->toPlainText());
    settings.setValue("enable_focus_browser", ui.focusBrowserCheckBox->isChecked());
    settings.setValue("focus_browser_allowed_domain", ui.allowedDomainsEdit->text());
    settings.setValue("focus_browser_start_url", ui.startUrlEdit->text());
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
    ui.lockWindowCheckBox->setChecked(settings.value("lock_window", true).toBool());
    ui.taskNameTextEdit->setText(settings.value("task_name").toString());
    ui.focusBrowserCheckBox->setChecked(settings.value("enable_focus_browser").toBool());
    ui.allowedDomainsEdit->setText(settings.value("focus_browser_allowed_domain").toString());
    ui.startUrlEdit->setText(settings.value("focus_browser_start_url").toString());

    ui.encourageText->setText(settings.value("slogan", "KEEP FINGHTING!").toString());
    ui.encourageText->setFont(QFont(settings.value("font", "Arial Black").toString(), 20, -1, true));
}

void MainWindow::ShowMsgBox(QString msg)
{
    QMessageBox::warning(
        this,
        tr("Note"),
        msg
    );
}

bool MainWindow::LockMouse() {
    if (!InstallMouseHook()) {
        ShowMsgBox(tr("Install hook for mouse failed:%1").arg(GetWindowsLastErrorMsg()));
        return false;
    }
    return true;
}

bool MainWindow::UnlockMouse()
{
    if (!UninstallMouseHook()) {
        ShowMsgBox(tr("Unnstall hook for mouse failed:%1").arg(GetWindowsLastErrorMsg()));
        return false;
    }
    return true;
}

bool MainWindow::LockKeyboard()
{

    ResetExceptedKeys();
    QString keysStr = ui.excludedKeysLineEdit->text();
    QStringList keys = keysStr.split(',');

    if (!keysStr.isEmpty()) {
        for (auto i : keys) {
            if (i.size() == 1 && i[0].isLetterOrNumber())
                SetExceptedKey(i[0].toUpper().toLatin1(), true);
            else if (vkcodeMap.contains(i))
                SetExceptedKey(vkcodeMap[i], true);
            else {
                ShowMsgBox(tr("Unknown key:%1").arg(i));
                return false;
            }
        }
    }

    if (!InstallKeyboardHook()) {
        ShowMsgBox(tr("Install hook for keyboard failed:%1").arg(GetWindowsLastErrorMsg()));
        return false;
    }

    return true;
}

bool MainWindow::UnlockKeyboard()
{
    if (!UninstallKeyboardHook()) {
        ShowMsgBox(tr("Uninstall hook for keyboard failed:%1").arg(GetWindowsLastErrorMsg()));
        return false;
    }
    return true;
}

bool MainWindow::InstallHooks()
{
    bool success = true;
    if (ui.lockMouseCheckBox->isChecked())
        if (!LockMouse())
            return false;

    if (ui.lockKeyboardCheckBox->isChecked())
        if (!LockKeyboard())
            return false;

    SetPectProcID(GetCurrentProcessId());

    if (!InstallProcPectHook()) {
        ShowMsgBox(tr("Install process protecting hook failed:%1").arg(GetWindowsLastErrorMsg()));
        return false;
    }

    return  true;
}

bool MainWindow::UninstallHooks()
{
    if (ui.lockKeyboardCheckBox->isChecked())
        if (!UnlockKeyboard())
            return false;
    if (ui.lockMouseCheckBox->isChecked())
        if (!UnlockMouse())
            return false;
    
    SetPectProcID(NULL);
    if (!UnnstallProcPectHook()) {
        ShowMsgBox(tr("Uninstall process protecting hook failed:%1").arg(GetWindowsLastErrorMsg()));
        return false;
    }

    return true;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    //开始后拒绝关闭
    if (isStart) {
        event->ignore();
    }
    StoreSetting();
}