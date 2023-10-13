#include <iostream>
#include <algorithm>
#include <string>
#include <tchar.h>
#include <windows.h>
#include <tlhelp32.h>

using namespace std;

int getPID(wstring procName)
{
	transform(procName.begin(), procName.end(), procName.begin(), ::tolower);
	int pid = -1;
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(pid);
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return(pid);
	}

	do
	{
		wstring strProc(pe32.szExeFile);
		transform(strProc.begin(), strProc.end(), strProc.begin(), ::tolower);
		if (strProc == procName)
		{
			return pe32.th32ProcessID;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return(pid);
}

BOOL TerminateProcess(int pid)
{
	WORD dwDesiredAccess = PROCESS_TERMINATE;
	BOOL bInheritHandle = FALSE;
	HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, pid);
	if (hProcess == NULL)
		return FALSE;
	BOOL result = TerminateProcess(hProcess, 1);
	CloseHandle(hProcess);
	return(TRUE);
}

BOOL TerminateProcess(wstring procName)
{
	int pid = getPID(procName);
	return TerminateProcess(pid);
}

int main(int argc, const char **argv)
{
	if (argc > 1) {
		size_t size = strlen(argv[1]);
		wstring ws(argv[1], &argv[1][size]);
		TerminateProcess(ws);
	}
	else {
		std::cout << "Usage: taskkill <process name.exe>" << endl;
	}
	return 0;
}