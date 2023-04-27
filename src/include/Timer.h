#ifndef TIMER_H
#define TIMER_H


#include <cstdint>

typedef uint64_t TimeStamp;

TimeStamp
GetTimeStamp();

uint64_t
GetElapsedMilliseconds(
	TimeStamp startTime,
	TimeStamp endTime
);

uint64_t
GetElapsedMicroseconds(
	TimeStamp startTime,
	TimeStamp endTime
);


#endif // TIMER_H
