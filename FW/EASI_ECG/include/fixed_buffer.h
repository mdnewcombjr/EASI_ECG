/**************************************************
 * File: fixed_buffer.h
 * Author: Mark Newcomb
 * Description: Implements a fixed-length buffer similar to 
 * a shift register.  Useful for moving averages 
 *************************************************/
 
#ifndef FIXED_BUFFER_H_
#define FIXED_BUFFER_H_

#include "string.h"
#include "tistdtypes.h"

// ============= Structure Definitions ===============

typedef struct fbuff_s fbuff;

struct fbuff_s {
	Uint32* 	buf;		// Buffer pointer.
	Uint16 	bufsize;	// Size of the buffer.
    Uint16 	front;		// Pointer to start of buffer
    Uint16 	back;		// Pointer to back of buffer
	Uint16 peekpos;	//Pointer to the peek position

	
	Bool (*push)(fbuff *f, Uint32 c); 	  // Push function
	Uint32 (*peek)(fbuff *f); //Peek function
	
	//Peek at a specific location in the buffer.  Index starting at 0 
	Bool (*peekat)(fbuff *f, Uint16 position, Uint32 *dest);


};

// =============== Prototypes ===============
/**
 * Initalizes the fixed-length buffer for use.  Size and a buffer array
 * must be supplied.
 */
void fbuff_initialize(fbuff *f, Uint16 size, Uint32* buffer);


#endif /*FIXED_BUFFER_H_*/
