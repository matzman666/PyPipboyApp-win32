// Win32Project1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PyPipBoyApp-Launcher.h"
#include <Windows.h>
#include <atlstr.h>
#include <shellapi.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance

BOOL PathRemoveFileSpecNew(LPTSTR path)
{
	LPTSTR newPath = PathFindFileName(path);
	if ((newPath) && (newPath != path)) {
		newPath[0] = TEXT('\0');
		return 1;
	}
	return 0;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	BOOL usePythonW = true;

	// Parse command line arguments
	LPTSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);

	if (szArgList) {
		for (int i = 0; i < argCount; i++) {
			if (!_tcscmp(L"--debug", szArgList[i])) {
				usePythonW = false;
			}
		}

		LocalFree(szArgList);
	}

	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR exeFullPath[MAX_PATH];
	GetModuleFileNameW(hModule, exeFullPath, MAX_PATH);
	PathRemoveFileSpecNew(exeFullPath);

	if (!PathFileExists(L"python34")){
		MessageBox(NULL, L"Can't find python34 subdirectory", L"Error", MB_OK);
	}

	CString CSnewpath = "";
	CSnewpath.Append(exeFullPath);
	CSnewpath.Append(L"python34;");
	CSnewpath.Append(exeFullPath);
	CSnewpath.Append(L"python34\\Lib\\site-packages\\pywin32_system32;");

	SetEnvironmentVariableW(L"path", CSnewpath);

	if (!PathFileExists(L"pypipboyapp")){
		MessageBox(NULL, L"Can't find pypipboyapp subdirectory", L"Error", MB_OK);
	}

	CString CSpipdir = "";
	CSpipdir.Append(exeFullPath);
	CSpipdir.Append(L"pypipboyapp");
	SetCurrentDirectoryW(CSpipdir);

	CString CSExe = "python.exe";
	if (usePythonW)
	{
		CSExe = "pythonw.exe";
	}

	CString CSparams = "pypipboyapp.py";
	if (usePythonW)
	{
		CSparams = CSparams + " --stdlog ../pypipboyapp.log";
	}
	CString CScommand = CSExe + " " + CSparams;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		CScommand.GetBuffer(),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		WCHAR msg[100];
		swprintf(msg, 100, L"CreateProcess failed with error: %d", GetLastError());
		MessageBox(NULL, msg, L"Error", MB_OK);
		return 1;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}




