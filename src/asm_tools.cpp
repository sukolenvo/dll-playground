#include <cstdint>

#include "asm_tools.hpp"

void write_jmp(void* from, void* to)
{
    intptr_t distance = reinterpret_cast<intptr_t>(to) - 5 - reinterpret_cast<intptr_t>(from);
    auto i = reinterpret_cast<unsigned char*>(from);
    *i++ = 0xe9;
    *i++ = distance & 0xff;
    distance >>= 8;
    *i++ = distance & 0xff;
    distance >>= 8;
    *i++ = distance & 0xff;
    distance >>= 8;
    *i = distance & 0xff;
}
