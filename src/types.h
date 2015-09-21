#ifndef TYPES_H_
#define TYPES_H_

/* Some useful definitions */

typedef unsigned char byte;
typedef unsigned short word;

union word_t {
	word val;
	struct {
		#ifndef BIG_ENDIAN
		byte hi, lo;		// Little endian
		#else
		byte lo, hi;		// Big endian
		#endif
	};
};

#endif
