#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <cstdint>
#include <cwchar>
inline uintptr_t RemoteModule(DWORD pid,const wchar_t* name,bool fullPath=false) {
 HANDLE snapshot=INVALID_HANDLE_VALUE;
 for(unsigned attempt=0;attempt<8;++attempt) {
  snapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pid);
  if(snapshot!=INVALID_HANDLE_VALUE || GetLastError()!=ERROR_BAD_LENGTH)break;
  Sleep(10);
 }
 if(snapshot==INVALID_HANDLE_VALUE)return 0;
 MODULEENTRY32W entry{};entry.dwSize=sizeof entry;uintptr_t result=0;
 for(BOOL ok=Module32FirstW(snapshot,&entry);ok;ok=Module32NextW(snapshot,&entry)) {
  if(_wcsicmp(fullPath?entry.szExePath:entry.szModule,name)==0){result=reinterpret_cast<uintptr_t>(entry.modBaseAddr);break;}
 }
 CloseHandle(snapshot);return result;
}
inline bool LoadDataDll(HANDLE process,DWORD pid,const wchar_t* fullPath) {
 if(RemoteModule(pid,fullPath,true))return true;
 FARPROC local=GetProcAddress(GetModuleHandleW(L"kernel32.dll"),"LoadLibraryW");HMODULE owner=nullptr;
 if(!local || !GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,reinterpret_cast<LPCWSTR>(local),&owner))return false;
 wchar_t ownerPath[MAX_PATH]{};if(!GetModuleFileNameW(owner,ownerPath,MAX_PATH))return false;
 const wchar_t* name=wcsrchr(ownerPath,L'\\');if(!name)return false;
 const uintptr_t remoteOwner=RemoteModule(pid,name+1);if(!remoteOwner)return false;
 const uintptr_t address=remoteOwner+reinterpret_cast<uintptr_t>(local)-reinterpret_cast<uintptr_t>(owner);
 const SIZE_T size=(wcslen(fullPath)+1)*sizeof(wchar_t);
 void* path=VirtualAllocEx(process,nullptr,size,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);if(!path)return false;
 SIZE_T written=0;
 if(!WriteProcessMemory(process,path,fullPath,size,&written) || written!=size){VirtualFreeEx(process,path,0,MEM_RELEASE);return false;}
 HANDLE thread=CreateRemoteThread(process,nullptr,0,reinterpret_cast<LPTHREAD_START_ROUTINE>(address),path,0,nullptr);
 if(!thread){VirtualFreeEx(process,path,0,MEM_RELEASE);return false;}
 const DWORD wait=WaitForSingleObject(thread,10000);CloseHandle(thread);
 // On timeout the loader may still use this path. Process exit reclaims it.
 if(wait!=WAIT_OBJECT_0){SetLastError(ERROR_TIMEOUT);return false;}
 VirtualFreeEx(process,path,0,MEM_RELEASE);
 return RemoteModule(pid,fullPath,true)!=0;
}
