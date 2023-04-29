#pragma once

#include <sstream>
#include <iostream>

const static char* PROGRAM_VERSION = "2.0.23.4.29"; // Major-Minor-Year-Month-Day

#define __PRINT_TO_VS_OUTPUT_WINDOW
#ifdef __PRINT_TO_VS_OUTPUT_WINDOW
	#include "Windows.h"
#endif


inline void __PrintToConsole( const std::stringstream& ss )
{
	std::string msg = ss.str();
	std::cout << msg;
	std::cout.flush();
#ifdef __PRINT_TO_VS_OUTPUT_WINDOW
	OutputDebugString( msg.c_str() );
#endif
}

#define PrintToConsole( s ) __PrintToConsole( std::stringstream() << s )
