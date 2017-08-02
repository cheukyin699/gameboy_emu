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

#include <iostream>

#include "Emulator.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc <= 1) return -1;


    Emulator *emu = new Emulator();
    emu->initRom(argv[1]);
    emu->dumpInfo();

    // Check if I can run the cartridge
    // (this emulator does not currently support memory bank swapping)
    if (emu->cartridge_t != 0 || emu->rom_size != 2 || emu->ram_size > 1) {
        cout << "Sorry, this emulator does not support memory bank swapping." << endl;
        return 0;
    }

    while (true) {
        emu->update();
    }

    return 0;
}
