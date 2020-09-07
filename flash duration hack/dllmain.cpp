// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

void MainThread(HMODULE handleToModule) {
    AllocConsole();

    uintptr_t module;
    uintptr_t player;

    module = (uintptr_t)GetModuleHandle(L"client.dll");

    player = *(uintptr_t*)(module + 0xD3FC5C);

    while (true) {
        if (GetAsyncKeyState(VK_END)) {
            break;
        }

        if (*(int*)(player + 0xA420) > 0) {
            *(int*)(player + 0xA420) = 0;
        }

        Sleep(1);
    }

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

