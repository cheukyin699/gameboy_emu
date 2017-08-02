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
#ifndef EMULATOR_H_
#define EMULATOR_H_

#include <string>

#include "LR35902.h"
#include "gb_memory.h"
#include "types.h"

using namespace std;

const int MaxCycles = 69905;
const unsigned RAM_SIZE_TABLE[] = {0, 1, 1, 4, 16};

class Emulator {
private:
    byte cartridge[0x200000];
    string title;
    LR35902 *cpu;
    Memory *mem;

    unsigned getROMSize(byte);
    void readTitle();

public:
    // Information about the cartridge
    byte cartridge_t;
    unsigned rom_size;     // Number of banks
    unsigned ram_size;     // Number of banks
    bool dest_code;        // Japanese or not
    bool is_gb;            // GB or SGB

    Emulator();
    ~Emulator();

    /* Initializes the ROM memory from file(name).
     * If it cannot open the file, it returns false.
     * Otherwise, it returns true.
     */
    bool initRom(const char*);

    void update();

    void dumpInfo();

    /* Uses 2 checks (the fact that the cartridge starting 0x104 to 0x133 must
     * match exactly with a table in the internal rom, and the checksum). If the
     * game successfully makes it through these 2 checks, return true.
     * Otherwise, return false.
     */
    bool checkRom();

    /* Returns true if the checksum performed on the cartridge is valid. Returns
     * false otherwise.
     */
    bool checksum();

    string cartridgeToString();
};

#endif
