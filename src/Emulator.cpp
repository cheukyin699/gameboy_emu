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
