#pragma once

#include <sstream>
#include <iostream>

const static wchar_t* PROGRAM_VERSION = L"2.0.23.4.26"; // Major-Minor-Year-Month-Day

#define __PRINT_TO_VS_OUTPUT_WINDOW
#ifdef __PRINT_TO_VS_OUTPUT_WINDOW
	#include "Windows.h"
#endif


inline void __PrintToConsole( const std::stringstream& ss )
{
	const char* msg = ss.str().c_str();
	std::cout << msg;
	std::cout.flush();
#ifdef __PRINT_TO_VS_OUTPUT_WINDOW
	OutputDebugString( msg );
#endif
}

#define PrintToConsole( s ) __PrintToConsole( std::stringstream() << s )
