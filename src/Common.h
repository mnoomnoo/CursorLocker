#pragma once

#include <sstream>
#include <iostream>

const static wchar_t* PROGRAM_VERSION = L"2.0.23.4.16"; // Major-Minor-Year-Month-Day

#define __PRINT_TO_VS_OUTPUT_WINDOW
#ifdef __PRINT_TO_VS_OUTPUT_WINDOW
	#include "Windows.h"
#endif


inline void __PrintToConsole( const std::wstringstream& ss )
{
	std::wcout << ss.str().c_str();
	std::wcout.flush();
#ifdef __PRINT_TO_VS_OUTPUT_WINDOW
	OutputDebugString( ss.str().c_str() );
#endif
}

#define PrintToConsole( s ) __PrintToConsole( std::wstringstream() << s )
