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
#define ZFLAG   0x80
#define NFLAG   0x40
#define HFLAG   0x20
#define CFLAG   0x10

union word_w {
    word val;
    struct {
        #ifndef BIG_ENDIAN
        byte hi, lo;        // Little endian
        #else
        byte lo, hi;        // Big endian
        #endif
    };
};

typedef word_w Register;

class LR35902 {
    private:
        // A pointer to the memory
        Memory *mem;
        // Some registers
        Register af;        // A and F
        Register bc;        // B and C
        Register de;        // D and E
        Register hl;        // H and L
        word sp;            // Stack pointer that grows downwards in memory
        word pc;            // Program counter (instruction pointer)

        // Some flag setters and resetters
        inline void setZ() {af.lo |= ZFLAG;}    // Sets the zero flag
        inline void setN() {af.lo |= NFLAG;}    // Sets the subtract flag
        inline void setH() {af.lo |= HFLAG;}    // Sets the half carry flag
        inline void setC() {af.lo |= CFLAG;}    // Sets the carry flag
        inline void resetZ() {af.lo &= ~ZFLAG;} // Resets the zero flag
        inline void resetN() {af.lo &= ~NFLAG;} // Resets the subtract flag
        inline void resetH() {af.lo &= ~HFLAG;} // Resets the half carry flag
        inline void resetC() {af.lo &= ~CFLAG;} // Resets the carry flag
        inline bool getFlag(byte mask) {return ((af.lo & mask) != 0? true:false);}

        // Some complex operations that need to
        // be performed quite a lot of times
        inline void ADD_A(byte);
        inline void ADD_HL(word);
        inline void ADD_SP(byte);
        inline void SUB_A(byte);
        inline void AND_A(byte);
        inline void OR_A(byte);
        inline void XOR_A(byte);
        inline void CP_A(byte);

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
{  4, 12,  8,  8,  4,  4,  8,  4, 20,  8,  8,  8,  4,  4,  8,  4,  // 0
   4, 12,  8,  8,  4,  4,  8,  4, 12,  8,  8,  8,  4,  4,  8,  4,  // 1
   0, 12,  8,  8,  4,  4,  8,  4,  0,  8,  8,  8,  4,  4,  8,  4,  // 2
   0, 12,  8,  8, 12, 12, 12,  4,  0,  8,  8,  8,  4,  4,  8,  4,  // 3
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 4
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 5
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 6
   8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4,  // 7
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 8
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 9
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // a
   4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // b
   0, 12, 12, 16,  0, 16,  8, 16,  0, 16, 12,  4,  0, 24,  8, 16,  // c
   0, 12, 12,  0,  0, 16,  8, 16,  0, 16, 12,  0,  0,  0,  8, 16,  // d
  12, 12,  8,  0,  0, 16,  8, 16, 16,  4, 16,  0,  0,  0,  8, 16,  // e
  12, 12,  8,  4,  0, 16,  8, 16, 12,  8, 16,  4,  0,  0,  8, 16   // f
};

/* A very long list of the different sizes of different opcodes.
 * The entries with 0 in them mean opcodes that don't exist.
 */
static int LR35902_opsizes[] =
// 0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
{  1,  3,  1,  1,  1,  1,  2,  1,  3,  1,  1,  1,  1,  1,  2,  1,  // 0
   2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,  // 1
   2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,  // 2
   2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,  // 3
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // a
   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // b
   1,  1,  3,  3,  3,  1,  2,  1,  1,  1,  3,  1,  3,  3,  2,  1,  // c
   1,  1,  3,  0,  3,  1,  2,  1,  1,  1,  3,  0,  3,  0,  2,  1,  // d
   2,  1,  2,  0,  0,  1,  2,  1,  2,  1,  3,  0,  0,  0,  2,  1,  // e
   2,  1,  2,  1,  0,  1,  2,  1,  2,  1,  3,  1,  0,  0,  2,  1   // f
};

#endif
