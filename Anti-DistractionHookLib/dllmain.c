// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#include <TlHelp32.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

static HHOOK keyboardHook = NULL, mouseHook = NULL;

static HINSTANCE hins = NULL;

#pragma data_seg("shared")

static DWORD protectedProcessID = 0;
static WCHAR dllFullPath[MAX_PATH] = L"";

#pragma data_seg()
#pragma comment(linker, "/SECTION:shared,RWS")

static BOOL exceptedKeys[0xff];

static HANDLE (WINAPI *orginOpenProcess)(DWORD, BOOL, DWORD) = OpenProcess;

static BOOL (WINAPI *orginCreateProcessA)(
    LPCSTR                lpApplicationName,
    LPSTR                 lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCSTR                lpCurrentDirectory,
    LPSTARTUPINFOA        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
) = CreateProcessA;

static BOOL(WINAPI *orginCreateProcessW)(
    LPCWSTR               lpApplicationName,
    LPWSTR                lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCWSTR               lpCurrentDirectory,
    LPSTARTUPINFOW        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
    ) = CreateProcessW;


static BOOL InstallProcPectHookForProcess(HANDLE hProcess);
static BOOL UninstallProcPectHookForProcess(DWORD pid);
static BOOL HookAPI();
static BOOL UnhookAPI();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    BOOL res;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:        
        hins = hModule;
        res = HookAPI();

        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        UnhookAPI();
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

HANDLE WINAPI HookedOpenProcess(DWORD dwDesiredAccess,BOOL bInheritHandle,DWORD dwProcessId)
{
    if (dwProcessId != protectedProcessID)
        return (*orginOpenProcess)(dwDesiredAccess, bInheritHandle, dwProcessId);
    return (*orginOpenProcess)(dwDesiredAccess, bInheritHandle, GetCurrentProcessId());
}

BOOL WINAPI HookedCreateProcessW(
    LPCWSTR               lpApplicationName,
    LPWSTR                lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCWSTR               lpCurrentDirectory,
    LPSTARTUPINFOW        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
) {
    BOOL res;

    res = orginCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles,
        dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

    if (res != 0) {
        BOOL res;
        res = InstallProcPectHookForProcess(lpProcessInformation->hProcess);

#ifdef _DEBUG
        if (res == FALSE)
            MessageBox(NULL, L"Hook API failed!", L"Note", 0);
#endif _DEBUG
    }

#ifdef _DEBUG
    wchar_t buf[1024];

    wsprintf(buf, L"PectPID:%d\ncommand line:%ws", protectedProcessID, lpCommandLine);

    MessageBoxW(NULL, buf, lpApplicationName, 0);
#endif
    return res;
}

BOOL WINAPI HookedCreateProcessA(
    LPCSTR                lpApplicationName,
    LPSTR                 lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCSTR                lpCurrentDirectory,
    LPSTARTUPINFOA        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
) {
    BOOL res;

    res = orginCreateProcessA(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles,
        dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

    if (res != 0) {
        BOOL res;
        res = InstallProcPectHookForProcess(lpProcessInformation->hProcess);

#ifdef _DEBUG
        if (res == FALSE)
            MessageBox(NULL, L"Hook API failed!", L"Note", 0);
#endif
    }

#ifdef _DEBUG
    char buf[1024];

    sprintf_s(buf, 1024, "PectPID:%d\ncommand line:%s", protectedProcessID, lpCommandLine);

    MessageBoxA(NULL, buf, lpApplicationName, 0);
#endif
    return res;
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

static BOOL InstallProcPectHookForProcess(HANDLE hProcess) {

    LPVOID loadLibraryAddr = DetourFindFunction("kernel32.dll", "LoadLibraryW");
    LPVOID llPraram = VirtualAllocEx(hProcess, NULL, wcslen(dllFullPath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (llPraram == NULL) {
        FreeLibrary(loadLibraryAddr);
        return FALSE;
    }


    WriteProcessMemory(hProcess, llPraram, dllFullPath, wcslen(dllFullPath) * 2 + 2, NULL);

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, llPraram, 0, NULL);
    if (hThread) {
        ResumeThread(hThread);
        
        WaitForSingleObject(hThread, 100);

        CloseHandle(hThread);
        return TRUE;
    }
    return FALSE;
}

BOOL InstallProcPectHook()
{
    WCHAR dirPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, dirPath);

    wsprintfW(dllFullPath, L"%ls\\hook.dll", dirPath);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return FALSE;

    PROCESSENTRY32 process;
    process.dwSize = sizeof(process);

    BOOL more = Process32First(hSnapshot, &process);

    while (more) {
        HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
            PROCESS_VM_WRITE|PROCESS_VM_READ, FALSE, process.th32ProcessID);
        
        if (hProcess) {
            InstallProcPectHookForProcess(hProcess);
            
            CloseHandle(hProcess);
        }
        
        more = Process32Next(hSnapshot, &process);
    }

    CloseHandle(hSnapshot);
    return TRUE;
}

static BOOL UninstallProcPectHookForProcess(DWORD pid) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

    if (hSnapshot == INVALID_HANDLE_VALUE)
        return FALSE;
    
    MODULEENTRY32 module;
    module.dwSize = sizeof(module);

    BOOL more = Module32First(hSnapshot, &module);

    while (more) {
        if (lstrcmpW(module.szModule, L"hook.dll") == 0) {
            HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
                PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);

            LPVOID freeLibrary = DetourFindFunction("kernel32.dll", "FreeLibrary");
            LPVOID llPraram = VirtualAllocEx(hProcess, NULL, sizeof(LPVOID), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            

            HANDLE hThread = CreateRemoteThread(
                hProcess,
                NULL, 
                0, 
                (LPTHREAD_START_ROUTINE)freeLibrary, 
                module.modBaseAddr, 
                0, 
                NULL
            );

            if (hThread) {

                WaitForSingleObject(hThread, 100);
                CloseHandle(hThread);
            }

            CloseHandle(hProcess);
            break;
        }

        more = Module32Next(hSnapshot, &module);
    }
    CloseHandle(hSnapshot);

    return TRUE;
}

BOOL UnnstallProcPectHook()
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return FALSE;

    PROCESSENTRY32 process;
    process.dwSize = sizeof(process);

    BOOL more = Process32First(hSnapshot, &process);

    while (more) {
        UninstallProcPectHookForProcess(process.th32ProcessID);

        more = Process32Next(hSnapshot, &process);
    }
    return FALSE;
}

void SetPectProcID(DWORD pid)
{
    protectedProcessID = pid;
}


BOOL HookAPI()
{
    LONG res = NO_ERROR;

    orginOpenProcess = DetourFindFunction("kernel32.dll", "OpenProcess");
    orginCreateProcessA = DetourFindFunction("kernel32.dll", "CreateProcessA");
    orginCreateProcessW = DetourFindFunction("kernel32.dll", "CreateProcessW");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    res |= DetourAttach((PVOID)&orginOpenProcess, (PVOID)HookedOpenProcess);
    res |= DetourAttach((PVOID)&orginCreateProcessA, (PVOID)HookedCreateProcessA);
    res |= DetourAttach((PVOID)&orginCreateProcessW, (PVOID)HookedCreateProcessW);

    if (res != NO_ERROR)
        MessageBox(NULL, L"Hook api failed", L"Note", 0);
    
    DetourTransactionCommit();
    return TRUE;
}

BOOL UnhookAPI()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourDetach((PVOID)&orginOpenProcess, (PVOID)HookedOpenProcess);
    DetourDetach((PVOID)&orginCreateProcessA, (PVOID)HookedCreateProcessA);
    DetourDetach((PVOID)&orginCreateProcessW, (PVOID)HookedCreateProcessW);

    DetourTransactionCommit();
    return TRUE;
}

void SetExceptedKey(DWORD key,BOOL enable)
{
    exceptedKeys[key] = enable;
}

void ResetExceptedKeys() {
    memset(exceptedKeys, 0, sizeof exceptedKeys);
}