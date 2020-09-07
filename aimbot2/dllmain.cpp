#include <Windows.h>
#include <iostream>
#include "loop.h"

void MainThread(HMODULE handleToModule) {
    AllocConsole();

    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    Run();
    
    fclose((FILE*)stdout);
    FreeConsole();
    FreeLibraryAndExitThread(handleToModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE handleToThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
        if (handleToThread)
            CloseHandle(handleToThread);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void UpdatePlayerPointers() {

}