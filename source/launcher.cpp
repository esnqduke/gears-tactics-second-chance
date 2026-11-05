#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <string>
#include <cstdio>
#include "late_load.h"
static void Report(const wchar_t* message,bool error=false) {
 wchar_t path[MAX_PATH]{};
 const DWORD length=GetEnvironmentVariableW(L"LOCALAPPDATA",path,MAX_PATH);
 if(length && length<MAX_PATH && wcscat_s(path,L"\\GearsTacticsInsaneDBNO")==0) {
  CreateDirectoryW(path,nullptr);
  if(wcscat_s(path,L"\\Launcher.log")==0) {
   FILE* file=nullptr;
   if(_wfopen_s(&file,path,L"a, ccs=UTF-8")==0 && file){SYSTEMTIME t{};GetLocalTime(&t);fwprintf(file,L"%04u-%02u-%02u %02u:%02u:%02u %ls\n",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,message);fclose(file);}
  }
 }
 if(error)MessageBoxW(nullptr,message,L"Gears Tactics DBNO",MB_OK|MB_ICONERROR);
}
static bool SupportedExe(const wchar_t* path) {
 FILE* file=nullptr;if(_wfopen_s(&file,path,L"rb")!=0 || !file)return false;
 IMAGE_DOS_HEADER dos{};IMAGE_NT_HEADERS64 nt{};
 const bool ok=fread(&dos,sizeof dos,1,file)==1 && dos.e_magic==IMAGE_DOS_SIGNATURE && dos.e_lfanew>0 && dos.e_lfanew<1048576 &&
  fseek(file,dos.e_lfanew,SEEK_SET)==0 && fread(&nt,sizeof nt,1,file)==1 && nt.Signature==IMAGE_NT_SIGNATURE &&
  nt.FileHeader.Machine==IMAGE_FILE_MACHINE_AMD64 && nt.FileHeader.TimeDateStamp==0x6036bd2c && nt.OptionalHeader.SizeOfImage==0xb57aa00;
 fclose(file);return ok;
}
int WINAPI wWinMain(HINSTANCE,HINSTANCE,PWSTR command,int) {
 int argc=0;wchar_t** argv=CommandLineToArgvW(command,&argc);
 if(!argv || argc<1){if(argv)LocalFree(argv);Report(L"Missing original game command. Steam launch options must end with %command%.",true);return 1;}
 std::wstring game=argv[0];LocalFree(argv);
 const wchar_t* leaf=wcsrchr(game.c_str(),L'\\');
 if(!leaf || _wcsicmp(leaf+1,L"GearsTactics.exe") || !SupportedExe(game.c_str())){Report(L"Unsupported game executable. No DLL loaded.",true);return 2;}
 wchar_t dll[MAX_PATH]{};
 if(!GetModuleFileNameW(nullptr,dll,MAX_PATH))return 3;
 wchar_t* slash=wcsrchr(dll,L'\\');if(!slash)return 3;
 if(wcscpy_s(slash+1,MAX_PATH-static_cast<size_t>(slash+1-dll),L"InsaneDBNO.dll")!=0 || GetFileAttributesW(dll)==INVALID_FILE_ATTRIBUTES){Report(L"InsaneDBNO.dll is missing next to the launcher.",true);return 3;}
 std::wstring originalCommand=command;
 STARTUPINFOW startup{};startup.cb=sizeof startup;PROCESS_INFORMATION info{};
 Report(L"Starting original GearsTactics.exe; waiting for startup before loading the data DLL.");
 if(!CreateProcessW(game.c_str(),originalCommand.data(),nullptr,nullptr,FALSE,0,nullptr,nullptr,&startup,&info)){Report(L"Could not start the original game.",true);return 4;}
 CloseHandle(info.hThread);
 if(WaitForSingleObject(info.hProcess,30000)!=WAIT_TIMEOUT){CloseHandle(info.hProcess);Report(L"Game exited before the DLL was loaded. No gameplay changes were applied.",true);return 5;}
 const bool loaded=LoadDataDll(info.hProcess,info.dwProcessId,dll);
 const DWORD error=GetLastError();CloseHandle(info.hProcess);
 if(!loaded){wchar_t status[100]{};swprintf_s(status,L"DLL loading was not confirmed. Local diagnostic status: %lu.",error);Report(status);Report(L"Data DLL was not confirmed in the running game. See Launcher.log. The original game remains running.",true);return 6;}
 Report(L"Data DLL loaded successfully. Launcher exiting; DLL handles future missions automatically.");return 0;
}
