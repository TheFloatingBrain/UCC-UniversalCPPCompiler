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

#include "UnPackage.h"
/*A file will be placed in each "Package" will have a
fi.pi ( File Information . Package Information ) file inside it containting the names of each file.*/
std::vector< std::string > UnPackage( std::vector< std::string > packagedPackages )
{
    std::vector< std::string > assemblyFiles;
    for( UINT i = 0; i < packagedPackages.size(); ++i )
    {
        std::ifstream fi;
        std::stringstream fileInfo;
        fileInfo << packagedPackages[ i ] << "/fi.pi";
        fi.open( fileInfo.str().c_str() );
        if( fi.good() == true )
        {
            while( fi.eof() == false )
            {
                std::stringstream file;
                std::string fileName;
                std::getline( fi, fileName );
                if( fileName.size() >= 1 ) {
                    file << packagedPackages[ i ] << "/" << fileName;
                    assemblyFiles.push_back( file.str() );
                }
            }
        }
        else
            std::cerr << "Error, could not open file package.\n";
        fi.close();
    }
    return assemblyFiles;
}
