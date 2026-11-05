#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include "../late_load.h"
int wmain(int argc,wchar_t** argv) {
 if(argc>1 && wcscmp(argv[1],L"--child")==0){Sleep(20000);return 0;}
 wchar_t host[MAX_PATH]{},dll[MAX_PATH]{};GetModuleFileNameW(nullptr,host,MAX_PATH);
 wcscpy_s(dll,host);wchar_t* slash=wcsrchr(dll,L'\\');if(!slash)return 1;wcscpy_s(slash+1,MAX_PATH-static_cast<size_t>(slash+1-dll),L"InsaneDBNO.dll");
 wchar_t command[MAX_PATH+32]{};swprintf_s(command,L"\"%ls\" --child",host);
 STARTUPINFOW startup{};startup.cb=sizeof startup;PROCESS_INFORMATION info{};
 if(!CreateProcessW(host,command,nullptr,nullptr,FALSE,CREATE_NO_WINDOW,nullptr,nullptr,&startup,&info))return 2;
 CloseHandle(info.hThread);Sleep(200);
 const bool first=LoadDataDll(info.hProcess,info.dwProcessId,dll);
 const bool second=first && LoadDataDll(info.hProcess,info.dwProcessId,dll);
 const DWORD error=GetLastError();
 TerminateProcess(info.hProcess,0);WaitForSingleObject(info.hProcess,5000);CloseHandle(info.hProcess);
 if(!first || !second){std::printf("FAIL: load test error %lu\n",error);return 3;}
 std::puts("PASS: real late DLL loading and idempotence in isolated child; no game modified.");return 0;
}
