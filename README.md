# Anti-distraction-Tool
һ�������������ϿεĹ��ߡ�

# ʹ�÷���

## 1.����
 - ʱ�� : ������ʱ��

 - �ӳ� : �����ʼ��ť���ÿ�ʼ����������ʱ���������

 - ��ʼ : �����ʼ��

## 2.����

- Ƶ�� : ��ʱ����Ƶ�ʣ������������أ�

- ������� : �Ƿ����������

- �������� : �Ƿ�����������

- �ų��ļ� : ��������Ȼ����ʹ�õļ���ʹ�ö��ŷָ������Ҫ��ͣɶ�ģ�

  e.g: ``` "a,space,alt"```�������ţ�

- ���ؽ��� : ����ָ������

- Ҫ���صĽ����� : ������ʹ�ö��ŷָ�

  e.g:``` "steam.exe,qq.exe"```�������ţ�

- ���̵�exit code : ɱ����ʱʹ�õ�exit code ~~��û����ã�~~

# ����

1. �������/���� ��Hookʵ�֣�
2. ���̷�ɱ ��Hookʵ�֣�
3. ���ؽ���

# ���̰�����
����A-Z��0-9�����������������Լ�������'a','7'�ȣ�
������������...������ɣ�
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