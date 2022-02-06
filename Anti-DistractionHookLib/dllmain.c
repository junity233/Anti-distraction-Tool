// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

static HHOOK keyboardHook = NULL, mouseHook = NULL;
static HINSTANCE hins;

static BOOL exceptedKeys[0xff];

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:        
        hins = hModule;
        ResetExceptedKeys();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        hins = NULL;
        break;
    }
    return TRUE;
}

LRESULT CALLBACK KeyboardHookHandle(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0) {
        
        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }
    DWORD vkCode = ((LPKBDLLHOOKSTRUCT)lParam)->vkCode;

    //debug下排除delete键
#ifdef _DEBUG
    if (vkCode == VK_DELETE)
        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
#endif

    if (exceptedKeys[vkCode]) {
        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }
    return 1;
}


LRESULT CALLBACK MouseHookHandle(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) {

        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }
    return 1;
}

BOOL  InstallKeyboardHook() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookHandle, hins, 0);
    if (keyboardHook != NULL) {
        return TRUE;
    }
    return FALSE;
}

BOOL UninstallKeyboardHook() {
    if (keyboardHook != NULL) {
        return UnhookWindowsHookEx(keyboardHook);
    }
    return TRUE;
}

BOOL InstallMouseHook() {
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookHandle, hins, 0);
    if (mouseHook != NULL) {
        return TRUE;
    }
    return FALSE;
}

BOOL UninstallMouseHook() {
    if (mouseHook != NULL) {
        return UnhookWindowsHookEx(mouseHook);
    }
    return TRUE;
}

void SetExceptedKey(DWORD key,BOOL enable)
{
    exceptedKeys[key] = enable;
}

void ResetExceptedKeys() {
    memset(exceptedKeys, 0, sizeof exceptedKeys);
}

