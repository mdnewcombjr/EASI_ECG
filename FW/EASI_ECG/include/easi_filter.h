#ifndef EASI_FILTER_H_
#define EASI_FILTER_H_

#include "tistdtypes.h"
#include "Dsplib.h"
#include "TMS320.h"

#define ENABLE_FIR_FILTER 0

void EASI_RUN_FILTERLOOP(Int32* inbuf, short sizein, Int32* outbuf, short sizeout,short channel);

#endif /*EASI_FILTER_H_*/
