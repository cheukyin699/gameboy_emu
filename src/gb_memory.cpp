#include "gb_memory.h"

Memory::~Memory() {
}

void Memory::write(word addr, byte d) {
    if (addr < 0x8000) {
        /* Don't write to ROM */
    } else if ((addr >= 0xc000) && (addr < 0xe000)) {
        /* Writing to echo RAM */
        raw[addr] = d;
        raw[addr+0x2000] = d;
    } else if ((addr >= 0xe000) && (addr < 0xfe00)) {
        /* Writing to echo RAM */
        raw[addr] = d;
        raw[addr-0x2000] = d;
    } else if ((addr >= 0xfea0) && (addr < 0xfeff)) {
        /* This is a restricted area */
    } else {
        /* All other memory can be written to perfectly fine */
        raw[addr] = d;
    }
}
