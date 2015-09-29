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

class Emulator {
private:
	byte cartridge[0x200000];
	byte cartridge_t;
	byte rom_size;
	byte ram_size;
	LR35902 *cpu;
	Memory *mem;

	byte getROMSize(byte);

public:
	Emulator();

	/* Initializes the ROM memory from file(name).
	 * If it cannot open the file, it returns false.
	 * Otherwise, it returns true.
	 */
	bool initRom(const char*);

	void update();

	void dumpInfo();

	string cartridgeToString();
};

#endif
