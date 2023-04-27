
#include "Timer.h"

// std includes
#include <chrono>

TimeStamp
GetTimeStamp()
{
	return TimeStamp( 
		std::chrono::duration_cast<std::chrono::nanoseconds>( 
			std::chrono::steady_clock::now().time_since_epoch()
		).count()
	);
}

uint64_t
GetElapsedMilliseconds(
	TimeStamp startTime,
	TimeStamp endTime
)
{
	// 0.000001

	const uint64_t nanoSecDiff = endTime - startTime;

	return nanoSecDiff / 1000000;
}

uint64_t
GetElapsedMicroseconds(
	TimeStamp startTime,
	TimeStamp endTime
)
{
	// 0.001

	const uint64_t nanoSecDiff = endTime - startTime;

	return nanoSecDiff / 1000;
}