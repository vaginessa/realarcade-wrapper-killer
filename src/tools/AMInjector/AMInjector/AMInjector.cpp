
// AMInjector (ActiveMark Instant Server Injection Tool) / esc0rtd3w 2016

// Original Source: http://www.rohitab.com/discuss/topic/39357-code-cave-injection-tutorial-c/


#include "stdafx.h"
#include <string>
#include <stdio.h>
#include <intrin.h>
#include <windows.h>
#include <istream>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <tlhelp32.h>

typedef BOOL(WINAPI *fnBeep)(DWORD, DWORD);
typedef int (WINAPI* MsgBoxParam)(HWND, LPCSTR, LPCSTR, UINT);

int injectStatus;

using namespace std;

struct PARAMETERS {
	DWORD MessageBoxInj;
	char text[50];
	char caption[25];
	int buttons;
	//		HWND handle;
};

DWORD getPid(string procName);
int privileges();
DWORD injectMsgBox(PARAMETERS * injectParams);
DWORD Useless();


DWORD getPid(string procName) {
	HANDLE hsnap;
	PROCESSENTRY32 pt;
	hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	do {
		if (pt.szExeFile == procName) {
			DWORD pid = pt.th32ProcessID;
			CloseHandle(hsnap);
			return pid;
		}
	} while (Process32Next(hsnap, &pt));
	CloseHandle(hsnap);
	//cout << "Where Am I?: getPid\n\n";
	//system("PAUSE");
	return 0;
}

DWORD injectMsgBox(PARAMETERS * injectParams) {
	MsgBoxParam MsgBox = (MsgBoxParam)injectParams->MessageBoxInj;
	int injectResult = MsgBox(0, injectParams->text, injectParams->caption, injectParams->buttons);
	switch (injectResult) {
	case IDOK:
		cout << "Where Am I?: IDOK\n\n";
		system("PAUSE");
		break;
	case IDCANCEL:
		cout << "Where Am I?: IDCANCEL\n\n";
		system("PAUSE");
		break;
	}
	return 0;
}

DWORD Useless() {
	return 0;
}

//this function is needed to get some extra privileges so your code will be able to work without conflicts with the system
int privileges() {
	HANDLE Token;
	TOKEN_PRIVILEGES tp; 
	
	//NTSTATUS NTAPI RtlAdjustPrivilege(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);  //This is the prototype of the RtlAdjustPrivilege function.
	//RtlAdjustPrivilege(20, TRUE, FALSE, &bl);  /*20 is the value of SeDebugPrivilege. The last parameter must point to a valid variable. It can't be NULL, or an access violation will occur and crash the calling process.*/

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &Token))
	{
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (AdjustTokenPrivileges(Token, 0, &tp, sizeof(tp), NULL, NULL) == 0) {
			return 1; //FAIL
		}
		else {
			return 0; //SUCCESS
		}
	}
	return 1;
}


HWND findWindow(string procName) {

	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 0);
	privileges();
	DWORD pid = 0;
	do {
		pid = getPid(procName);
		if (pid != 0) break;
	} while (1);

	return(0);
}


/*
void newProcessSelect(string process) {
	
	//

}
*/


int main()
{

	privileges();  //don't mind of the result, because maybe it fails because you already have that privilege

	// Process List
	string processList [] = {

		"notepad.exe", // Test EXE
		"aminstantservice.exe" // AM Instant Server EXE
	};

	// DLL List
	LPCSTR dllList[] = {

		"advapi32.dll",
		"crypt32.dll",
		"kernel32.dll",
		"netapi.dll",
		"shell32.dll",
		"user32.dll",
		"wininet.dll",
		"wintrust.dll",
		"ws2_32.dll",
		"wsock32.dll"
	};

	// Import List
	LPCSTR importList[] = {

		"MessageBoxA"
	};

	// AMI Server Versions
	string versionList[] = {

		"8.42.30.0",
		"8.43.37.0"
	};

	// Code Caves (Start Locations 2 Bytes From Initial NOP)
	// AMI Server Uses ASLR For Addresses
	string CodeCave[] = {
		
		"00EBCD5E", // 34 Bytes
		"00EBEA65", // 27 Bytes
		"00EC0DE5", // 27 Bytes
		"00EC64D9", // 39 Bytes
		"00EC7BE5", // 27 Bytes
		"00EC7DE5", // 27 Bytes
		"00EE40D7" // Small Cave (9 Bytes)
	};


	// Set Defaults From Arrays
	string process = processList[1];
	string version = versionList[1];

	LPCSTR dll = dllList[5];
	LPCSTR import = importList[0];


	//cout << "Enter process name:\n\n";
	//cin >> process;

	//findWindow(process);

	//int processNew;
	
	DWORD pid = getPid(process);
	if (pid == 0) {

		cout << "Process \"" << process << "\" Not Found!\n\n";
		system("PAUSE");

		return 1; //error

	}

	HANDLE p;
	p = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (p == NULL) {

		cout << "Opening Process \"" << process << "\" Failed!\n\n";
		system("PAUSE");

		return 1; //error

	}


	char* txtInject = "Test Injection Text";
	char* txtCaption = "Injection Results";

	PARAMETERS msgboxParams;	  //let's fill in a PARAMETERS struct

	msgboxParams.MessageBoxInj = (DWORD)GetProcAddress(GetModuleHandleA(dll), import);
	strcpy_s(msgboxParams.text, txtInject);
	strcpy_s(msgboxParams.caption, txtCaption);
	msgboxParams.buttons = MB_OKCANCEL | MB_ICONQUESTION;


	DWORD size_injectMsgBox = (PBYTE)Useless - (PBYTE)injectMsgBox;  //this gets injectMsgBox's size



																	 //--------now we are ready to inject


	void * injectMsgBoxAddress = VirtualAllocEx(p, NULL, size_injectMsgBox, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(p, injectMsgBoxAddress, (void*)injectMsgBox, size_injectMsgBox, NULL);

	void * DataAddress = VirtualAllocEx(p, NULL, sizeof(PARAMETERS), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(p, DataAddress, &msgboxParams, sizeof(PARAMETERS), NULL);

	HANDLE thread = CreateRemoteThread(p, NULL, 0, (LPTHREAD_START_ROUTINE)injectMsgBoxAddress, DataAddress, 0, NULL);


	// Injection Complete
	if (thread != 0) {

		injectStatus = 1;

		WaitForSingleObject(thread, INFINITE);   //this waits until thread thread has finished
		VirtualFree(injectMsgBoxAddress, 0, MEM_RELEASE); //free injectMsgBox memory
		VirtualFree(DataAddress, 0, MEM_RELEASE); //free data memory
		CloseHandle(thread);
		CloseHandle(p);  //don't wait for the thread to finish, just close the handle to the process
		
		system("CLS");
		cout << "----------------------------------" << endl;
		cout << "Injection Complete!" << endl;
		cout << "----------------------------------\n" << endl;
		cout << "----------------------------------" << endl;
		cout << "process: " << process << endl;
		cout << "pid: " << pid << endl;
		cout << "thread: " << thread << endl;
		cout << "----------------------------------" << endl;
		cout << "DataAddress: " << DataAddress << endl;
		cout << "injectMsgBoxAddress: " << injectMsgBoxAddress << endl;
		cout << "size_injectMsgBox: " << size_injectMsgBox << endl;
		cout << "----------------------------------" << endl;
		cout << "data.buttons: " << msgboxParams.buttons << endl;
		cout << "data.caption: " << msgboxParams.caption << endl;
		cout << "data.MessageBoxInj: " << msgboxParams.MessageBoxInj << endl;
		cout << "data.text: " << msgboxParams.text << endl;
		cout << "----------------------------------\n" << endl;
		system("PAUSE");
	}
	else {

		injectStatus = 0;

		system("CLS");
		cout << "----------------------------------" << endl;
		cout << "Error!" << endl;
		cout << "----------------------------------\n" << endl;
		cout << "----------------------------------" << endl;
		cout << "process: " << process << endl;
		cout << "pid: " << pid << endl;
		cout << "thread: " << thread << endl;
		cout << "----------------------------------" << endl;
		cout << "DataAddress: " << DataAddress << endl;
		cout << "injectMsgBoxAddress: " << injectMsgBoxAddress << endl;
		cout << "size_injectMsgBox: " << size_injectMsgBox << endl;
		cout << "----------------------------------" << endl;
		cout << "data.buttons: " << msgboxParams.buttons << endl;
		cout << "data.caption: " << msgboxParams.caption << endl;
		cout << "data.MessageBoxInj: " << msgboxParams.MessageBoxInj << endl;
		cout << "data.text: " << msgboxParams.text << endl;
		cout << "----------------------------------\n" << endl;

		system("PAUSE");
	}


	return EXIT_SUCCESS;



}

#pragma intrinsic(_ReturnAddress)

__declspec(noinline)
void noinline_func(void)
{
	printf("Return address from %s: %p\n", __FUNCTION__, _ReturnAddress());
}