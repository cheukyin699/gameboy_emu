#include "Emulator.h"

int main(int argc, char *argv[]) {
	if (argc <= 1) return -1;


	Emulator *emu = new Emulator();
	emu->initRom(argv[1]);

	while (true) {
		emu->update();
	}

	return 0;
}
