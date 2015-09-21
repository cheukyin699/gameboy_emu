#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "LR35902.h"
#include "gb_memory.h"
#include "types.h"

const int MaxCycles = 69905;

class Emulator {
private:
	byte cartridge[0x200000];
	LR35902 *cpu;
	Memory *mem;

public:
	Emulator();

	/* Initializes the ROM memory from file(name).
	 * If it cannot open the file, it returns false.
	 * Otherwise, it returns true.
	 */
	bool initRom(const char*);

	void update();
};

#endif
