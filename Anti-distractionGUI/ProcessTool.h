#pragma once
#include <Windows.h>
#include <TlHelp32.h>

class ProcessTool
{
public:
	ProcessTool() { hSnapshot = INVALID_HANDLE_VALUE; };
	~ProcessTool();

	bool Init();

	typedef PROCESSENTRY32* Process;

	Process GetCurrentProcess() {
		return currentProcess;
	}

	Process GetNextProcess();

	static bool KillProcess(Process process,UINT exitCode);

private:
	HANDLE hSnapshot;
	Process currentProcess;
	bool more = true;
};

