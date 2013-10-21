/**************************************************
 * File: fixed_buffer.h
 * Author: Mark Newcomb
 * Description: Implements a fixed-length buffer similar to 
 * a shift register.  Useful for moving averages 
 *************************************************/
 
#include "fixed_buffer.h"

Bool pushfxn(fbuff *f, Uint32 c);
Uint32 peekfxn(fbuff *f);
Bool peekatfxn(fbuff *f, Uint16 pos, Uint32 *dest);

// ========================================================

/**
 * Size and a buffer array
 * must be supplied.
 */
void fbuff_initialize(fbuff *f, Uint16 size, Uint32* buffer)
{
	int i;
	f->front = 0;
	f->back = (size-1);
	f->bufsize = size;
	f->peekpos = 0;
	f->buf = buffer;
	
	// Initalize the queue to all 0's
	for(i=0; i < f->bufsize; i++)
	{
		f->buf[i] = 0;
	}
	  
	f->push = &pushfxn;
	f->peek = &peekfxn;
	f->peekat = &peekatfxn;
}

/**
 * Pushes a character onto the shift buffer.
 */
Bool pushfxn(fbuff *f, Uint32 c)
{
	memmove(&(f->buf[1]),&(f->buf[0]),sizeof(f->buf[0])*((f->bufsize)-1));
    f->buf[0] = c;  

	return TRUE;
}

//Returns (but does not alter or remove) the next position pointed by the peek pointer
Uint32 peekfxn(fbuff *f)
{
	Uint32 temp;
	
	temp = f->buf[f->peekpos];
	
	if(f->peekpos >= f->back)
		f->peekpos = 0;
	else
		f->peekpos += 1;
	
	return temp;
}

Bool peekatfxn(fbuff *f, Uint16 pos, Uint32 *dest)
{
	if(pos > (f->back))
		return FALSE;
	
	*dest = f->buf[pos];
	return TRUE;
}

