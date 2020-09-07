// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

struct _Target {
    typedef void(__thiscall* _Func)(void* pThis);
    _Func Func;

    void* pThis = (void*)0xc3b138;
};

DWORD WINAPI MainThread(LPVOID param) {
    uintptr_t base = (uintptr_t)GetModuleHandle(0);

    while (!GetAsyncKeyState(VK_END)) {
        if (GetAsyncKeyState(VK_NUMPAD3)) {
        }
    }
    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

