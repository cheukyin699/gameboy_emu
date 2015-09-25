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
#ifndef TYPES_H_
#define TYPES_H_

/* Some useful definitions */

typedef unsigned char byte;
typedef unsigned short word;

union word_w {
	word val;
	struct {
		#ifndef BIG_ENDIAN
		byte hi, lo;		// Little endian
		#else
		byte lo, hi;		// Big endian
		#endif
	};
};

// Bit operations find this map useful
static unsigned bit_vals[] = {
	0x01, 0x02, 0x04, 0x08,
	0x10, 0x20, 0x40, 0x80
};

#endif
