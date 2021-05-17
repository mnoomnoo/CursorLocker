#pragma once

#include <sstream>
#include <iostream>

const static wchar_t* PROGRAM_VERSION = L"1.3.21.5.10"; // Major-Minor-Year-Month-Day

static void __PrintToConsole( const std::wstringstream& ss )
{
	std::wcout << ss.str().c_str();
	std::wcout.flush();
}

#define PrintToConsole( s ) __PrintToConsole( std::wstringstream() << s )
