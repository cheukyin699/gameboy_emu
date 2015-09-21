#ifndef GB_MEMORY_H_
#define GB_MEMORY_H_

#include "types.h"

// CPU Memory
union Memory {
	byte raw[0x10000];
	struct {
		byte rombank0[0x4000];		// 16kB ROM bank #0
		byte swrombank[0x4000];		// 16kB switchable ROM bank
		byte vidram[0x2000];		// 8kB video RAM
		byte swrambank[0x2000];		// 8kB switchable RAM bank
		byte intram[0x2000];		// 8kB internal RAM
		byte echointram[0x1e00];	// Echo of internal RAM (0xc000 - 0xde00)
		byte oam[0xa0];			// Sprite attribute memory
		byte _emptyio[0x60];		// Empty but unusable for I/O
		byte io[0x4c];			// I/O ports
		byte _emptyio_h[0x34];		// Empty but unusable for I/O
		byte intram2[0x7f];		// More internal RAM
		byte intenable;			// Interrupt enable register
	};
};

#endif
