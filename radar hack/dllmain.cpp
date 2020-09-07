// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>

void MainThread(HMODULE handleToModule) {
    AllocConsole();

    uintptr_t module;
    uintptr_t entityListAddress;

    module = (uintptr_t)GetModuleHandle(L"client.dll");

    entityListAddress = module + 0x4D5442C;

    while (true) {
        if (GetAsyncKeyState(VK_END)) {
            break;
        }

        for (int i = 0; i < 10; i++) {
            uintptr_t entityAddress = entityListAddress + i * 0x10;
            unsigned int entity = *(uintptr_t*)entityAddress;
            if (entity) {
                (*(bool*)(entity + 0x93D)) = true;
            }
        }
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
        if (handleToThread) {
            CloseHandle(handleToThread);
        }
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

