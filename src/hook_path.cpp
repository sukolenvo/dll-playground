#include <cstdint>
#include <iostream>
#include <vector>

#include <windows.h>

#include "asm_tools.hpp"
#include "hook_path.hpp"

intptr_t path_func_addr = 0x008dd910;

void* (__fastcall* trampoline_on_path_selected)(void* ecx, void *edx, void* param1, void* param2);

struct PathPoint
{
    int xy;
    char remaining[0x40-4];
};

struct Path
{
    PathPoint* start;
    PathPoint* end;
};

void* __fastcall my_on_path_selected(void* ecx, void* edx, Path* param1, void *param2)
{
    void* result = trampoline_on_path_selected(ecx, edx, param1, param2);
    int** costsP = reinterpret_cast<int**>((intptr_t)ecx - 0x58);
    int* costs = *costsP;

    const int steps = param1->end - param1->start;
    int totalCost = 1;
    for (int i = 0; i < steps; ++i)
    {
        totalCost += -*costs++ - 1;
    }
    return result;
}

bool setup_path_hook()
{
    static const std::vector<unsigned char> expected_instructions = { 0x57, 0x8b, 0xf9, 0x80, 0xbf, 0x4c, 0xff, 0xff, 0xff, 0x00 };
    if (!std::equal(expected_instructions.cbegin(),
        expected_instructions.cend(),
        reinterpret_cast<unsigned char*>(path_func_addr)))
    {
        std::cout << "Failed to setup path hook: unexpected instructions" << std::endl;
        return false;
    }
    trampoline_on_path_selected = reinterpret_cast<decltype(trampoline_on_path_selected)>(
        VirtualAlloc(nullptr,
            100,
            MEM_RESERVE | MEM_COMMIT,
            PAGE_EXECUTE_READWRITE));

    DWORD beforeProtection;
    if (VirtualProtect(reinterpret_cast<LPVOID>(0x00401000), 10522624, PAGE_EXECUTE_READWRITE, &beforeProtection) == 0)
    {
        std::cout << "Failed to change beforeProtection to read write " << GetLastError() << std::endl;
        return false;
    }
    std::memcpy(trampoline_on_path_selected, reinterpret_cast<LPVOID>(path_func_addr), expected_instructions.size());
    std::memset(reinterpret_cast<LPVOID>(path_func_addr), 0x90, expected_instructions.size()); // NOP
    write_jmp(reinterpret_cast<void*>(path_func_addr), &my_on_path_selected);
    write_jmp(reinterpret_cast<void*>(reinterpret_cast<intptr_t>(trampoline_on_path_selected) + expected_instructions.size()),
        reinterpret_cast<void*>(path_func_addr + expected_instructions.size()));
    DWORD ignore;
    return VirtualProtect(reinterpret_cast<LPVOID>(0x00401000), 10522624, beforeProtection, &ignore) != 0;
}
