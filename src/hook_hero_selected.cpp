#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

#include <windows.h>

#include "asm_tools.hpp"
#include "hook_hero_selected.hpp"

intptr_t hero_select_addr = 0x00C09C20;

void* (__thiscall* trampoline_on_hero_selected)(void* self, void* hero);

struct Hero
{
    byte data[48];
    int moves;
};

int current_moves = -1;
int** path_details;

void* __stdcall my_on_hero_selected(void* hero)
{
    Hero* self;
    __asm mov self, ecx
    __asm mov path_details, edi
    void* result = trampoline_on_hero_selected(self, hero);
    current_moves = self->moves;
    return result;
}

bool setup_hero_select_hook()
{
    static const std::vector<unsigned char> expected_instructions = { 0x56, 0x57, 0x8b, 0x7c, 0x24, 0xc, 0x57 };
    if (!std::equal(expected_instructions.cbegin(),
        expected_instructions.cend(),
        reinterpret_cast<unsigned char*>(hero_select_addr)))
    {
        std::cout << "Failed to setup hero select hook: unexpected instructions" << std::endl;
        return false;
    }
    trampoline_on_hero_selected = reinterpret_cast<decltype(trampoline_on_hero_selected)>(
        VirtualAlloc(nullptr,
            100,
            MEM_RESERVE | MEM_COMMIT,
            PAGE_EXECUTE_READWRITE));
    if (trampoline_on_hero_selected == nullptr)
    {
        std::cout << "Failed to allocate memory for trampoline function " << GetLastError() << std::endl;
        return false;
    }
    DWORD beforeProtection;
    if (VirtualProtect(reinterpret_cast<LPVOID>(0x00401000), 10522624, PAGE_EXECUTE_READWRITE, &beforeProtection) == 0)
    {
        std::cout << "Failed to change beforeProtection to read write " << GetLastError() << std::endl;
        return false;
    }
    std::memcpy(trampoline_on_hero_selected, reinterpret_cast<LPVOID>(hero_select_addr), 6);
    std::memset(reinterpret_cast<LPVOID>(hero_select_addr), 0x90, 6); // NOP
    write_jmp(reinterpret_cast<void*>(hero_select_addr), &my_on_hero_selected);
    write_jmp(reinterpret_cast<void*>(reinterpret_cast<intptr_t>(trampoline_on_hero_selected) + 6),
        reinterpret_cast<void*>(hero_select_addr + 6));
    DWORD ignore;
    return VirtualProtect(reinterpret_cast<LPVOID>(0x00401000), 10522624, beforeProtection, &ignore) != 0;
}

int get_current_moves()
{
    return current_moves;
}

int get_path_length()
{
    if (path_details == nullptr) {
        return 0;
    }
    int* path_start = *(path_details - 33);
    int* path_end = *(path_details - 31);
    if (path_start != nullptr && *path_start == 0 && path_end > path_start && path_end - path_start < 200) {
        int totalCost = 1;
        while (path_start < path_end) {
            totalCost += -*path_start++ - 1;
        }
        return totalCost;
    }
    return 0;
}