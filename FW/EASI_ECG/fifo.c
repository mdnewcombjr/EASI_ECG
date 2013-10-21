/**************************************************
 * File: fifo.c
 * Author: Jonathan Nutzmann
 * Modified By: Mark Newcomb
 * Description: Implements a queue data structure that 
     can be used as a buffer for sending and receiving data.
 *************************************************/
 
#include "fifo.h"

Bool pushcharfxn(fifo *f, Uint8 c);
Bool popcharfxn(fifo *f, Uint8 *c);
Bool removecharfxn(fifo *f);

// ========================================================

/**
 * Initalizes the fifo for use.  Size and a buffer array
 * must be supplied.
 */
void fifo_initialize(fifo *f, Uint16 size, Uint8* buffer){
	int i;
	f->front = 0;
	f->back = 0;
	f->flags.isEmpty = 1;
	f->flags.isFull = 0;
	f->bufsize = size;
	
	f->buf = buffer;
	
	// Initalize the queue to all 0's
	for(i=0; i <= f->bufsize; i++)
	{
		f->buf[i] = 0;
	}
	  
	f->count = 0;
	f->push = &pushcharfxn;
	f->pop = &popcharfxn;
	f->remove = &removecharfxn;
}

/**
 * Pushes a character onto the queue.
 */
Bool pushcharfxn(fifo *f, Uint8 c)
{
	if (f->count >= f->bufsize)
	{
		f->flags.isFull = 1;
		return FALSE;
	}		
	else 
	{
    f->back = f->back + 1;
		if(f->back >= f->bufsize) f->back = 0;
    f->buf[f->back] = c;    
    f->count++;
		f->flags.isEmpty = 0;
	}

  if (f->count >= f->bufsize)
  {
		f->flags.isFull = 1;
  }

	return TRUE;
}

/**
 * Removes the last character pushed.
 */
Bool removecharfxn(fifo *f)
{
    if (f->count <= 0)
	{
		f->flags.isEmpty = TRUE;
		return FALSE;
    }
	else 
	{
		if(--(f->back) < 0) f->back = f->bufsize - 1;
        f->count--;
		f->flags.isFull = 0;
	}

	if (f->count <= 0) {
		f->flags.isEmpty = 1;
	}

	return TRUE;
}

/**
 * Pops a character off of the queue.
 */
Bool popcharfxn(fifo *f, Uint8 *c)
{
    if (f->count <= 0)
	{
		f->flags.isEmpty = TRUE;
		return FALSE;
    }
	else 
	{
    f->front = f->front + 1;
		if(f->front >= f->bufsize) f->front = 0;
		*c = f->buf[f->front];
		f->buf[f->front] = '\0';
    f->count--;
		f->flags.isFull = 0;
	}

	if (f->count <= 0) {
		f->flags.isEmpty = 1;
	}

	return TRUE;
}
