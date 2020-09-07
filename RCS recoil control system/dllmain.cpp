#include <Windows.h>

#define exitKey VK_END

#define PlayerAddress 0xD3FC5C
#define ClientState 0x589DD4
#define ClientState_viewAngles 0x4D88
#define ShotsFired 0xA390
#define AimPunchAngle 0x302C

struct vec {
    float x, y, z;

    vec operator+(vec v) {
        return { x + v.x, y + v.y, z + v.z };
    }
    vec operator-(vec v) {
        return { x - v.x, y - v.y, z - v.z };
    }
    vec operator*(float f) {
        return { x * f, y * f, z * f };
    }

    void Normalize() {
        while (y < -180) y += 360;

        while (y > 180) y -= 360;

        if (x > 89) x = 89;

        if (x < -89) x = -89;
    }
};

int WINAPI HackThread(HMODULE handleToModule) {
    AllocConsole();

    uintptr_t clientModule = (uintptr_t)GetModuleHandle(L"client.dll");
    uintptr_t engineModule = (uintptr_t)GetModuleHandle(L"engine.dll");
    uintptr_t player = *(uintptr_t*)(clientModule + PlayerAddress);
    vec* viewAngles = (vec*)(*(uintptr_t*)(engineModule + ClientState) + ClientState_viewAngles);
    int* shotsFired = (int*)(player + ShotsFired);
    vec* aimPunchAngle = (vec*)(player + AimPunchAngle);

    vec oldPunch { 0,0,0 };

    while (!GetAsyncKeyState(exitKey)) {
        vec punchAngle = *aimPunchAngle * 2;

        if (*shotsFired > 1) {
            vec newAngle = *viewAngles + oldPunch - punchAngle;

            newAngle.Normalize();

            *viewAngles = newAngle;
        }

        oldPunch = punchAngle;
    }

    FreeConsole();
    FreeLibraryAndExitThread(handleToModule, 0);
    CloseHandle(handleToModule);
}

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
    return TRUE;
}

