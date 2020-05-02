/*

Copyright (c) 2013 Christopher A. Greeley
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#include "Assemble.h"

void Assemble( std::vector< std::string > fileNames, UINT begin )
{
    if( begin == fileNames.size() )
        return;
    std::stringstream commandBuffers[ AMOUNT_OF_TOOLS ];
    auto apply = [ & ] ( std::string dataToAppend ) {
        for( UINT i = 0; i < AMOUNT_OF_TOOLS; ++i )
            commandBuffers[ i ] << dataToAppend;
    };
    //Find the correct directory.
    apply( ReadInfo() );
    //To get the appropriate tools, and file name.//
    for( UINT i = 0; i < AMOUNT_OF_TOOLS; ++i )
        commandBuffers[ i ] << TOOLS[ i ] << " " << fileNames[ begin ];
    //To assign the proper number of bits to each toolcahin.//
    for( UINT i = 0; i < AMOUNT_OF_TOOLS; ++i )
    {
        std::stringstream end;
        end << BITS[ i ] << ".s";
        commandBuffers[ i ] << " -m" << BITS[ i ] << " -o " << ReplaceEnding( fileNames[ begin ], end.str() );
    }
    //Append the command line options.//
    apply( " -S -pthread -O0 -g -Wno-long-long -Wall -std=c++0x" );
    //Execute the commands.//
    for( UINT i = 0; i < AMOUNT_OF_TOOLS; ++i )
        system( commandBuffers[ i ].str().c_str() );
    Assemble( fileNames, ( begin + 1 ) );
}


