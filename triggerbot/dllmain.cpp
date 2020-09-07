// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <math.h>

struct vector {
    float x, y, z;
};

void MainThread(HMODULE handleToModule) {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    uintptr_t module;
    uintptr_t player;
    uintptr_t forceAttackAddress;
    uintptr_t crosshairIDAddress;
    int recoilReset;

    int playerTeam;

    module = (uintptr_t)GetModuleHandle(L"client.dll");

    forceAttackAddress = module + 0x3185984;

    player = *(uintptr_t*)(module + 0xD3FC5C);

    crosshairIDAddress = player + 0xB3E4;

    playerTeam = *(int*)(player + 0xF4);

    uintptr_t entityList = module + 0x4D5442C;

    while (true) {
        if (GetAsyncKeyState(VK_END)) {
            break;
        }

        int crosshairID = *(int*)(crosshairIDAddress);

        std::cout << crosshairID << std::endl;

        if (crosshairID && crosshairID <= 10) {
            uintptr_t ent = *(uintptr_t*)(entityList + (crosshairID - 1) * 0x10);

            if (*(int*)(ent + 0xF4) != playerTeam) {
                vector playerPos = *(vector*)(player + 0x138);
                vector enemyPos = *(vector*)(ent + 0x138);
                float distance = sqrt(pow((playerPos.x - enemyPos.x), 2) + pow((playerPos.y - enemyPos.y), 2) + pow((playerPos.z - enemyPos.z), 2));

                *(int*)(forceAttackAddress) = 5;
                Sleep(20);
                *(int*)(forceAttackAddress) = 4;
                Sleep(distance * 3.3);
            }
        }
    }

    fclose(f);
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

