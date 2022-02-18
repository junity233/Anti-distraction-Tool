#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

#ifdef _USRDLL

#define DLLEXPORT _declspec(dllexport) 

#else // DLL

#define DLLEXPORT _declspec(dllimport) 

#endif


#ifdef __cplusplus
extern "C" {
#endif

	DLLEXPORT BOOL InstallKeyboardHook();
	DLLEXPORT BOOL UninstallKeyboardHook();
	DLLEXPORT void SetExceptedKey(DWORD key,BOOL enable);
	DLLEXPORT void ResetExceptedKeys();

	DLLEXPORT BOOL InstallMouseHook();
	DLLEXPORT BOOL UninstallMouseHook();

	DLLEXPORT BOOL InstallProcPectHook();
	DLLEXPORT BOOL UnnstallProcPectHook();

	DLLEXPORT void SetPectProcID(DWORD pid);

#ifdef __cplusplus
}
#endif