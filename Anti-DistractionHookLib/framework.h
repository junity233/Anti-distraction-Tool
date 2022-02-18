﻿#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

#ifdef _USRDLL

#define DLLEXPORT _declspec(dllexport) 

#else // DLL

#define DLLEXPORT _declspec(dllimport) 

#endif

//dll文件名
#define HOOK_DLL_NAME L"hook.dll"


#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief 安装键盘钩子
	 * @return 
	*/
	DLLEXPORT BOOL InstallKeyboardHook();
	/**
	 * @brief 卸载键盘钩子
	 * @return 
	*/
	DLLEXPORT BOOL UninstallKeyboardHook();
	/**
	 * @brief 设置一个要排除的键
	 * @param key 
	 * @param enable 
	 * @return 
	*/
	DLLEXPORT void SetExceptedKey(DWORD key,BOOL enable);
	/**
	 * @brief 重置要排除的键（全不排除）
	 * @return 
	*/
	DLLEXPORT void ResetExceptedKeys();

	/**
	 * @brief 安装鼠标钩子
	 * @return 
	*/
	DLLEXPORT BOOL InstallMouseHook();
	/**
	 * @brief 卸载鼠标钩子
	 * @return 
	*/
	DLLEXPORT BOOL UninstallMouseHook();

	/**
	 * @brief 安装进程防杀
	 * @return 
	*/
	DLLEXPORT BOOL InstallProcPectHook();
	/**
	 * @brief 卸载进程防杀
	 * @return 
	*/
	DLLEXPORT BOOL UnnstallProcPectHook();
	/**
	 * @brief 设置主进程的PID
	 * @param pid 
	 * @return 
	*/
	DLLEXPORT void SetPectProcID(DWORD pid);

#ifdef __cplusplus
}
#endif