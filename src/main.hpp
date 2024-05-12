#ifndef DLL_PLAYGROUND_MAIN_HPP
#define DLL_PLAYGROUND_MAIN_HPP

#include <windows.h>

BOOL WINAPI DllMain (HANDLE handle, DWORD dwReason, LPVOID );

__declspec(dllexport)
VOID CALLBACK MyDetourFinishHelperProcess(
    _In_ HWND,
    _In_ HINSTANCE,
    _In_ LPSTR,
    _In_ INT
);

#endif //DLL_PLAYGROUND_MAIN_HPP
