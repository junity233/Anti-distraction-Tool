#pragma once
#include <Windows.h>
#include <TlHelp32.h>

class ProcessUtils
{
public:
	~ProcessUtils();

	bool Init();

	typedef PROCESSENTRY32* Process;

	Process GetCurrentProcess() {
		return currentProcess;
	}

	Process GetNextProcess();

	static bool KillProcess(Process process,UINT exitCode);

private:
	HANDLE hSnapshot = INVALID_HANDLE_VALUE;
	Process currentProcess = NULL;
	bool more = true;
};

