#ifndef EASI_RTC_H_
#define EASI_RTC_H_
#include "stdio.h"
#include "csl_general.h"
#include "csl_rtc.h"

void EASI_RTC_INIT();

Uint16 getCurrentSecond();
void wait(Uint16 mSecDelay);

#endif /*EASI_RTC_H_*/
