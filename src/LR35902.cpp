#include "LR35902.h"

LR35902::LR35902(Memory *m) {
	mem = m;
	pc = 0x100;
	sp = 0xfffe;

	// Initial register values
	af.val = 0x01;
	af.lo = 0xb0;
	bc.val = 0x13;
	de.val = 0xd8;
	hl.val = 0x14d;
}

void LR35902::execCB(byte code) {
	switch (code) {
	case 0x00:
		// RLC B
		RLC(bc.hi);
		break;
	case 0x01:
		// RLC C
		RLC(bc.hi);
		break;
	case 0x02:
		// RLC D
		RLC(de.hi);
		break;
	case 0x03:
		// RLC E
		RLC(de.lo);
		break;
	case 0x04:
		// RLC H
		RLC(hl.hi);
		break;
	case 0x05:
		// RLC L
		RLC(hl.lo);
		break;
	case 0x06:
		// RLC (HL)
		RLC(mem->raw[hl.val]);
		break;
	case 0x07:
		// RLC A
		RLC(af.hi);
		break;
	case 0x08:
		// RRC B
		RRC(bc.hi);
		break;
	case 0x09:
		// RRC C
		RRC(bc.lo);
		break;
	case 0x0a:
		// RRC D
		RRC(de.hi);
		break;
	case 0x0b:
		// RRC E
		RRC(de.lo);
		break;
	case 0x0c:
		// RRC H
		RRC(hl.hi);
		break;
	case 0x0d:
		// RRC L
		RRC(hl.lo);
		break;
	case 0x0e:
		// RRC (HL)
		RRC(mem->raw[hl.val]);
		break;
	case 0x0f:
		// RRC A
		RRC(af.hi);
		break;
	case 0x10:
		// RL B
		RL(bc.hi);
		break;
	case 0x11:
		// RL C
		RL(bc.hi);
		break;
	case 0x12:
		// RL D
		RL(de.hi);
		break;
	case 0x13:
		// RL E
		RL(de.lo);
		break;
	case 0x14:
		// RL H
		RL(hl.hi);
		break;
	case 0x15:
		// RL L
		RL(hl.lo);
		break;
	case 0x16:
		// RL (HL)
		RL(mem->raw[hl.val]);
		break;
	case 0x17:
		// RL A
		RL(af.hi);
		break;
	case 0x18:
		// RR B
		RR(bc.hi);
		break;
	case 0x19:
		// RR C
		RR(bc.lo);
		break;
	case 0x1a:
		// RR D
		RR(de.hi);
		break;
	case 0x1b:
		// RR E
		RR(de.lo);
		break;
	case 0x1c:
		// RR H
		RR(hl.hi);
		break;
	case 0x1d:
		// RR L
		RR(hl.lo);
		break;
	case 0x1e:
		// RR (HL)
		RR(mem->raw[hl.val]);
		break;
	case 0x1f:
		// RR A
		RR(af.hi);
		break;
	case 0x30:
		// SWAP B
		SWAP(bc.hi);
		break;
	case 0x31:
		// SWAP C
		SWAP(bc.lo);
		break;
	case 0x32:
		// SWAP D
		SWAP(de.hi);
		break;
	case 0x33:
		// SWAP E
		SWAP(de.lo);
		break;
	case 0x34:
		// SWAP H
		SWAP(hl.hi);
		break;
	case 0x35:
		// SWAP L
		SWAP(hl.lo);
		break;
	case 0x36:
		// SWAP (HL)
		SWAP(mem->raw[hl.val]);
		break;
	case 0x37:
		// SWAP A
		SWAP(af.hi);
		break;
	}
}

int LR35902::execCurr() {
	byte opcode = mem->raw[pc];
	byte *args = &mem->raw[pc+1];
	word *wargs = (word*)&mem->raw[pc+1];
	int cycles = LR35902_opcycles[opcode];

	// Increment program counter first because you might change it later
	pc += LR35902_opsizes[opcode];

	switch (opcode) {
	case 0x00:
		// NOP
		break;
	case 0x01:
		// LD BC, nn
		bc.val = wargs[0];
		break;
	case 0x02:
		// LD (BC), A
		mem->raw[bc.val] = af.hi;
		break;
	case 0x03:
		// INC BC
		bc.val++;
		break;
	case 0x04:
		// INC B
		if (int(bc.hi&0xf)+1 > ((++bc.hi)&0xf)) setH();
		if (bc.hi == 0) setZ();
		resetN();
		break;
	case 0x05:
		// DEC B
		setN();
		if (int(bc.hi&0xf)-1 < ((--bc.hi)&0xf)) setH();
		if (bc.hi == 0) setZ();
		break;
	case 0x06:
		// LD B, n
		bc.hi = args[0];
		break;
	case 0x07:
		// RLCA
		RLC(af.hi);
		break;
	case 0x08:
		// LD (nn), SP
		mem->raw[wargs[0]] = sp;
		break;
	case 0x09:
		// ADD HL, BC
		ADD_HL(bc.val);
		break;
	case 0x0a:
		// LD A, (BC)
		af.hi = mem->raw[bc.val];
		break;
	case 0x0b:
		// DEC BC
		bc.val--;
		break;
	case 0x0c:
		// INC C
		if (int(bc.lo&0xf)+1 > ((++bc.lo)&0xf)) setH();
		if (bc.lo == 0) setZ();
		resetN();
		break;
	case 0x0d:
		// DEC C
		setN();
		if (int(bc.lo&0xf)-1 < ((--bc.lo)&0xf)) setH();
		if (bc.lo == 0) setZ();
		break;
	case 0x0e:
		// LD C, n
		bc.lo = args[0];
		break;
	case 0x0f:
		// RRCA
		RRC(af.hi);
		break;
	case 0x10:
		if (args[0] == 0) {
			// STOP
			// TODO
			// Halts CPU & Display until a button is pressed.
		}
		else {
			// Invalid (or something)
		}
		break;
	case 0x11:
		// LD DE, nn
		de.val = wargs[0];
		break;
	case 0x12:
		// LD (DE), A
		mem->raw[de.val] = af.hi;
		break;
	case 0x13:
		// INC DE
		de.val++;
		break;
	case 0x14:
		// INC D
		if (int(de.hi&0xf)+1 > ((++de.hi)&0xf)) setH();
		if (de.hi == 0) setZ();
		resetN();
		break;
	case 0x15:
		// DEC D
		setN();
		if (int(de.hi&0xf)-1 < ((--de.hi)&0xf)) setH();
		if (de.hi == 0) setZ();
		break;
	case 0x16:
		// LD D, n
		de.hi = args[0];
		break;
	case 0x17:
		// RLA
		RL(af.hi);
		break;
	case 0x18:
		// JR n
		pc += args[0];
		break;
	case 0x19:
		// ADD HL, DE
		ADD_HL(de.val);
		break;
	case 0x1a:
		// LD A, (DE)
		af.hi = mem->raw[de.val];
		break;
	case 0x1b:
		// DEC DE
		de.val--;
		break;
	case 0x1c:
		// INC E
		if (int(de.lo&0xf)+1 > ((++de.lo)&0xf)) setH();
		if (de.lo == 0) setZ();
		resetN();
		break;
	case 0x1d:
		// DEC E
		setN();
		if (int(de.lo&0xf)-1 < ((--de.lo)&0xf)) setH();
		if (de.lo == 0) setZ();
		break;
	case 0x1e:
		// LD E, n
		de.lo = args[0];
		break;
	case 0x1f:
		// RRA
		RR(af.hi);
		break;
	case 0x20:
		// JR NZ, n
		if (!getFlag(ZFLAG))
			pc += args[0];
		break;
	case 0x21:
		// LD HL, nn
		hl.val = wargs[0];
		break;
	case 0x22:
		/* LD (HLI), A
		 * LD (HL++), A
		 * LDI (HL), A
		 */
		mem->raw[hl.val] = af.hi;
		hl.val++;
		break;
	case 0x23:
		// INC HL
		hl.val++;
		break;
	case 0x24:
		// INC H
		if (int(hl.hi&0xf)+1 > ((++hl.hi)&0xf)) setH();
		if (hl.hi == 0) setZ();
		resetN();
		break;
	case 0x25:
		// DEC H
		setN();
		if (int(hl.hi&0xf)-1 < ((--hl.hi)&0xf)) setH();
		if (hl.hi == 0) setZ();
		break;
	case 0x26:
		// LD H, n
		hl.hi = args[0];
		break;
	case 0x27:
		// DAA
		if (af.hi == 0) setZ();
		resetH();
		{
			byte hi = af.hi / 10;
			byte lo = af.hi % 10;
			byte t = (hi << 4) | lo;
			if (t == af.hi) resetC();
			else			setC();
		}
		break;
	case 0x28:
		// JR Z, n
		if (getFlag(ZFLAG))
			pc += args[0];
		break;
	case 0x29:
		// ADD HL, HL
		ADD_HL(hl.val);
		break;
	case 0x2a:
		/* LD A, (HLI)
		 * LD A, (HL++)
		 * LDI A, (HL)
		 */
		af.hi = mem->raw[hl.val];
		hl.val++;
		break;
	case 0x2b:
		// DEC HL
		hl.val--;
		break;
	case 0x2c:
		// INC L
		if (int(hl.lo&0xf)+1 > ((++hl.lo)&0xf)) setH();
		if (hl.lo == 0) setZ();
		resetN();
		break;
	case 0x2d:
		// DEC L
		setN();
		if (int(hl.lo&0xf)-1 < ((--hl.lo)&0xf)) setH();
		if (hl.lo == 0) setZ();
		break;
	case 0x2e:
		// LD L, n
		hl.lo = args[0];
		break;
	case 0x2f:
		// CPL
		af.hi = ~af.hi;
		setN();
		setH();
		break;
	case 0x30:
		// JR NC, n
		if (!getFlag(CFLAG))
			pc += args[0];
		break;
	case 0x31:
		// LD SP, nn
		sp = wargs[0];
		break;
	case 0x32:
		/* LD (HLD), A
		 * LD (HL--), A
		 * LDD (HL), A
		 */
		mem->raw[hl.val] = af.hi;
		hl.val--;
		break;
	case 0x33:
		// INC SP
		sp++;
		break;
	case 0x34:
		// INC (HL)
		if (int(mem->raw[hl.val]&0xf)+1 > ((++mem->raw[hl.val])&0xf)) setH();
		if (mem->raw[hl.val] == 0) setZ();
		resetN();
		break;
	case 0x35:
		// DEC (HL)
		setN();
		if (int(mem->raw[hl.val]&0xf)-1 < ((--mem->raw[hl.val])&0xf)) setH();
		if (mem->raw[hl.val] == 0) setZ();
		break;
	case 0x37:
		// SCF
		resetN();
		resetH();
		setC();
		break;
	case 0x38:
		// JR C, n
		if (getFlag(CFLAG))
			pc += args[0];
		break;
	case 0x39:
		// ADD HL, SP
		ADD_HL(sp);
		break;
	case 0x3a:
		/* LD A, (HLD)
		 * LD A, (HL--)	 ; Post-increment
		 * LDD A, (HL)
		 */
		af.hi = mem->raw[hl.val];
		hl.val--;
		break;
	case 0x3b:
		// DEC SP
		sp--;
		break;
	case 0x3c:
		// INC A
		if (int(af.hi&0xf)+1 > ((++af.hi)&0xf)) setH();
		if (af.hi == 0) setZ();
		resetN();
		break;
	case 0x3d:
		// DEC A
		setN();
		if (int(af.hi&0xf)-1 < ((--af.hi)&0xf)) setH();
		if (af.hi == 0) setZ();
		break;
	case 0x3e:
		// LD A, n
		af.hi = args[0];
		break;
	case 0x3f:
		// CCF
		if (getFlag(CFLAG)) resetC();
		else				setC();
		resetN();
		resetH();
		break;
	case 0x40:
		// LD B, B
		bc.hi = bc.hi;
		break;
	case 0x41:
		// LD B, C
		bc.hi = bc.lo;
		break;
	case 0x42:
		// LD B, D
		bc.hi = de.hi;
		break;
	case 0x43:
		// LD B, E
		bc.hi = de.lo;
		break;
	case 0x44:
		// LD B, H
		bc.hi = hl.hi;
		break;
	case 0x45:
		// LD B, L
		bc.hi = hl.lo;
		break;
	case 0x46:
		// LD B, (HL)
		bc.hi = mem->raw[hl.val];
		break;
	case 0x47:
		// LD B, A
		bc.hi = af.hi;
		break;
	case 0x48:
		// LD C, B
		bc.lo = bc.hi;
		break;
	case 0x49:
		// LD C, C
		bc.lo = af.hi;
		break;
	case 0x4a:
		// LD C, D
		bc.lo = de.hi;
		break;
	case 0x4b:
		// LD C, E
		bc.lo = de.lo;
		break;
	case 0x4c:
		// LD C, H
		bc.lo = hl.hi;
		break;
	case 0x4d:
		// LD C, L
		bc.lo = hl.lo;
		break;
	case 0x4e:
		// LD C, (HL)
		bc.lo = mem->raw[hl.val];
		break;
	case 0x4f:
		// LD C, A
		bc.lo = af.hi;
		break;
	case 0x50:
		// LD D, B
		de.hi = bc.hi;
		break;
	case 0x51:
		// LD D, C
		de.hi = bc.lo;
		break;
	case 0x52:
		// LD D, D
		de.hi = de.hi;
		break;
	case 0x53:
		// LD D, E
		de.hi = de.lo;
		break;
	case 0x54:
		// LD D, H
		de.hi = hl.hi;
		break;
	case 0x55:
		// LD D, L
		de.hi = hl.lo;
		break;
	case 0x56:
		// LD D, (HL)
		de.hi = mem->raw[hl.val];
		break;
	case 0x57:
		// LD D, A
		de.hi = af.hi;
		break;
	case 0x58:
		// LD E, B
		de.lo = bc.hi;
		break;
	case 0x59:
		// LD E, C
		de.lo = af.hi;
		break;
	case 0x5a:
		// LD E, D
		de.lo = de.hi;
		break;
	case 0x5b:
		// LD E, E
		de.lo = de.lo;
		break;
	case 0x5c:
		// LD E, H
		de.lo = hl.hi;
		break;
	case 0x5d:
		// LD E, L
		de.lo = hl.lo;
		break;
	case 0x5e:
		// LD E, (HL)
		de.lo = mem->raw[hl.val];
		break;
	case 0x5f:
		// LD E, A
		de.lo = af.hi;
		break;
	case 0x60:
		// LD H, B
		hl.hi = bc.hi;
		break;
	case 0x61:
		// LD H, C
		hl.hi = bc.lo;
		break;
	case 0x62:
		// LD H, D
		hl.hi = de.hi;
		break;
	case 0x63:
		// LD H, E
		hl.hi = de.lo;
		break;
	case 0x64:
		// LD H, H
		hl.hi = hl.hi;
		break;
	case 0x65:
		// LD H, L
		hl.hi = hl.lo;
		break;
	case 0x66:
		// LD H, (HL)
		hl.hi = mem->raw[hl.val];
		break;
	case 0x67:
		// LD H, A
		hl.hi = af.hi;
		break;
	case 0x68:
		// LD L, B
		hl.lo = bc.hi;
		break;
	case 0x69:
		// LD L, C
		hl.lo = af.hi;
		break;
	case 0x6a:
		// LD L, D
		hl.lo = de.hi;
		break;
	case 0x6b:
		// LD L, E
		hl.lo = de.lo;
		break;
	case 0x6c:
		// LD L, H
		hl.lo = hl.hi;
		break;
	case 0x6d:
		// LD L, L
		hl.lo = hl.lo;
		break;
	case 0x6e:
		// LD L, (HL)
		hl.lo = mem->raw[hl.val];
		break;
	case 0x6f:
		// LD L, A
		hl.lo = af.hi;
		break;
	case 0x70:
		// LD (HL), B
		mem->raw[hl.val] = bc.hi;
		break;
	case 0x71:
		// LD (HL), C
		mem->raw[hl.val] = bc.lo;
		break;
	case 0x72:
		// LD (HL), D
		mem->raw[hl.val] = de.hi;
		break;
	case 0x73:
		// LD (HL), E
		mem->raw[hl.val] = de.lo;
		break;
	case 0x74:
		// LD (HL), H
		mem->raw[hl.val] = hl.hi;
		break;
	case 0x75:
		// LD (HL), L
		mem->raw[hl.val] = hl.lo;
		break;
	case 0x76:
		// HALT
		// Waits until an interrupt occurs.
		// TODO
		break;
	case 0x77:
		// LD (HL), A
		mem->raw[hl.val] = af.hi;
		break;
	case 0x78:
		// LD A, B
		af.hi = bc.hi;
		break;
	case 0x79:
		// LD A, C
		af.hi = bc.lo;
		break;
	case 0x7a:
		// LD A, D
		af.hi = de.hi;
		break;
	case 0x7b:
		// LD A, E
		af.hi = de.lo;
		break;
	case 0x7c:
		// LD A, H
		af.hi = hl.hi;
		break;
	case 0x7d:
		// LD A, L
		af.hi = hl.lo;
		break;
	case 0x7e:
		// LD A, (HL)
		af.hi = mem->raw[hl.val];
		break;
	case 0x7f:
		// LD A, A
		af.hi = af.hi;
		break;
	case 0x80:
		// ADD A, B
		ADD_A(bc.hi);
		break;
	case 0x81:
		// ADD A, C
		ADD_A(bc.lo);
		break;
	case 0x82:
		// ADD A, D
		ADD_A(de.hi);
		break;
	case 0x83:
		// ADD A, E
		ADD_A(de.lo);
		break;
	case 0x84:
		// ADD A, H
		ADD_A(hl.hi);
		break;
	case 0x85:
		// ADD A, L
		ADD_A(hl.lo);
		break;
	case 0x86:
		// ADD A, (HL)
		ADD_A(mem->raw[hl.val]);
		break;
	case 0x87:
		// ADD A, A
		ADD_A(af.hi);
		break;
	case 0x88:
		// ADC A, B
		ADD_A(bc.hi+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x89:
		// ADC A, C
		ADD_A(bc.lo+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x8a:
		// ADC A, D
		ADD_A(de.hi+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x8b:
		// ADC A, E
		ADD_A(de.lo+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x8c:
		// ADC A, H
		ADD_A(hl.hi+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x8d:
		// ADC A, L
		ADD_A(hl.lo+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x8e:
		// ADC A, (HL)
		ADD_A(mem->raw[hl.val]+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x8f:
		// ADC A, A
		ADD_A(af.hi+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x90:
		// SUB A, B
		SUB_A(bc.hi);
		break;
	case 0x91:
		// SUB A, C
		SUB_A(bc.lo);
		break;
	case 0x92:
		// SUB A, D
		SUB_A(de.hi);
		break;
	case 0x93:
		// SUB A, E
		SUB_A(de.lo);
		break;
	case 0x94:
		// SUB A, H
		SUB_A(hl.hi);
		break;
	case 0x95:
		// SUB A, L
		SUB_A(hl.lo);
		break;
	case 0x96:
		// SUB A, (HL)
		SUB_A(mem->raw[hl.val]);
		break;
	case 0x97:
		// SUB A, A
		SUB_A(af.hi);
		break;
	case 0x98:
		// SBC A, B
		SUB_A(bc.hi+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x99:
		// SBC A, C
		SUB_A(bc.lo+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x9a:
		// SBC A, D
		SUB_A(de.hi+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x9b:
		// SBC A, E
		SUB_A(de.lo+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x9c:
		// SBC A, H
		SUB_A(hl.hi+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x9d:
		// SBC A, L
		SUB_A(hl.lo+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x9e:
		// SBC A, (HL)
		SUB_A(mem->raw[hl.val]+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0x9f:
		// SBC A, A
		SUB_A(af.hi+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0xa0:
		// AND A, B
		AND_A(bc.hi);
		break;
	case 0xa1:
		// AND A, C
		AND_A(bc.lo);
		break;
	case 0xa2:
		// AND A, D
		AND_A(de.hi);
		break;
	case 0xa3:
		// AND A, E
		AND_A(de.lo);
		break;
	case 0xa4:
		// AND A, H
		AND_A(hl.hi);
		break;
	case 0xa5:
		// AND A, L
		AND_A(hl.lo);
		break;
	case 0xa6:
		// AND A, (HL)
		AND_A(mem->raw[hl.val]);
		break;
	case 0xa7:
		// AND A, A
		AND_A(bc.hi);
		break;
	case 0xa8:
		// XOR A, B
		XOR_A(bc.hi);
		break;
	case 0xa9:
		// XOR A, C
		XOR_A(bc.lo);
		break;
	case 0xaa:
		// XOR A, D
		XOR_A(de.hi);
		break;
	case 0xab:
		// XOR A, E
		XOR_A(de.lo);
		break;
	case 0xac:
		// XOR A, H
		XOR_A(hl.hi);
		break;
	case 0xad:
		// XOR A, L
		XOR_A(hl.lo);
		break;
	case 0xae:
		// XOR A, (HL)
		XOR_A(mem->raw[hl.val]);
		break;
	case 0xaf:
		// XOR A, A
		XOR_A(bc.hi);
		break;
	case 0xb0:
		// OR A, B
		OR_A(bc.hi);
		break;
	case 0xb1:
		// OR A, C
		OR_A(bc.lo);
		break;
	case 0xb2:
		// OR A, D
		OR_A(de.hi);
		break;
	case 0xb3:
		// OR A, E
		OR_A(de.lo);
		break;
	case 0xb4:
		// OR A, H
		OR_A(hl.hi);
		break;
	case 0xb5:
		// OR A, L
		OR_A(hl.lo);
		break;
	case 0xb6:
		// OR A, (HL)
		OR_A(mem->raw[hl.val]);
		break;
	case 0xb7:
		// OR A, A
		OR_A(bc.hi);
		break;
	case 0xb8:
		// CP A, B
		CP_A(bc.hi);
		break;
	case 0xb9:
		// CP A, C
		CP_A(bc.lo);
		break;
	case 0xba:
		// CP A, D
		CP_A(de.hi);
		break;
	case 0xbb:
		// CP A, E
		CP_A(de.lo);
		break;
	case 0xbc:
		// CP A, H
		CP_A(hl.hi);
		break;
	case 0xbd:
		// CP A, L
		CP_A(hl.lo);
		break;
	case 0xbe:
		// CP A, (HL)
		CP_A(mem->raw[hl.val]);
		break;
	case 0xbf:
		// CP A, A
		CP_A(bc.hi);
		break;
	case 0xc0:
		// RET NZ
		{
			if (!getFlag(ZFLAG)) {
				word_w w;
				w.lo = mem->raw[sp];
				w.hi = mem->raw[sp+1];
				pc = w.val;
				sp += 2;
			}
			break;
		}
	case 0xc1:
		// POP BC
		bc.hi = mem->raw[sp+1];
		bc.lo = mem->raw[sp];
		sp += 2;
		break;
	case 0xc2:
		// JP NZ, nn
		if (!getFlag(ZFLAG))
			pc = wargs[0];
		break;
	case 0xc3:
		// JP nn
		pc = wargs[0];
		break;
	case 0xc4:
		{
			// CALL NZ, nn
			if (!getFlag(ZFLAG)) {
				word_w w;
				w.val = pc;
				mem->raw[sp-1] = w.hi;
				mem->raw[sp-2] = w.lo;
				sp -= 2;

				// Actually jump
				pc = wargs[0];
			}
			break;
		}
	case 0xc5:
		// PUSH BC
		mem->raw[sp-1] = bc.hi;
		mem->raw[sp-2] = bc.lo;
		sp -= 2;
		break;
	case 0xc6:
		// ADD A, n
		ADD_A(args[0]);
		break;
	case 0xc7:
		// RST 0x00
		pc = 0x00;
		break;
	case 0xc8:
		// RET Z
		{
			if (getFlag(ZFLAG)) {
				word_w w;
				w.lo = mem->raw[sp];
				w.hi = mem->raw[sp+1];
				pc = w.val;
				sp += 2;
			}
			break;
		}
	case 0xc9:
		// RET
		{
			word_w w;
			w.lo = mem->raw[sp];
			w.hi = mem->raw[sp+1];
			pc = w.val;
			sp += 2;
			break;
		}
	case 0xca:
		// JP Z, nn
		if (getFlag(ZFLAG))
			pc = wargs[0];
		break;
	case 0xcb:
		// Extra commands with prefix "CB"
		// Doesn't use lookup - the cycles are too similar.
		cycles = LR35902_cbopcycles(args[0]);
		execCB(args[0]);
		break;
	case 0xcc:
		{
			// CALL Z, nn
			if (getFlag(ZFLAG)) {
				word_w w;
				w.val = pc;
				mem->raw[sp-1] = w.hi;
				mem->raw[sp-2] = w.lo;
				sp -= 2;

				// Actually jump
				pc = wargs[0];
			}
			break;
		}
	case 0xcd:
		{
			// CALL nn
			// Because the instruction size is already
			// added, you can push it to stack. Push high
			// then low.
			word_w w;
			w.val = pc;
			mem->raw[sp-1] = w.hi;
			mem->raw[sp-2] = w.lo;
			sp -= 2;

			// Actually jump
			pc = wargs[0];
			break;
		}
	case 0xce:
		// ADC A, n
		ADD_A(args[0]+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0xcf:
		// RST 0x08
		pc = 0x08;
		break;
	case 0xd0:
		// RET NC
		{
			if (!getFlag(CFLAG)) {
				word_w w;
				w.lo = mem->raw[sp];
				w.hi = mem->raw[sp+1];
				pc = w.val;
				sp += 2;
			}
			break;
		}
	case 0xd1:
		// POP DE
		de.hi = mem->raw[sp+1];
		de.lo = mem->raw[sp];
		sp += 2;
		break;
	case 0xd2:
		// JP NC, nn
		if (!getFlag(CFLAG))
			pc = wargs[0];
		break;
	case 0xd4:
		{
			// CALL NC, nn
			if (!getFlag(CFLAG)) {
				word_w w;
				w.val = pc;
				mem->raw[sp-1] = w.hi;
				mem->raw[sp-2] = w.lo;
				sp -= 2;

				// Actually jump
				pc = wargs[0];
			}
			break;
		}
	case 0xd5:
		// PUSH DE
		mem->raw[sp-1] = de.hi;
		mem->raw[sp-2] = de.lo;
		sp -= 2;
		break;
	case 0xd6:
		// SUB A, n
		SUB_A(args[0]);
		break;
	case 0xd7:
		// RST 0x10
		pc = 0x10;
		break;
	case 0xd8:
		// RET C
		{
			if (getFlag(CFLAG)) {
				word_w w;
				w.lo = mem->raw[sp];
				w.hi = mem->raw[sp+1];
				pc = w.val;
				sp += 2;
			}
			break;
		}
	case 0xda:
		// JP C, nn
		if (getFlag(CFLAG))
			pc = wargs[0];
		break;
	case 0xdc:
		{
			// CALL C, nn
			if (getFlag(CFLAG)) {
				word_w w;
				w.val = pc;
				mem->raw[sp-1] = w.hi;
				mem->raw[sp-2] = w.lo;
				sp -= 2;

				// Actually jump
				pc = wargs[0];
			}
			break;
		}
	case 0xde:
		// SBC A, n
		SUB_A(args[0]+((af.lo&CFLAG) == 0? 0:1));
		break;
	case 0xdf:
		// RST 0x18
		pc = 0x18;
		break;
	case 0xe0:
		// LD ($FF00+n), A
		mem->raw[args[0]+0xff00] = af.hi;
		break;
	case 0xe1:
		// POP HL
		hl.hi = mem->raw[sp+1];
		hl.lo = mem->raw[sp];
		sp += 2;
		break;
	case 0xe2:
		// LD ($FF00+C), A
		mem->raw[bc.lo+0xff00] = af.hi;
		break;
	case 0xe5:
		// PUSH HL
		mem->raw[sp-1] = hl.hi;
		mem->raw[sp-2] = hl.lo;
		sp -= 2;
		break;
	case 0xe6:
		// AND A, n
		AND_A(args[0]);
		break;
	case 0xe7:
		// RST 0x20
		pc = 0x20;
		break;
	case 0xe8:
		// ADD SP, n
		ADD_SP(args[0]);
		break;
	case 0xe9:
		// JP (HL)
		pc = hl.val;
		break;
	case 0xea:
		// LD (nn), A
		mem->raw[wargs[0]] = af.hi;
		break;
	case 0xef:
		// RST 0x28
		pc = 0x28;
		break;
	case 0xf0:
		// LD A, ($FF00+n)
		af.hi = mem->raw[0xff00+args[0]];
		break;
	case 0xf1:
		// POP AF
		af.hi = mem->raw[sp+1];
		af.lo = mem->raw[sp];
		sp += 2;
		break;
	case 0xf2:
		// LD A, ($FF00+C)
		af.hi = mem->raw[bc.lo+0xff00];
		break;
	case 0xf3:
		// DI
		// Disable interrupts
		// TODO
		break;
	case 0xf5:
		// PUSH AF
		mem->raw[sp-1] = af.hi;
		mem->raw[sp-2] = af.lo;
		sp -= 2;
		break;
	case 0xf6:
		// OR A, n
		OR_A(args[0]);
		break;
	case 0xf7:
		// RST 0x30
		pc = 0x30;
		break;
	case 0xf8:
		/* LD HL, SP+n
		 * LDHL SP, n
		 */
		hl.val = sp+args[0];
		// Check for carry
		if (int(sp+args[0]) > hl.val) setC();
		// Check for carry in the lower half
		if (int((sp&0xf)+(args[0]&0xf)) > (hl.val&0xf)) setH();
		break;
	case 0xf9:
		// LD SP, HL
		sp = hl.val;
		break;
	case 0xfe:
		// CP A, n
		CP_A(args[0]);
		break;
	case 0xfa:
		// LD A, (nn)
		af.hi = mem->raw[wargs[0]];
		break;
	case 0xfb:
		// EI
		// Enable interrupts
		// TODO
		break;
	case 0xff:
		// RST 0x38
		pc = 0x38;
		break;
	}
	return cycles;
}

inline void LR35902::ADD_A(byte b) {
	int temp = af.hi + b;
	byte org = af.hi;
	af.hi += b;
	resetN();		   // Resets the subtract flag
	if (af.hi == 0) setZ();
	if (temp > af.hi) setC();
	if (int((org&0xf)+(b&0xf)) > (af.hi&0xf)) setH();
}

inline void LR35902::SUB_A(byte b) {
	int temp = af.hi - b;
	byte org = af.hi;
	af.hi -= b;
	setN();
	if (af.hi == 0) setZ();
	if (org < b) setC();
	if ((org&0xf) < (b&0xf)) setH();
}

inline void LR35902::AND_A(byte b) {
	af.hi &= b;
	resetN();
	setH();
	resetC();
	if (af.hi == 0) setZ();
}

inline void LR35902::OR_A(byte b) {
	af.hi |= b;
	resetN();
	resetH();
	resetC();
	if (af.hi == 0) setZ();
}

inline void LR35902::XOR_A(byte b) {
	af.hi ^= b;
	resetN();
	resetH();
	resetC();
	if (af.hi == 0) setZ();
}

inline void LR35902::CP_A(byte b) {
	int res = af.hi - b;
	if (res == 0) setZ();
	setN();
	if (af.hi < b) setC();
	if ((af.hi&0xf) < (b&0xf)) setH();
}

inline void LR35902::ADD_HL(word w) {
	int temp = hl.val + w;
	word org = hl.val;
	hl.val += w;
	resetN();		   // Resets the subtract flag
	if (temp > hl.val) setC();
	if (int((org&0xfff)+(w&0xfff)) > (hl.val&0xfff)) setH();
}

inline void LR35902::ADD_SP(byte b) {
	int temp = sp + b;
	byte org = sp;
	sp += b;
	resetN();
	resetZ();
	if (temp > sp) setC();
	else resetC();
	if (int((org&0xfff)+(b&0xfff)) > (sp&0xfff)) setH();
	else resetH();
}

inline void LR35902::SWAP(byte &b) {
	byte t = b;	 // temp
	b <<= 4;
	b |= (t >> 4);

	if (b == 0) setZ();
	resetN();
	resetH();
	resetC();
}

inline void LR35902::RLC(byte &b) {
	if (b&0x80) setC();
	else resetC();
	b <<= 1;
	if (b == 0) setZ();
	resetN();
	resetH();
}

inline void LR35902::RL(byte &b) {
	bool cvar = getFlag(CFLAG);
	if (b&0x80) setC();
	else resetC();
	b <<= 1;
	b |= cvar;
	if (b == 0) setZ();
	resetN();
	resetH();
}

inline void LR35902::RRC(byte &b) {
	if (b&0x1)  setC();
	else resetC();
	b >>= 1;
	if (b == 0) setZ();
	resetN();
	resetH();
}

inline void LR35902::RR(byte &b) {
	bool cvar = getFlag(CFLAG);
	if (af.hi&0x1)  setC();
	else resetC();
	af.hi <<= 1;
	af.hi |= cvar;
	if (af.hi == 0) setZ();
	resetN();
	resetH();
}
