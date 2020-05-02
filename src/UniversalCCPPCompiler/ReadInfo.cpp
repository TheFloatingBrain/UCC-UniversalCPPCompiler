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

#include "ReadInfo.h"

std::string ReadInfo()
{
    std::string line;
    std::ifstream infoFile;
    std::string info[ 3 ];
    std::stringstream directory;
    //To obtain the information.//
    infoFile.open( "naclInfo.txt" );
    if( infoFile.is_open() )
    {
        for( UINT lines = 0; infoFile.good() && lines < 3; ++lines ) {
            getline( infoFile, line );
            info[ lines ] = line;
        }
        infoFile.close();
    }
    else { /*In case an error occures.*/
        std::cerr << "Error: Failed to open file naclInfo.txt\n";
        return "Error reading file.";
    }
    //To gain what directory the toolchain is in.//
    directory << info[ 0 ] << "/pepper_" << info[ 1 ] << "/toolchain/" << info[ 2 ] << "/";
    //Diffrent platforms have different folders that work.//
    #ifdef GNU_LINUX
        directory << "bin/";
    #endif
    //TODO: Untested.//
    #ifdef MAC_OSX
        directory << "bin/";
    #endif
    #ifdef WINDOWS
        directory << "libexec/";
    #endif
    return directory.str();
}
