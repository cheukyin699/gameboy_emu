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
#ifndef LR35902_H_
#define LR35902_H_

#include "gb_memory.h"
#include "types.h"

/* The only 4 flags in the game boy
 * ZFLAG - The zero flag
 * NFLAG - The subtract flag
 * HFLAG - The half carry flag
 * CFLAG - The carry flag
 */
#define ZFLAG	0x80
#define NFLAG	0x40
#define HFLAG	0x20
#define CFLAG	0x10

typedef word_w Register;

class LR35902 {
private:
	// A pointer to the memory
	Memory *mem;
	// Some registers
	Register af;		// A and F
	Register bc;		// B and C
	Register de;		// D and E
	Register hl;		// H and L
	word sp;		// Stack pointer that grows downwards in memory
	word pc;		// Program counter (instruction pointer)

	// Some flag setters and resetters
	inline void setZ() {af.lo |= ZFLAG;}	// Sets the zero flag
	inline void setN() {af.lo |= NFLAG;}	// Sets the subtract flag
	inline void setH() {af.lo |= HFLAG;}	// Sets the half carry flag
	inline void setC() {af.lo |= CFLAG;}	// Sets the carry flag
	inline void resetZ() {af.lo &= ~ZFLAG;} // Resets the zero flag
	inline void resetN() {af.lo &= ~NFLAG;} // Resets the subtract flag
	inline void resetH() {af.lo &= ~HFLAG;} // Resets the half carry flag
	inline void resetC() {af.lo &= ~CFLAG;} // Resets the carry flag
	inline bool getFlag(byte mask) {return ((af.lo & mask) != 0? true:false);}

	// Some complex operations that need to
	// be performed quite a lot of times
	void ADD_A(byte);
	void ADD_HL(word);
	void ADD_SP(byte);
	void SUB_A(byte);
	void AND_A(byte);
	void OR_A(byte);
	void XOR_A(byte);
	void CP_A(byte);

	// Some complex operations prefixed with CB that need
	// to be performed quite a lot of times
	void SWAP(byte&);
	void RLC(byte&);
	void RL(byte&);
	void RRC(byte&);
	void RR(byte&);
	void SLA(byte&);
	void SRA(byte&);
	void SRL(byte&);
	void BIT(byte, byte&);
	inline void SET(byte, byte&);
	inline void RES(byte, byte&);

	// Other operations
	void pushPCToStack();

	// This huge function mimics the execCurr() function, only that this is
	// tailored for all commands prefixed with 0xcb.
	void execCB(byte);

public:
	LR35902(Memory*);

	/* Executes the current command (the one pointed with the PC).
	 * Returns the number of cycles the command has to undergo.
	 */
	int execCurr();

	/* Some functions for easier debugging */

	/* Gets the Program Counter */
	word getPC() {return pc;}
};

/* The entries with 0 in them are entries that are determined at runtime,
 * particularly because the cycles are dynamic (i.e. the depend on runtime
 * variables)
 */
static int LR35902_opcycles[] =
// 0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
{  4, 12,  8,  8,  4,  4,  8,  4, 20,  8,  8,  8,  4,  4,  8,  4,  // 0x
   4, 12,  8,  8,  4,  4,  8,  4, 12,  8,  8,  8,  4,  4,  8,  4,  // 1x
   0, 12,  8,  8,  4,  4,  8,  4,  0,  8,  8,  8,  4,  4,  8,  4,  // 2x
   0, 12,  8,  8, 12, 12, 12,  4,  0,  8,  8,  8,  4,  4,  8,  4,  // 3x
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 4x
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 5x
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 6x
   8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4,  // 7x
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 8x
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 9x
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // ax
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // bx
   0, 12, 12, 16,  0, 16,  8, 16,  0, 16, 12,  4,  0, 24,  8, 16,  // cx
   0, 12, 12,  0,  0, 16,  8, 16,  0, 16, 12,  0,  0,  0,  8, 16,  // dx
  12, 12,  8,  0,  0, 16,  8, 16, 16,  4, 16,  0,  0,  0,  8, 16,  // ex
  12, 12,  8,  4,  0, 16,  8, 16, 12,  8, 16,  4,  0,  0,  8, 16   // fx
};

/* This whole list is for the opcodes with prefix CB on them.
 * Basically, this copied 16 times, so I made it into a function. */
// 0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
// 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0x
static inline int LR35902_cbopcycles(int opcode) {
	switch (opcode % 16) {
	case 0x6:
	case 0xe:
		return 16;
	default:
		return 8;
	}
}

/* A very long list of the different sizes of different opcodes.
 * The entries with 0 in them mean opcodes that don't exist.
 */
static int LR35902_opsizes[] =
// 0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
{  1,  3,  1,  1,  1,  1,  2,  1,  3,  1,  1,  1,  1,  1,  2,  1,  // 0x
   2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,  // 1x
   2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,  // 2x
   2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,  // 3x
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4x
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5x
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6x
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7x
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8x
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9x
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // ax
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // bx
   1,  1,  3,  3,  3,  1,  2,  1,  1,  1,  3,  2,  3,  3,  2,  1,  // cx
   1,  1,  3,  0,  3,  1,  2,  1,  1,  1,  3,  0,  3,  0,  2,  1,  // dx
   2,  1,  2,  0,  0,  1,  2,  1,  2,  1,  3,  0,  0,  0,  2,  1,  // ex
   2,  1,  2,  1,  0,  1,  2,  1,  2,  1,  3,  1,  0,  0,  2,  1   // fx
};

#endif
