#include "ProcessUtils.h"

ProcessUtils::~ProcessUtils() {
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		CloseHandle(hSnapshot);
	}
}

bool ProcessUtils::Init() {
	if (this->hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);
	this->hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return false;
	}
	
	this->currentProcess = (Process)malloc(sizeof(PROCESSENTRY32));
	if (currentProcess == NULL) {
		CloseHandle(hSnapshot);
		return false;
	}

	currentProcess->dwSize = sizeof(PROCESSENTRY32);

	more = Process32First(hSnapshot, currentProcess);
	return true;
}

ProcessUtils::Process ProcessUtils::GetNextProcess() {
	if (!more) {
		CloseHandle(hSnapshot);
		hSnapshot = INVALID_HANDLE_VALUE;
		return nullptr;
	}
	more = Process32Next(hSnapshot, currentProcess);
	return currentProcess;
}

bool ProcessUtils::KillProcess(Process process,UINT exitCode) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process->th32ProcessID);

	if (hProcess == INVALID_HANDLE_VALUE) {
		return false;
	}
	
	bool res = TerminateProcess(hProcess, exitCode);
	CloseHandle(hProcess);

	return res;
}