#include <iostream>
#include <Windows.h>

int main()
{
	// Start a new process
	STARTUPINFO si = {};
	si.cb = sizeof(si);
	si.dwX = 0;
	si.dwY = 0;
	si.dwXSize = 250;
	si.dwYSize = 250;
	si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;

	PROCESS_INFORMATION pi;
	LPCTSTR szCmdline = TEXT("C:\\Windows\\System32\\notepad.exe");
	if (CreateProcess((LPTSTR)szCmdline, NULL, NULL, NULL, false, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		// Somme app don't read and use STARTUPINFO (like notepad.exe) so let's set position
		WaitForInputIdle(pi.hProcess, INFINITE);
		// Set window position
		EnumWindows([](HWND hwnd, LPARAM lParam)->BOOL
			{
				DWORD pid;
				GetWindowThreadProcessId(hwnd, &pid);
				if (pid == lParam)
				{
					SetWindowPos(hwnd, HWND_TOP, 0, 0, 250, 250, 0);
					return FALSE;
				}
				return TRUE;
			}, pi.dwProcessId);

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
		std::cout << "CreateProcess failed: " << GetLastError() << std::endl;
}
