/* This file is part of gameboy_emu.
 *
 * gameboy_emu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * gameboy_emu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with gameboy_emu.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "Emulator.h"

using namespace std;

Emulator::Emulator() {
    title = "";
    mem = new Memory;
    cpu = new LR35902(mem);
}

bool Emulator::initRom(const char *fn) {
    FILE *rom;
    rom = fopen(fn, "rb");

    if (rom == NULL) return false;

    fread(cartridge, 1, 0x200000, rom);
    fclose(rom);

    // Loads first 0x8000 bytes into first 0x8000 memory
    memcpy(&mem->raw, &cartridge, 0x8000);

    // Find the cartridge type
    cartridge_t = mem->raw[0x147];

    // Find the size of the ROM
    rom_size = getROMSize(mem->raw[0x148]);

    // Find the size of the RAM
    switch (mem->raw[0x149]) {
    case 0: ram_size = 0; break;
    case 1: ram_size = 1; break;
    case 2: ram_size = 1; break;
    case 3: ram_size = 4; break;
    case 4: ram_size = 16; break;
    default: ram_size = 0; break;
    }

    // Find the title of the ROM
    readTitle();

    // Get destination code
    dest_code = mem->raw[0x14a];

    // SGB or GB?
    is_gb = mem->raw[0x146] == 0x00;

    return true;
}

void Emulator::update() {
    int cyclesthisupdate = 0;

    while (cyclesthisupdate < MaxCycles) {
        int cycles = cpu->execCurr();
        cyclesthisupdate += cycles;
    }
}

unsigned Emulator::getROMSize(byte b) {
    if (b <= 6) {
        return pow(2, b+1);
    } else {
        switch (b) {
        case 0x52: return 72;
        case 0x53: return 80;
        case 0x54: return 96;
        default: return 0;
        }
    }
}

void Emulator::dumpInfo() {
    cout << "Information for current ROM: `" << title << "'" << endl
        << "Cartridge type: " << cartridgeToString() << endl
        << "ROM size: " << rom_size << " banks" << endl
        << "RAM size: " << ram_size << " banks" << endl
        << "Destination code: " << (dest_code? "Non-Japanese": "Japanese") << endl
        << "For GB? " << (is_gb? "yes": "no") << endl;
}

string Emulator::cartridgeToString() {
    switch (cartridge_t) {
    case 0x00: return "ROM ONLY";
    case 0x01: return "ROM + MBC1";
    case 0x02: return "ROM + MBC1 + RAM";
    case 0x03: return "ROM + MBC1 + RAM + BATT";
    case 0x05: return "ROM + MBC2";
    case 0x06: return "ROM + MBC2 + BATT";
    case 0x08: return "ROM + RAM";
    case 0x09: return "ROM + RAM + BATT";
    case 0x0b: return "ROM + MMM01";
    case 0x0c: return "ROM + MMM01 + SRAM";
    case 0x0d: return "ROM + MMM01 + SRAM + BATT";
    case 0x0f: return "ROM + MBC3 + TIMER + BATT";
    case 0x10: return "ROM + MBC3 + TIMER + RAM + BATT";
    case 0x11: return "ROM + MBC3";
    case 0x12: return "ROM + MBC3 + RAM";
    case 0x13: return "ROM + MBC3 + RAM + BATT";
    case 0x19: return "ROM + MBC5";
    case 0x1a: return "ROM + MBC5 + RAM";
    case 0x1b: return "ROM + MBC5 + RAM + BATT";
    case 0x1c: return "ROM + MBC5 + RUMBLE";
    case 0x1d: return "ROM + MBC5 + RUMBLE + SRAM";
    case 0x1e: return "ROM + MBC5 + RUMBLE + SRAM + BATT";
    case 0x1f: return "Pocket Camera";
    case 0xfd: return "Bandai TAMA5";
    case 0xfe: return "Hudson HuC-3";
    case 0xff: return "Hudson HuC-1";
    default: return "Unknown";
    }
}

void Emulator::readTitle() {
    // Reads from 0x134-0x142
    for (unsigned off = 0; mem->raw[0x134+off] != 0 && 0x134+off <= 0x142; off++) {
        title += char(mem->raw[0x134+off]);
    }
}
