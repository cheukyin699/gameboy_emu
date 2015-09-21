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
#include <cstdio>
#include <cstring>
#include <iostream>

#include "Emulator.h"

using namespace std;

Emulator::Emulator() {
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

	return true;
}

void Emulator::update() {
	int cyclesthisupdate = 0;

	while (cyclesthisupdate < MaxCycles) {
		int cycles = cpu->execCurr();
		cyclesthisupdate += cycles;
	}
}
