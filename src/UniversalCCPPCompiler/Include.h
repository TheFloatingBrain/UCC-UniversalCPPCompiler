/*
Copyright (C) 2013 Christopher A. Greeley

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef INCLUDE_H_INCLUDED
#define INCLUDE_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

typedef unsigned int UINT;
typedef const unsigned int CUINT;

#define GNU_LINUX
//#define WINDOWS
//#define MAC_OSX


//To establish some constants, so mistakes are less likly to occur.//
#ifdef GNU_LINUX
    #define AMOUNT_OF_TOOLS 2
    const std::string TOOLS[ AMOUNT_OF_TOOLS ] = { "x86_64-nacl-g++", "i686-nacl-g++" };
    const std::string BITS[ AMOUNT_OF_TOOLS ] = { "64", "32" };
    const std::string INTERNAL_EXEC_CONVENTION( ".gnulinux" );
#endif
#ifdef WINDOWS
    #define AMOUNT_OF_TOOLS 1
    const std::string TOOLS[ AMOUNT_OF_TOOLS ] = { "x86_64-nacl-g++.exe", "x86_64-nacl-g++.exe" };
    const std::string BITS[ AMOUNT_OF_TOOLS ] = { "64", "32" };
    const std::string INTERNAL_EXEC_CONVENTION( ".exe" );
#endif
//TODO: Untested.//
#ifdef MAC_OSX
    #define AMOUNT_OF_TOOLS 2
    const std::string TOOLS[ AMOUNT_OF_TOOLS ] = { "x86_64-nacl-g++", "i686-nacl-g++" };
    const std::string BITS[ AMOUNT_OF_TOOLS ] = { "64", "32" };
    const std::string INTERNAL_EXEC_CONVENTION( ".app" );
#endif

CUINT NACL = 0;
CUINT P = 1;
CUINT UP = 2;
CUINT TOOL_CHAIN = 3;
CUINT F = 4;
CUINT NO_NACL = 5;
CUINT NO_WEB = 6;
CUINT NO_UCPP = 7;
CUINT OUTPUT = 8;

#endif // INCLUDE_H_INCLUDED
