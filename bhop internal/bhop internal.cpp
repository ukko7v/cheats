#include <Windows.h>

void MainThread(HMODULE hModule) {
	AllocConsole();

	uintptr_t module;
	uintptr_t player;
	uintptr_t flagAddress;
	uintptr_t forcejumpAddress;
	bool moving = false;

	module = (uintptr_t)GetModuleHandle("client.dll");// get a module

	player = *(uintptr_t*)(module + 0xD3FC5C);
	
	flagAddress = player + 0x104;
	forcejumpAddress = module + 0x51FE044;

	while (true) {
		if (GetAsyncKeyState(VK_END)) {
			break;
		}

		if ((*(float*)(player + 0x114) + *(float*)(player + 0x118) + *(float*)(player + 0x11C)) != 0)
			moving = true;
		else
			moving = false;

		if (GetAsyncKeyState(VK_SPACE) && *(BYTE*)flagAddress & (1 << 0) && moving) {
			*(uintptr_t*)forcejumpAddress = 6;
		}

		Sleep(1);
	}

	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE a = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
		if (a)
			CloseHandle(a);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

