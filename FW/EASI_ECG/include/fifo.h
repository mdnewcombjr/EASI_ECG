/**************************************************
 * File: fifo.h
 * Author: Jonathan Nutzmann
 * Modified By: Mark Newcomb
 * Description: Implements a queue data structure that 
     can be used as a buffer for sending and receiving data.
 *************************************************/

#ifndef __FIFO_H__
#define __FIFO_H__

#include "tistdtypes.h"

// ============= Structure Definitions ===============

typedef struct fifo_s fifo;

struct fifo_s {
	Uint8* 	buf;		// Buffer pointer.
	Uint16 	bufsize;	// Size of the buffer.
    Uint16 	front;		// Pointer to start of buffer
    Uint16 	back;		// Pointer to back of buffer
    Uint16 	count;		// Number of elements in buffer
	
	struct {
		Bool isEmpty;
		Bool isFull;
	} flags;
	
	Bool (*push)(fifo *f, Uint8 c); 	  // Push function
	Bool (*pop)(fifo *f, Uint8 *c);	  // Pop function
	Bool (*remove)(fifo *f); // Remove function

};

// =============== Prototypes ===============
/**
 * Initalizes the fifo for use.  Size and a buffer array
 * must be supplied.
 */
void fifo_initialize(fifo *f, Uint16 size, Uint8* buffer);

#endif
