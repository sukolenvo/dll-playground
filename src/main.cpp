#include <windows.h>
#include <iostream>

#include <detours.h>

#include "hook_hero_selected.hpp"
#include "hook_d3d9.hpp"
#include "main.hpp"

bool createConsole()
{
    if (AllocConsole() == 0)
    {
        std::cout << "failed to allocate console" << GetLastError() << std::endl;
        return false;
    }
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    std::cout << "Console created" << std::endl;
    return true;
}

DWORD WINAPI run()
{
    if (!setup_d3d9_hook()) {
        std::cout << "Failed to setup d3d9 hook" << std::endl;
        return -1;
    }
    if (!setup_hero_select_hook())
    {
        std::cout << "Failed to setup hero select hook" << std::endl;
        return -1;
    }
    return 0;
}

HANDLE thread = nullptr;

BOOL DllMain(HANDLE handle, DWORD dwReason, LPVOID)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "library loaded" << std::endl;
        thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)run, NULL, 0, NULL);
        if (thread == nullptr)
        {
            std::cout << "Failed to launch thread " << GetLastError() << std::endl;
            return false;
        }

        break;
    }
    return true;
}

VOID CALLBACK MyDetourFinishHelperProcess(HWND hwnd, HINSTANCE hinst, LPSTR str, INT int_param)
{
    // https://github.com/microsoft/Detours/wiki/DetourFinishHelperProcess
    DetourFinishHelperProcess(hwnd, hinst, str, int_param);
}

