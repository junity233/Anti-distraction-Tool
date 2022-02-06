#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

#define DLLEXPORT _declspec(dllexport) 

#ifdef __cplusplus
extern "C" {
#endif

	DLLEXPORT BOOL InstallKeyboardHook();
	DLLEXPORT BOOL UninstallKeyboardHook();
	DLLEXPORT void SetExceptedKey(DWORD key,BOOL enable);
	DLLEXPORT void ResetExceptedKeys();

	DLLEXPORT BOOL InstallMouseHook();
	DLLEXPORT BOOL UninstallMouseHook();

#ifdef __cplusplus
}
#endif