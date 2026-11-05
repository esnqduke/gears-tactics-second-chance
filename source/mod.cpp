#define WIN32_LEAN_AND_MEAN
#include "mod_core.h"

#include <cstdio>
#include <cstring>
#include <cwchar>

static DbnoRuntime runtime;
static HMODULE thisModule = nullptr;


static void Log(const char* message) {
    wchar_t path[MAX_PATH]{};
    const DWORD lengthRoot=GetEnvironmentVariableW(L"LOCALAPPDATA",path,MAX_PATH);
    if(!lengthRoot || lengthRoot>=MAX_PATH) return;
    if(wcscat_s(path,L"\\GearsTacticsInsaneDBNO")!=0) return;
    if(!CreateDirectoryW(path,nullptr) && GetLastError()!=ERROR_ALREADY_EXISTS) return;
    if(wcscat_s(path,L"\\InsaneDBNO.log")!=0) return;
    HANDLE file = CreateFileW(path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return;
    SYSTEMTIME time{};
    GetLocalTime(&time);
    char line[512]{};
    int length = sprintf_s(line, "%04u-%02u-%02u %02u:%02u:%02u %s\r\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, message);
    if (length > 0) { DWORD written = 0; WriteFile(file, line, static_cast<DWORD>(length), &written, nullptr); }
    CloseHandle(file);
}

static bool SupportedBuild(uintptr_t base) {
    __try {
        auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
        if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
        auto nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
        return nt->Signature == IMAGE_NT_SIGNATURE && nt->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 &&
            nt->FileHeader.TimeDateStamp == 0x6036bd2c && nt->OptionalHeader.SizeOfImage == 0xb57aa00;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}


static DWORD WINAPI Start(void*) {
    HMODULE pinned=nullptr;
    if(!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN,reinterpret_cast<LPCWSTR>(thisModule),&pinned)) return 0;
    runtime.base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!SupportedBuild(runtime.base)) { Log("UNSUPPORTED BUILD: data worker disabled; XInput forwarding active."); return 0; }
    Log("READY: Insane DBNO 0.2.2 LATE LOAD. Waiting for Insane mission. No code hooks. Target FatigueSteps=2.");
    for(;;) {
        const unsigned changed=runtime.tick();
        if(changed) {char message[160]{};sprintf_s(message,"APPLIED: FatigueSteps=2; updated %u data fields in active Insane mission.",changed);Log(message);}
        Sleep(1000);
    }
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        thisModule = module;
        wchar_t host[MAX_PATH]{};
        GetModuleFileNameW(nullptr, host, MAX_PATH);
        const wchar_t* name = wcsrchr(host, L'\\');
        if (!name || _wcsicmp(name + 1, L"GearsTactics.exe") != 0) return TRUE;
        HANDLE thread = CreateThread(nullptr, 0, Start, nullptr, 0, nullptr);
        if (thread) CloseHandle(thread);
    }
    return TRUE;
}
