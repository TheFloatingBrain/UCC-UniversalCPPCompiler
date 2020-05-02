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

#include "ReadArguments.h"

void ReadArguments( std::vector< std::string > arguments )
{
    //Establish some constants.//
    const std::vector< std::string > FLAGS{
        std::string{ "-nacl:" }, std::string{ "-p:" }, std::string{ "-up:" },
        std::string{ "-toolchain:" }, std::string{ "-f:" }, std::string{ "-nonacl" },
        std::string{ "-noweb" }, std::string{ "-noucpp" }, std::string{ "-o:" }
     };
    std::stringstream nacl;
    std::stringstream toolchain;
    //To buffer the files from packages later.//
    std::vector< std::string > unPackagedFiles;
    //To store all the files later.//
    std::vector< std::string > files;
    //So all the commands can be in one place.//
    std::vector< std::vector< std::string > > commandStrings[ 9 ];
    //For speed.//
    CUINT SIZE = arguments.size();
    //Go through all the command line arguments.//
    for( UINT i = 0; i < SIZE; ++i )
    {
        //Go through all the flags.//
        for( UINT j = 0; j < 9; ++j )
        {
            //Check if the flag is at the begning of the argument.//
            if( arguments[ i ].substr( 0, FLAGS[ j ].size() ) == FLAGS[ j ] )
            {
                //Get everything that may be in that command.//
                std::vector< std::string > commandBuffer;
                //Allocate everything but the flag, if the parameters allow it.//
                if( ( arguments[ i ].size() - FLAGS[ j ].size() ) < arguments[ i ].size() )
                    commandBuffer.push_back( arguments[ i ].substr( FLAGS[ j ].size(), ( arguments[ i ].size() - FLAGS[ j ].size() ) ) );
                else
                    commandBuffer.push_back( arguments[ i ] );
                for( UINT o = ( i + 1 ); o < SIZE; ++o )
                {
                    bool flagFound = false;
                    for( UINT k = 0; k < FLAGS.size(); ++k )
                    {
                        /*All the parameters for an argument are considered to be everything until
                        the next argument or th end of the end of all the command.*/
                        if( arguments[ o ].substr( 0, FLAGS[ k ].size() ).compare( FLAGS[ k ] ) == 0 ) {
                            flagFound = true;
                            break;
                        }
                    }
                    if( flagFound == false )
                        commandBuffer.push_back( arguments[ o ] );
                    else
                        break;
                }
                //j < MAX FLAG threrfore j < commandStrings.size(), allocate the string to the vector.//
                commandStrings[ j ].push_back( commandBuffer );
            }
        }
    }
    //Congerate all the Naticve Client arguments together.//
    for( UINT i = 0; i < commandStrings[ NACL ].size(); ++i ) {
        for( UINT j = 0; j < commandStrings[ NACL ][ i ].size(); ++j ) {
            nacl << commandStrings[ NACL ][ i ][ j ] << " ";
        }
    }
    //Congerate all the *other* tool chain arguments together.//
    for( UINT i = 0; i < commandStrings[TOOL_CHAIN ].size(); ++i ) {
        for( UINT j = 0; j < commandStrings[ TOOL_CHAIN ][ i ].size(); ++j ) {
            toolchain << commandStrings[ TOOL_CHAIN ][ i ][ j ] << " ";
        }
    }
    //Get everything from the packages.//
    for( UINT i = 0; i < commandStrings[ UP ].size(); ++i )
        unPackagedFiles = UnPackage( commandStrings[ UP ][ i ] );
    for( UINT i = 0; i < unPackagedFiles.size(); ++i )
        files.push_back( unPackagedFiles[ i ] );
    //Get all the files.//
    for( UINT i = 0; i < commandStrings[ F ].size(); ++i ) {
        for( UINT j = 0; j < commandStrings[ F ][ i ].size(); ++j )
            files.push_back( commandStrings[ F ][ i ][ j ] );
    }
    //Compile.//
    if( commandStrings[ OUTPUT ].size() != 0 || commandStrings[ P ].size() )
    {
        Compile( ( ( commandStrings[ NO_NACL ].size() <= 0 ) ? ( true ) : ( false ) ), ( ( commandStrings[ P ].size() > 0 ) ? ( true ) : ( false ) ), ( ( commandStrings[ NO_UCPP ].size() <= 0 ) ? ( true ) : ( false ) ),
            ( ( commandStrings[ NO_WEB ].size() <= 0 ) ? ( true ) : ( false ) ), nacl.str(), toolchain.str(), files, ( commandStrings[ P ].size() <= 0 ) ? commandStrings
            [ OUTPUT ][ ( commandStrings[ OUTPUT ].size() - 1 ) ][ ( commandStrings[ OUTPUT ][ ( commandStrings[ OUTPUT ].size() - 1 ) ].size() - 1 ) ] : commandStrings
            [ P ][ ( commandStrings[ P ].size() - 1 ) ][ ( commandStrings[ P ][ ( commandStrings[ P ].size() - 1 ) ].size() - 1 ) ] );
    }
    else {
        Compile( ( ( commandStrings[ NO_NACL ].size() <= 0 ) ? ( true ) : ( false ) ), ( ( commandStrings[ P ].size() > 0 ) ? ( true ) : ( false ) ), ( ( commandStrings[ NO_UCPP ].size() <= 0 ) ? ( true ) : ( false ) ),
            ( ( commandStrings[ NO_WEB ].size() <= 0 ) ? ( true ) : ( false ) ), nacl.str(), toolchain.str(), files );
    }
}
