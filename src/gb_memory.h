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
#ifndef GB_MEMORY_H_
#define GB_MEMORY_H_

#include "types.h"

// CPU Memory
union Memory {
    byte raw[0x10000];
    struct {
        byte rombank0[0x4000];      // 16kB ROM bank #0
        byte swrombank[0x4000];     // 16kB switchable ROM bank
        byte vidram[0x2000];        // 8kB video RAM
        byte swrambank[0x2000];     // 8kB switchable RAM bank
        byte intram[0x2000];        // 8kB internal RAM
        byte echointram[0x1e00];    // Echo of internal RAM (0xc000 - 0xde00)
        byte oam[0xa0];             // Sprite attribute memory
        byte _emptyio[0x60];        // Empty but unusable for I/O
        byte io[0x4c];              // I/O ports
        byte _emptyio_h[0x34];      // Empty but unusable for I/O
        byte intram2[0x7f];         // More internal RAM
        byte intenable;             // Interrupt enable register
    };

    void write(word, byte);         // Restrict writing
};

#endif
