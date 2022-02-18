# Anti-distraction-Tool
一个帮助你认真上课的工具。

# 使用方法

## 1.启动
 - 时间 : 持续的时间

 - 延迟 : 点击开始按钮后多久开始——让你有时间打开相关软件

 - 开始 : 点击开始！

## 2.设置

- 频率 : 计时器的频率（与进程拦截相关）

- 锁定鼠标 : 是否开启鼠标锁定

- 锁定键盘 : 是否开启键盘锁定

- 排除的键 : 键盘中仍然可以使用的键，使用逗号分割（比如你要暂停啥的）

  e.g: ``` "a,space,alt"```（无引号）

- 拦截进程 : 拦截指定进程

- 要拦截的进程名 : 如名，使用逗号分割

  e.g:``` "steam.exe,qq.exe"```（无引号）

- 进程的exit code : 杀进程时使用的exit code ~~（没多大用）~~

# 功能

1. 锁定鼠标/键盘 （Hook实现）
2. 进程防杀 （Hook实现）
3. 拦截进程

# 键盘按键名
对于A-Z和0-9，按键名就是它们自己（比如'a','7'等）
对于其他按键...看定义吧：
```cpp

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

```