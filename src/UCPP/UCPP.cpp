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

//Modified, non - C++0x version.//
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <ctype.h>
#include <stdio.h>
#include <sstream>
#include <locale>
#include <cstddef>
#include <typeinfo>
#include <stdlib.h>
#include <utility>
#include <map>
#include <fstream>

typedef std::vector< std::string > SVECT;
typedef unsigned int UINT;
typedef unsigned short int SUINT;

//TODO: Fix initilizer lists.//
namespace UCC
{
    bool compilerFailure = false;
    namespace Process
    {
        const std::string UCPP_ERROR = std::string{ "! UCPP Compiler Error:" };
        const std::string UCC_INTERNAL_ERROR = std::string{ "! UCPP Internal Error (Contact a Developer):" };
        const std::string UNDEFINED_DATA = std::string{ "$`" };
        /*NAME: It is the name of a varible, class, struct, function ect.
        KEY_OP: Keyword or operator.*/
        enum RAW_DATA_TYPE {
            NAME = 0,
            KEY_OP = 1
        };
        enum OPERATOR_CONDITIONAL
        {
            PART_OF_OP = 0,
            IS_OP = 1,
            NO_OP = 2
        };
        enum SCOPE
        {
            NONE = 0,
            CLASS = 1,
            STRING_LITERAL = 2
        };
        enum DECL_SCOPE
        {
            CLASS_DECL = 0,
            BLOCK_DECL = 1,
            OTHER_DECL = 2,
        	DECL_ERROR = 3
		};
        enum SCOPE_INFORMATION
        {
            BEFORE_SCOPE = 0,
            OUT_OF_SCOPE = 1,
            NORMAL = 2
        };
        namespace Debug
        {
            namespace StringParsing {
                std::string ParseToString( SCOPE_INFORMATION scopeInformation );
            }
        }
        struct Data
        {
            Data() {}
            Data( std::string parsedData_ , UINT originalLocation_ ) {
                parsedData = parsedData_;
                originalLocation = originalLocation_;
            }
            std::string parsedData;
            UINT originalLocation;
            std::string ParseToString()
            {
                std::stringstream construct;
                construct << parsedData << ":" << originalLocation;
                return construct.str();
            }
        };
        struct RawData : public Data
        {
            RAW_DATA_TYPE rawDataType;
            RawData& operator = ( Data &operhand )
            {
                originalLocation = operhand.originalLocation;
                parsedData = operhand.parsedData;
                return ( * this );
            }
			friend std::ostream& operator << ( std::ostream&, UCC::Process::RawData );
        };
		std::ostream& operator << ( std::ostream &out, UCC::Process::RawData data ) {
				out << data.ParseToString();
				return out;
		}
      	typedef std::pair< RawData, RawData > DOUBLE_RAW_DATA;
        namespace TypeClarity {
            RawData DataToRawData( UCC::Process::Data data, UCC::Process::RAW_DATA_TYPE dataType );
        }
        namespace Globals
        {
            std::string code;
            SVECT wordsToParse;
            SVECT operators;
            std::vector< UCC::Process::RawData > mergedData;
            std::vector< UCC::Process::Data > parsedCustomNames;
            std::vector< UCC::Process::Data > parsedCodeChunks;
			UINT location = 1;
        }
        namespace Algorithm
        {
            namespace Conventional
			{
                void Order( std::vector< UCC::Process::Data > *toSort );
				void Order( std::vector< UCC::Process::RawData > *toSort );
                void MergeSort();
            }
        }
        namespace ScopeAnylsis
        {
            const UINT UNDEFINED = 0;
            template< typename T >
            UINT GetScope( std::string openingSymbol, std::string closingSymbol, T symbolePosition,
               UINT from = UNDEFINED, UINT to = UCC::Process::Globals::mergedData[ ( UCC::Process::Globals::mergedData.size() - 1 ) ].originalLocation );
        }
        struct ScopeData
        {
            UINT scope;
            std::string symbol;
            SCOPE_INFORMATION scopeInformation;
            std::string ParseToString()
            {
                std::stringstream construct;
                construct << symbol << ":" << scope << " : " <<
                    UCC::Process::Debug::StringParsing::ParseToString( scopeInformation );
                return construct.str();
            }
        };
        namespace Parseing
        {
            void Parse();
            //Return true if the word you found is not what it seems.//
            bool NotWord( UINT begin, UINT end, std::string canidate );
            //To help check if a keyword is next to an operator.//
            OPERATOR_CONDITIONAL HasOperator( UINT canidateLocation );
        }
		namespace AttributeAnylsis
		{
			//Determin if the data is static.//
			bool IsStatic( RawData data );
			//Determin if somthing was acessed using the "this" keyword.//
			bool UsedThis( RawData data );
          	//Determin if the data is a part of a control statment.//
          	bool IsAPartOfControl( RawData data );
		}
        namespace ScopeAnylsis
        {
            /*Determins weather or not a varible or borrow-allocatd
            varible is in a class.*/
            bool IsInClass( RawData data, RawData opening, RawData closing, RawData last );
            //Determin if the data is in say a function, if statement, ect and not an array of initializer list.//
            bool IsInBlock( RawData data, RawData opening, RawData closing, RawData last );
			//Detrimin if the data is in a function.//
			inline bool IsInFunction( RawData data, RawData opening, RawData closing, RawData last );
			//Detrimin if the data is in a array.//
			bool IsInArray( RawData data, RawData opening, RawData closing, RawData last );
          	//Get the two charechters that mark the scope of data.//
          	DOUBLE_RAW_DATA GetScopeMarkers( UCC::Process::RawData relitiveTo, std::string openingMarker, 
                                              std::string closingMaker );
			//Find the end of the scope.//
			int GetEndOfScope( UCC::Process::RawData scopeBegin, std::string closingSymbol );
			//Determin if somthing is a part of a string/char.//
			bool IsInString( UCC::Process::RawData canidate );
        }
		namespace Algorithm
		{
			namespace Search
			{
				const int NONE = ( - 1 );
				/*Find the previouse line relitive to a pice of data 
				(returns the last pice of data from that line).*/
				RawData GetLastLine( RawData from );
				//The next three functions return 0, or "NONE", if no charechter was found.//
				//Returns the index of the next occurence of a charechter relitive to a point.//
				int GetNext( Data *fromWhere, std::string symbol );
				//Returns the index of the next last of a charechter relitive to a point.//
				int GetLast( Data *fromWhere, std::string symbol );
				//Finds the index of the data.//
				int FindDataIndex( Data *toBeFound );
				//Finds where the varible was declared.//
				DECL_SCOPE GetDeclorationType( RawData declared );
				//Finds the beginig of the last class/struct decloration.//
				RawData GetLastClassDecloration( RawData relitiveTo );
			}
		}
    }
	namespace UCPP
	{
		namespace Borrow
		{
			const std::string UCC_BLOCK_POINTER = std::string{ "UCC_BLOCK_POINTER_" };
			const std::string UCC_CLASS_POINTER = std::string{ "UCC_CLASS_POINTER_" };
			const std::string BORROW = std::string{ "borrow" };
			const std::string NEW = std::string{ "new" };
			const std::string AUTO = std::string{ "auto" };
			const std::string DELETE = std::string{ "delete" };
			//Insert the compiled data into a "block" (function, if, array, else, while, for, do, ect.).//
            std::string InsertIntoBlock( UCC::Process::RawData toInsert, UINT end );
			//Insert the compiled data into a class.//
			std::string InsertIntoClass( UCC::Process::RawData toInsert, UINT end );
			//Insert the compiled data.//
			std::string Insert( UCC::Process::RawData toInsert, UCC::Process::DECL_SCOPE scopeType );
			//Genereate and insert the compiled data to delete the value of the pointers in a class.//
			void AddDeleteClass( UCC::Process::RawData toDelete, std::string pointerName );
			//Genereate and insert the compiled data to delete the value of the pointers in a "block".//
			void AddDeleteBlock( UCC::Process::RawData toDelete, std::string pointerName );
			//Genereate and insert the compiled data to delete the value of the pointers.//
			void Delete( UCC::Process::RawData toDelete, std::string pointerName, UCC::Process::DECL_SCOPE scopeType );
			//Generate the compiled data.//
			inline void GenerateBorrow( UCC::Process::RawData toGenerate, UINT end );
			//Gather the pre - requisit data then generate the compiled data.
          	void DoBorrow();
		}
      	namespace Repl
		{
			//Find where to insert the compiled code for repl().//
        	void DoRepl();
			//Insert the compiled repl() data.//
          	void InsertRepl( UCC::Process::RawData replLocation );
        }
	}
    /* : TODO:
        * : Comment interfaces.
        * : Fill these in.
    */
    namespace Formatting
	{
		void FormatPreProcessor();
        void FitConditionals();
		void DeleteComments();
        std::string FormatForWrite();
		void DeleteNewLines();
    }
}
std::string ParseToDeclString( UCC::Process::DECL_SCOPE declScope )
{
	std::string output;
	switch( ( UINT ) declScope )
	{
		case 0: {
			output = "CLASS_DECL";
			break;
		}
		case 1: {
			output = "BLOCK_DECL";
			break;
		}
		case 2: {
			output = "OTHER_DECL";
			break;
		}
		case 3: {
			output = "DECL_ERROR";
			break;
		}
		default: {
			break;
		}
	}
	return output;
}
int main( int argc, char **args )
{
	if( argc >= 1 )
	{
		std::ifstream fileBuffer;
		std::stringstream codeBuffer;
		std::ofstream write;
		std::string outFileName( args[ 1 ] );
		const std::string CPP_EXTENSION = std::string{ ".cpp" };
		UINT eliminateFrom = outFileName.size();
		for( UINT i = 0; i < outFileName.size(); ++i ) {
			if( outFileName[ i ] == '.' )
				eliminateFrom = i;
		}
		for( UINT i = ( outFileName.size() - eliminateFrom ); i > 0; --i )
			outFileName.erase( ( outFileName.end() - 1 ) );
		for( UINT i = 0; i < 4; ++i )
			outFileName.push_back( CPP_EXTENSION[ i ] );
		fileBuffer.open( args[ 1 ] );
		if( fileBuffer.is_open() == true )
		{
			while( fileBuffer.good() == true && fileBuffer.eof() != true )
			{
				std::string line;
				std::getline( fileBuffer, line );
				codeBuffer << line << "\n";
			}
			fileBuffer.close();
		}
		else
			std::cerr << UCC::Process::UCPP_ERROR << "The file you have requested to compile is non - existant.\n";
		UCC::Process::Globals::operators = SVECT{ 
			/*"not", "not_eq", "or", "or_eq", "and", "and_eq", "xor", "xor_eq", 
			"const_cast", "dynamic_cast", "static_cast",*/ "<", ">", "(", "}", ")", "\"", ";", 
			"{", "'", "=", ",", "[", "]", ":", ".", "\n", "~", "+", "-", "*", "/", "\n", "#", 
			"%", "!", "&", "|", "^", "?"
		 };
		UCC::Process::Globals::wordsToParse = SVECT{ 
			"return"
			//"main", "return", "int",
			/*"private", "public", "class", "borrow", "static", "this", "struct", 
			"namespace", "long", "short", "while", "do", "for", "if", "else", 
			"alignas", "alignof",  "asm", "auto", "bitand", "bitor", "bool", 
			"break", "case", "catch", "char", "char16_t", "char32_t", "compl", "const", 
			"constexpr", "double", "continue", "decltype", "default", "delete", "enum", 
			"explicit", "export", "repl", "extern", "false", "float", "friend", "goto", 
			"inline", "mutable", "new", "noexcept", "nullptr", "operator", 
			"protected", "register", "reinterpret_cast", 
			"signed", "sizeof", "static_assert",  
			"template", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", 
			"union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", 
			"override", "final"*/
		 };
		UCC::Process::Globals::code = codeBuffer.str();
		UCC::Process::Parseing::Parse();
		UCC::Process::Algorithm::Conventional::MergeSort();
		UCC::Formatting::FormatPreProcessor();
		UCC::Formatting::FitConditionals();
		UCC::Formatting::DeleteComments();
		UCC::Formatting::DeleteNewLines();
		UCC::Process::Globals::location = ( UCC::Process::Globals::mergedData[ ( UCC::Process::Globals::mergedData.size() - 1 ) ].originalLocation + 1 );
		if( argc >= 2 ) {
			std::cerr << "\n++++++++++++++++++++++++++++++++++\n";
			std::cerr << "+     Compiling please wait.     +\n";
		}
		UCC::UCPP::Repl::DoRepl();
		UCC::UCPP::Borrow::DoBorrow();
		if( UCC::compilerFailure == false && argc >= 2 )
			std::cerr << "+ Success: compilation complete. +\n";
		else if( UCC::compilerFailure == false && argc < 2 )
			std::cerr << "Success: compilation complete.\n";
		else if( UCC::compilerFailure == true && argc >= 2 )
			std::cerr << "+ !      Compiler failure.       +\n";
		else if( UCC::compilerFailure == true && argc < 2 )
			std::cerr << "Compiler failure.\n";
		if( argc >= 2 )
			std::cerr << "++++++++++++++++++++++++++++++++++\n";
		const std::string CODE_TO_WRITE = UCC::Formatting::FormatForWrite();
		write.open( outFileName.c_str() );
		for( UINT i = 0; i < CODE_TO_WRITE.size(); ++i )
			write << CODE_TO_WRITE[ i ];
		write.close();
	}
	else
		std::cerr << UCC::Process::UCPP_ERROR << "No file to compile.\n";
	return 0;
}
namespace UCC
{
    namespace Process
    {
        namespace TypeClarity
        {
            RawData DataToRawData( UCC::Process::Data data, UCC::Process::RAW_DATA_TYPE dataType )
            {
                UCC::Process::RawData rawData;
                rawData.operator =( data );
                rawData.rawDataType = dataType;
                return rawData;
            }
        }
        namespace Algorithm
        {
            namespace Conventional
            {
                void Order( std::vector< UCC::Process::Data > *toSort )
                {
                    const UINT SIZE = toSort->size();
                    if( SIZE > 0 )
                    {
                        for( UINT i = 0; i < SIZE; ++i )
                        {
                            for( UINT j = 0; j < SIZE; ++j )
                            {
                                if( j != i )
                                {
                                    if( i < j )
                                    {
                                        if( toSort->at( i ).originalLocation > toSort->at( j ).originalLocation )
                                        {
                                            UCC::Process::Data lower = toSort->at( i );
                                            toSort->at( i ) = toSort->at( j );
                                            toSort->at( j ) = lower;
                                        }
                                    }
                                    else if( toSort->at( i ).originalLocation < toSort->at( j ).originalLocation )
                                    {
                                        UCC::Process::Data lower = toSort->at( j );
                                        toSort->at( j ) = toSort->at( i );
                                        toSort->at( i ) = lower;
                                    }
                                }
                            }
                        }
                    }
                }
				void Order( std::vector< UCC::Process::RawData > *toSort )
                {
                    const UINT SIZE = toSort->size();
                    if( SIZE > 0 )
                    {
                        for( UINT i = 0; i < SIZE; ++i )
                        {
                            for( UINT j = 0; j < SIZE; ++j )
                            {
                                if( j != i )
                                {
                                    if( i < j )
                                    {
                                        if( toSort->at( i ).originalLocation > toSort->at( j ).originalLocation )
                                        {
                                            UCC::Process::Data lower = toSort->at( i );
                                            toSort->at( i ) = toSort->at( j );
                                            toSort->at( j ) = lower;
                                        }
                                    }
                                    else if( toSort->at( i ).originalLocation < toSort->at( j ).originalLocation )
                                    {
                                        UCC::Process::Data lower = toSort->at( j );
                                        toSort->at( j ) = toSort->at( i );
                                        toSort->at( i ) = lower;
                                    }
                                }
                            }
                        }
                    }
                }
                void MergeSort()
                {
                    const UINT PARSED_CODE_CHUNKS_SIZE = UCC::Process::Globals::parsedCodeChunks.size();
                    std::vector< UCC::Process::Data > customNames = UCC::Process::Globals::parsedCustomNames;
                    //T( ( ( ( t * n ^ 2 ) + ( b * n ^ 2 ) ) + n ) )//
                    Order( &UCC::Process::Globals::parsedCodeChunks );
                    Order( &customNames );
                    for( UINT i = 0; i < PARSED_CODE_CHUNKS_SIZE; ++i )
                    {
                        //The size changes and must be re-evaluated.//
                        const UINT PARSED_CUSTOM_NAMES_SIZE = customNames.size();
                        for( UINT j = 0; j < PARSED_CUSTOM_NAMES_SIZE; ++j )
                        {
                            if( UCC::Process::Globals::parsedCodeChunks[ i ].originalLocation > customNames[ j ].originalLocation )
                            {
                                if( ( i + 1 ) < PARSED_CODE_CHUNKS_SIZE )
                                {
                                    if( UCC::Process::Globals::parsedCodeChunks[ ( i + 1 ) ].originalLocation > customNames[ j ].originalLocation )
                                    {
                                        UCC::Process::Globals::mergedData.push_back(
                                            UCC::Process::TypeClarity::DataToRawData( customNames[ j ], NAME ) );
                                        customNames.erase( customNames.begin() + j );
                                    }
                                }
                                else {
                                    UCC::Process::Globals::mergedData.push_back(
                                        UCC::Process::TypeClarity::DataToRawData( customNames[ j ], NAME ) );
                                    customNames.erase( customNames.begin() + j );
                                }
                            }
                        }
                        //All pre-requisits should be in order, add it to the vector.//
                        UCC::Process::Globals::mergedData.push_back(
                            UCC::Process::TypeClarity::DataToRawData( UCC::Process::Globals::parsedCodeChunks[ i ], KEY_OP ) );
                    }
					Order( &UCC::Process::Globals::mergedData );
				}
			}
            namespace Search
			{
				void Debug_( std::string msg = "" )
				{
					static long int dbg = 0;
					++dbg;
					std::cerr << "ASD:" << msg << ":" << dbg << "\n";
				}
				int FindDataIndex( Data *toBeFound )
				{
					const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
					if( MERGED_DATA_SIZE > 0 )
					{
						for( UINT i = 0; i < MERGED_DATA_SIZE; ++i ) {
							if( UCC::Process::Globals::mergedData[ i ].originalLocation == toBeFound->originalLocation )
								return ( int ) i;
						}
					}
					return NONE;
				}
				int GetNext( Data *fromWhere, std::string symbol )
				{
					const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
					if( MERGED_DATA_SIZE > 0 )
                    {
						for( UINT i = ( FindDataIndex( fromWhere ) + 1 ); i < MERGED_DATA_SIZE; ++i ) {
							if( UCC::Process::Globals::mergedData[ i ].parsedData == symbol )
								return i;
						}
					}
                    return ( NONE );
				}
				int GetLast( Data *fromWhere, std::string symbol )
				{
					if( UCC::Process::Globals::mergedData.size() > 0 )
					{
						const int START = ( FindDataIndex( fromWhere ) - 1 );
						if( START >= 0 )
						{
                            for( UINT i = ( UINT ) START; i > 0; --i ) {
								if( UCC::Process::Globals::mergedData[ i ].parsedData == symbol )
									return i;
							}
						}
					}
					return NONE;
				}
				void Debug()
				{
					static int dbg = 0;
					++dbg;
					std::cerr << "ASD:" << dbg << "\n";
				}
				RawData GetLastLine( RawData from )
				{
                    const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
					const UINT DATA_INDEX  = FindDataIndex( &from );
					if( MERGED_DATA_SIZE > 0 && DATA_INDEX != 0 && DATA_INDEX != ( - 1 ) )
                    {
						const int OPEN_BRACKET_INDEX = GetLast( &from, "{" );
						UINT highest;
						int currentLast;
						std::vector< UINT > compareVector;
						currentLast = GetLast( &from, ";" );
						if( currentLast > NONE )
							compareVector.push_back( currentLast );
						currentLast = GetLast( &from, "\n" );
						if( currentLast > NONE )
							compareVector.push_back( currentLast );
						currentLast = GetLast( &from, "}" );
						if( currentLast > NONE )
							compareVector.push_back( currentLast );
						if( ( ( OPEN_BRACKET_INDEX ) - 1 ) >= 0 )
						{
							int hasClosing = NONE;
							hasClosing = GetNext( &from, "}" );
							if( ( hasClosing - 1 ) > NONE )
							{
								if( UCC::Process::ScopeAnylsis::IsInArray( from, UCC::Process::Globals::mergedData[ OPEN_BRACKET_INDEX ],
									UCC::Process::Globals::mergedData[ ( hasClosing - 1 ) ], UCC::Process::Globals::mergedData[ ( OPEN_BRACKET_INDEX - 1 ) ] )
									== false )
										compareVector.push_back( OPEN_BRACKET_INDEX );
							}
						}
						const UINT SIZE = compareVector.size();
						if( SIZE > 0 )
						{
							highest = compareVector[ 0 ];
							for( UINT i = 0; i < SIZE; ++i )
							{
								if( UCC::Process::Globals::mergedData[ compareVector[ i ] ].originalLocation >
									UCC::Process::Globals::mergedData[ highest ].originalLocation )
										highest = compareVector[ i ];
							}
							return UCC::Process::Globals::mergedData[ highest ];
						}
						else if( DATA_INDEX != 0 && DATA_INDEX != ( - 1 ) )
							return GetLastLine( UCC::Process::Globals::mergedData[ ( DATA_INDEX - 1 ) ] );
					}
                    RawData nothingFound;
				    nothingFound.parsedData = UNDEFINED_DATA;
					nothingFound.originalLocation = ( UCC::Process::Globals::code.size() + 1 );
					return nothingFound;
				}
				DECL_SCOPE GetDeclorationType( RawData declared )
				{
					const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
					if( MERGED_DATA_SIZE > 0 )
					{
						if( UCC::Process::AttributeAnylsis::UsedThis( declared ) == true )
							return ( CLASS_DECL );
						else
						{
							//TODO: Clean this up! Yuk!//
							int nextClosed = 0;
							int lastOpen = 0;
							const int DATA_INDEX = FindDataIndex( &declared );
							int i = DATA_INDEX;
							int scope = 0;
							/*while( ( i - 1 ) >= 0 )
							{								
								if( UCC::Process::Globals::mergedData[ i ].parsedData == "}" )
									++scope;
								else if( UCC::Process::Globals::mergedData[ i ].parsedData == "{" ) {
									--scope;
								}
								if( scope == ( - 1 ) )
									break;
								--i;
								
							}
							lastOpen = i;
							scope = ( - 1 );
							while( i < MERGED_DATA_SIZE )
							{
								if( UCC::Process::Globals::mergedData[ i ].parsedData == "}" )
									--scope;
								else if( UCC::Process::Globals::mergedData[ i ].parsedData == "{" )
									++scope;
								if( scope == ( - 1 ) )
									break;
								++i;
							}
							nextClosed = i;*/
							UCC::Process::DOUBLE_RAW_DATA scopeData = 
								UCC::Process::ScopeAnylsis::GetScopeMarkers( declared, "{", "}" );
							nextClosed = FindDataIndex( &scopeData.second );
							lastOpen = FindDataIndex( &scopeData.first );
							if( lastOpen == NONE )
								return ( OTHER_DECL );
							if( nextClosed == NONE )
							{
								compilerFailure = true;
								std::cerr << UCPP_ERROR << "Bracket mismatch!\n";
								return ( DECL_ERROR );
							}
							if( lastOpen < MERGED_DATA_SIZE && nextClosed < MERGED_DATA_SIZE )
							{
								if( ( lastOpen - 1 ) >= 0 && ( nextClosed - 1 ) >= 0 )
								{
									if( UCC::Process::ScopeAnylsis::IsInBlock( declared, UCC::Process::Globals::mergedData[ lastOpen ], UCC::Process::Globals::mergedData[ ( nextClosed - 1 ) ], 
										UCC::Process::Globals::mergedData[ ( lastOpen - 1 ) ] ) == true )
									{
										if( UCC::Process::AttributeAnylsis::UsedThis( declared ) == true )
											return ( CLASS_DECL );
										return ( BLOCK_DECL );
									}
									else if( UCC::Process::ScopeAnylsis::IsInClass( declared, UCC::Process::Globals::mergedData[ lastOpen ], UCC::Process::Globals::mergedData[ ( nextClosed - 1 ) ], 
											UCC::Process::Globals::mergedData[ ( lastOpen - 1 ) ] ) == true )
									{
										compilerFailure = true;
										std::cerr << UCPP_ERROR << "Can not use 'borrow' with constant field.\n";
										return ( DECL_ERROR );
									}
									else if( UCC::Process::ScopeAnylsis::IsInArray( declared, UCC::Process::Globals::mergedData[ lastOpen ], UCC::Process::Globals::mergedData[ ( nextClosed - 1 ) ], 
											UCC::Process::Globals::mergedData[ ( lastOpen - 1 ) ] ) == true )
										return ( BLOCK_DECL );
									else 
										return ( OTHER_DECL );
								}
							}
							else
							{
								compilerFailure = true;
								std::cerr << UCC_INTERNAL_ERROR << "Data excedes range in, GetDecloarationType().\n";
								return ( DECL_ERROR );
							}
						}
					}
					compilerFailure = true;
					std::cerr << UCPP_ERROR << "No code!\n";
					return ( DECL_ERROR );	
				}
				RawData GetLastClassDecloration( RawData relitiveTo )
				{
					const int DATA_INDEX = FindDataIndex( &relitiveTo );
					UCC::Process::DOUBLE_RAW_DATA scopeData = 
						UCC::Process::ScopeAnylsis::GetScopeMarkers( relitiveTo, "{", "}" );
					UCC::Process::RawData class__;
					class__.parsedData = UNDEFINED_DATA;
					class__.originalLocation = UCC::Process::Globals::location;
					++UCC::Process::Globals::location;//( UCC::Process::Globals::code.size() + 1 );
					if( DATA_INDEX != UCC::Process::NONE )
					{
						for( UINT i = DATA_INDEX; ( ( ( int ) i ) - 1 ) >= 0; --i )
						{
							if( UCC::Process::Globals::mergedData[ i ].parsedData == "class" || 
								UCC::Process::Globals::mergedData[ i ].parsedData == "struct" )
							{
								if( i < UCC::Process::Algorithm::Search::FindDataIndex( &scopeData.first ) )
								{
									UCC::Process::RawData canidate = UCC::Process::Algorithm::Search::GetLastLine( UCC::Process::Globals::mergedData[ i ] );
                                    if( UCC::Process::Algorithm::Search::GetLast( &UCC::Process::Globals::mergedData[ i ], "typedef" ) == ( ( ( int ) i ) - 1 ) || 
											UCC::Process::Algorithm::Search::FindDataIndex( &canidate ) == ( ( ( int ) i ) - 1 ) )
                                    {
                                        if( ( i + 2 ) < UCC::Process::Globals::mergedData.size() )
                                        {
                                            if( UCC::Process::Globals::mergedData[ ( i + 2 ) ].parsedData != ";" ) {
                                                class__ = UCC::Process::Globals::mergedData[ i ];
    									    	break;
                                            }
                                        }
                                    }
                                    else if( UCC::Process::Algorithm::Search::FindDataIndex( &canidate ) == ( i - 1 ) ) {
											class__ = UCC::Process::Globals::mergedData[ i ];
											break;
									}
								}
							}
						}
					}
					return class__;
				}
            }
        }
        namespace Parseing
        {
            //TODO: Clean all of this up!//
            void Parse()
            {
                const UINT CSIZE = UCC::Process::Globals::code.size();
                const UINT OSIZE = UCC::Process::Globals::operators.size();
                const UINT WSIZE = UCC::Process::Globals::wordsToParse.size();
                //So the loop will execut in any senerio.//
                UINT currentWordSize = 1;
                for( UINT i = 0; i < WSIZE; ++i )
                {
                    currentWordSize = UCC::Process::Globals::wordsToParse[ i ].size();
                    for( UINT j = 0; j < CSIZE; ++j )
                    {
                        if( UCC::Process::Globals::code.substr( j, currentWordSize )
                            == UCC::Process::Globals::wordsToParse[ i ] )
                        {
                            if( NotWord( j, ( j + currentWordSize - 1 ),
                                UCC::Process::Globals::wordsToParse[ i ] ) == false ) {
                                UCC::Process::Globals::parsedCodeChunks.push_back( UCC::Process::Data(
                                    UCC::Process::Globals::wordsToParse[ i ], j ) );
                            }
                        }
                    }
                }
                for( UINT i = 0; i < CSIZE; ++i )
                {
                    if( std::isspace( UCC::Process::Globals::code[ i ], std::locale() ) == false &&
                        HasOperator( i ) == NO_OP )
                    {
                        UINT j = ( i + 1 );
                        while( j < CSIZE )
                        {
                            if( std::isspace( UCC::Process::Globals::code[ j ], std::locale() ) != false ||
                                HasOperator( j ) != NO_OP )
                                break;
                            ++j;
                        }
                        const UINT DATA_LENGTH = ( j - i );
                        std::string word = UCC::Process::Globals::code.substr( i, DATA_LENGTH );
                        i += DATA_LENGTH;
                        bool isKeyWord = false;
                        for( UINT o = 0; o < WSIZE; ++o )
                        {
                            if( word == UCC::Process::Globals::wordsToParse[ o ] ) {
                                isKeyWord = true;
                                break;
                        }
                    }
                    if( isKeyWord == false ) {
                        UCC::Process::Globals::parsedCustomNames.push_back(
                        UCC::Process::Data( word, ( i - 1 ) ) );
                    }
                }
                }
                UINT currentOperatorSize = 0;
                for( UINT i = 0; i < OSIZE; ++i )
                {
                    currentOperatorSize = UCC::Process::Globals::operators[ i ].size();
                    for( UINT j = 0; j < CSIZE; ++j )
                    {
                        if( UCC::Process::Globals::code.substr( j, currentOperatorSize )
                            == UCC::Process::Globals::operators[ i ] ) {
                            UCC::Process::Globals::parsedCodeChunks.push_back( UCC::Process::Data(
                                UCC::Process::Globals::operators[ i ], j ) );
                        }
                    }
                }
            }
            bool NotWord( UINT begin, UINT end, std::string canidate )
            {
                const UINT SIZE = UCC::Process::Globals::code.size();
                if( begin < SIZE && end < SIZE )
                {
                    if( ( ( end + 1 ) < SIZE ) )
                    {
                        if( std::isspace( UCC::Process::Globals::code[ ( end + 1 ) ], std::locale() ) == false
                            &&
                            ( int ) HasOperator( ( end + 1 ) ) == 2 )
                                return ( true );
                    }
                    if( ( ( ( int ) begin ) - 1 ) > ( -1 ) )
                    {
                        if( ( ( begin - 1 ) < SIZE ) )
                        {
                            if( std::isspace( UCC::Process::Globals::code
                                [ ( begin - 1 ) ], std::locale() ) == false &&
                                ( int ) HasOperator( ( begin - 1 ) ) == 2  )
                                    return ( true );
                            else
                                return ( false );
                        }
                        else
                            return ( false );
                    }
                    else
                        return ( false );
                }
                else
                    return ( false );
                return ( true );
            }
            OPERATOR_CONDITIONAL HasOperator( UINT canidateLocation )
            {
                UINT CODE_SIZE = UCC::Process::Globals::code.size();
                if( canidateLocation < CODE_SIZE )
                {
                    std::string canidate = "";
                    canidate = UCC::Process::Globals::code[ canidateLocation ];
                    UINT OPERATORS_SIZE = UCC::Process::Globals::operators.size();
                    for( UINT i = 0; i < OPERATORS_SIZE; ++i )
                    {
                        if( canidate == UCC::Process::Globals::operators[ i ] )
                            return ( IS_OP );
                        if( UCC::Process::Globals::operators[ i ].find( canidate ) !=
                            std::string::npos )
                            return ( PART_OF_OP );
                    }
                }
                return ( NO_OP );
            }
        }
		namespace AttributeAnylsis
		{
			bool IsStatic( RawData data )
			{
				RawData lastLine = UCC::Process::Algorithm::Search::GetLastLine( data );
				try {
					if( lastLine.parsedData == UCC::Process::UNDEFINED_DATA )
						throw "In IsStatic(): Data undefined.\n";
				}
				catch( std::string exception ) {
					std::cerr << UCC_INTERNAL_ERROR << exception;
					return ( false );
				}
				catch( std::exception exception ) {
					std::cerr << UCC_INTERNAL_ERROR << "501: " << exception.what() << "\n";
				}
				try
				{
					int nextSemiColon = UCC::Process::Algorithm::Search::GetNext( &data, ";" );
					int lineLocation = UCC::Process::Algorithm::Search::FindDataIndex( &lastLine );
					int dataLocation = UCC::Process::Algorithm::Search::FindDataIndex( &data );
					const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
					if( lineLocation != UCC::Process::Algorithm::Search::NONE && 
						    dataLocation != UCC::Process::Algorithm::Search::NONE && 
						    nextSemiColon != UCC::Process::Algorithm::Search::NONE )
					{
						for( UINT i = lineLocation; i < nextSemiColon && i < MERGED_DATA_SIZE; ++i ) {
							if( UCC::Process::Globals::mergedData[ i ].parsedData == "static" )
								return ( true );
						}
					}
					else
						throw "No error";
				}
				catch( std::string exception ) {
					return ( false );
				}
				catch( std::exception exception ) {
					std::cerr << UCC_INTERNAL_ERROR << "526: " << exception.what() << "\n";
				}
				return ( false );
			}
			bool UsedThis( RawData data )
			{
				try
				{
					if( data.rawDataType == NAME )
					{
						const int LAST_THIS = UCC::Process::Algorithm::Search::GetLast( &data, "this" );
						if( LAST_THIS != UCC::Process::Algorithm::Search::NONE )
						{
							RawData lastLine = UCC::Process::Algorithm::Search::GetLastLine( data );
							const int LAST_LINE_INDEX =  UCC::Process::Algorithm::Search::FindDataIndex( &lastLine );
							const int DATA_INDEX = UCC::Process::Algorithm::Search::FindDataIndex( &data );
							if( DATA_INDEX == UCC::Process::Algorithm::Search::NONE )
								throw std::string{ "Internal Warning: Data index not defined in UsedThis().\n" };
							if( LAST_LINE_INDEX == UCC::Process::Algorithm::Search::NONE )
								throw std::string{ "Internal Warning: Last line index not defined in UsedThis().\n" };
							const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
							if( MERGED_DATA_SIZE != 0 )
							{
								int end = UCC::Process::Algorithm::Search::NONE;
								for( UINT i = ( UINT ) DATA_INDEX; i < MERGED_DATA_SIZE; ++i )
								{
									if( lastLine.originalLocation != UCC::Process::Globals::mergedData[ i ].originalLocation || 
                                            UCC::Process::Globals::mergedData[ i ].parsedData == "," ) {
										end = ( int ) i;
										break;
									}
    							}
    							for( UINT i = LAST_LINE_INDEX; i < MERGED_DATA_SIZE; ++i )
								{
									if( end != UCC::Process::Algorithm::Search::NONE ) {
										if( i >= end )
											break;
									}
									if( UCC::Process::Globals::mergedData[ i ].parsedData == "this" )
										return ( true );
								}
							}
						}
					}
				}
				catch( std::string exception ) {
					std::cerr << exception;
					return ( false );
				}
				return ( false );
			}
            bool IsAPartOfControl( RawData data )
            {
              	const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
              	if( MERGED_DATA_SIZE > 0 )
                {
              		std::vector< int > compareVector;
                    int highest = NONE;
                  	int scope = 0;
              		compareVector.push_back( UCC::Process::Algorithm::Search::GetLast( &data, "if" ) );
                	compareVector.push_back( UCC::Process::Algorithm::Search::GetLast( &data, "for" ) );
                	compareVector.push_back( UCC::Process::Algorithm::Search::GetLast( &data, "while" ) );
                    compareVector.push_back( UCC::Process::Algorithm::Search::GetLast( &data, "switch" ) );
                    for( UINT i = 0; i < 4; ++i ) {
                        if( compareVector[ i ] > highest )
                            highest = compareVector[ i ];
                    }
                    if( highest == NONE )
                        return ( false );
                    if( ( highest + 1 ) < MERGED_DATA_SIZE && data.originalLocation > 
                       UCC::Process::Globals::mergedData[ highest ].originalLocation )
                    {
                    	for( UINT i = ( highest + 1 ); i < MERGED_DATA_SIZE; ++i )
                    	{
                    		if( UCC::Process::Globals::mergedData[ i ].parsedData == "(" )
                            	++scope;
                          	if( UCC::Process::Globals::mergedData[ i ].parsedData == ")" )
                              	--scope;
                          	//Reuse the varible.//
                            if( scope == 0 ) {
                              	highest = ( int ) i;
                             	break;
                            }
                        }
                      	if( data.originalLocation < 
                           UCC::Process::Globals::mergedData[ highest ].originalLocation )
                          		return ( true );
                    }
                    else if( data.originalLocation == 
                            UCC::Process::Globals::mergedData[ highest ].originalLocation )
                           		return ( true );
                  	else
                    {
                      	std::cerr << UCPP_ERROR << "Improper conditional.\n";
                      	compilerFailure = true;
                      	return ( false );
                    }
                }
				return ( false );
            }
		}
        namespace ScopeAnylsis
        {
            namespace Generic
            {
                inline UINT GetPosition( UINT begin ) {
                    return begin;
                }
                inline UINT GetPosition( UCC::Process::Data begin ) {
                    return UCC::Process::Algorithm::Search::FindDataIndex( &begin );
                }
            }
            template< typename T >
            UINT GetScope( std::string openingSymbol, std::string closingSymbol, T symbolePosition, UINT from, UINT to )
            {
                const UINT NUMBER_OF_SYMBOLES = UCC::Process::Globals::mergedData.size();
                const UINT HIGHEST_LOCATION = NUMBER_OF_SYMBOLES - 1;
                UINT begin = UCC::Process::ScopeAnylsis::Generic::GetPosition( symbolePosition );
                UINT scope = 0;
                //Change this?//
                bool wasUndefined = false;
                if( to == HIGHEST_LOCATION ) {
                    to = begin;
                    wasUndefined = true;
                }
                if( from < HIGHEST_LOCATION && to <= HIGHEST_LOCATION )
                {
					UINT iBegin = 0;
					if( wasUndefined == false )
					{
						for ( UINT i = 0; i < NUMBER_OF_SYMBOLES; ++i ) {
							if( UCC::Process::Algorithm::Search::FindDataIndex( &UCC::Process::Globals::mergedData[ i ] ) == from )
								iBegin = i;
						}
					}
					Data *current;
					for( UINT i = iBegin; i < NUMBER_OF_SYMBOLES; ++i )
					{
						current =& UCC::Process::Globals::mergedData[ i ];
						if( from <= i && to >= i )
						{
							if( current->parsedData == openingSymbol )
								++scope;
							else if( current->parsedData == closingSymbol ) {
								if( scope != 0 )
									--scope;
							}
						}
						else {
							current = 0;
							break;
						}
						current = 0;
					}
				}
                return scope;
            }
            bool IsInClass( RawData data, RawData opening, RawData closing, RawData last )
            {
                if( GetScope( "{", "}", data, 
						UCC::Process::Algorithm::Search::FindDataIndex( &opening ), UCC::Process::Algorithm::Search::FindDataIndex( &closing ) ) == 1 )
                {
                    //There is no need to tell if the borrow keyword is assigned to a varible declared as a class member.//
                    if( IsInFunction( data, opening, closing, last ) == true )
                        return ( false );
                    if( IsInArray( data, opening, closing, last ) == true )
                        return ( false );
                    return ( true );
                }
                return ( false );
            }
			void Debug()
			{
				static int dbg = 0;
				++dbg;
				std::cerr << "SAD:" << dbg << "\n";
			}
            bool IsInArray( RawData data, RawData opening, RawData closing, RawData last )
            {
                if( GetScope( "{", "}", data, 
						UCC::Process::Algorithm::Search::FindDataIndex( &opening ), UCC::Process::Algorithm::Search::FindDataIndex( &closing ) ) == 1 ) {
					if( last.parsedData == "(" || last.parsedData == "=" )
						return ( true );
				}
				return ( false );
            }
			bool IsInBlock( RawData data, RawData opening, RawData closing, RawData last )
			{
				if( GetScope( "{", "}", data, 
						UCC::Process::Algorithm::Search::FindDataIndex( &opening ), UCC::Process::Algorithm::Search::FindDataIndex( &closing ) ) == 1 ) {
					if( last.parsedData == ")" || last.parsedData == ":" || last.parsedData == ";" || last.parsedData == "do" )
                        return ( true );
				}
                return ( false );
            }
			bool IsInFunction( RawData data, RawData opening, RawData closing, RawData last ) {
				return ( ( last.parsedData == ")" ) && ( IsInBlock( data, opening, closing, last ) == true ) );
			}
          	//TODO: Fill this in.//
        	DOUBLE_RAW_DATA GetScopeMarkers( UCC::Process::RawData relitiveTo, std::string openingMarker, 
                                              std::string closingMaker )
            {
              	const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
            	//TODO: Clean this up! Yuk!//
				int nextClosed = 0;
				int lastOpen = 0;
				const int DATA_INDEX = UCC::Process::Algorithm::Search::FindDataIndex( &relitiveTo );
				int i = DATA_INDEX;
				int scope = 0;
				while( ( i - 1 ) >= 0 )
				{
					if( UCC::Process::Globals::mergedData[ i ].parsedData == closingMaker )
						++scope;
					else if( UCC::Process::Globals::mergedData[ i ].parsedData == openingMarker )
						--scope;
					if( scope == ( - 1 ) )
						break;
					--i;
				}
				lastOpen = i;
				scope = ( - 1 );
				while( i < MERGED_DATA_SIZE )
				{
					if( UCC::Process::Globals::mergedData[ i ].parsedData == closingMaker )
						--scope;
					else if( UCC::Process::Globals::mergedData[ i ].parsedData == openingMarker )
						++scope;
					if( scope == ( - 1 ) )
						break;
					++i;
				}
				nextClosed = i;
        	    return DOUBLE_RAW_DATA( UCC::Process::Globals::mergedData[ lastOpen ], 
                                       UCC::Process::Globals::mergedData[ nextClosed ] );
            }
			int GetEndOfScope( UCC::Process::RawData scopeBegin, std::string closingSymbol )
			{
				const std::string BEGINING_SYMBOL = scopeBegin.parsedData;
				const int BEGIN = UCC::Process::Algorithm::Search::FindDataIndex( &scopeBegin );
				if( BEGIN != UCC::Process::NONE )
				{
					const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
					if( MERGED_DATA_SIZE != 0 )
					{
						int scope = 0;
						UINT i = ( BEGIN + 1 );
						while( i < MERGED_DATA_SIZE )
						{
							if( UCC::Process::Globals::mergedData[ i ].parsedData == BEGINING_SYMBOL )
								++scope;
							else if( UCC::Process::Globals::mergedData[ i ].parsedData == closingSymbol )
								--scope;
							if( scope == ( - 1 ) )
								break;
							++i;
						}
						return i;
					}
					else {
						UCC::compilerFailure = true;
						std::cerr << UCC::Process::UCPP_ERROR << "No data.\n";
					}
				}
				else {
					UCC::compilerFailure = true;
					std::cerr << UCC::Process::UCC_INTERNAL_ERROR << "Undefind data in GetEndOfScope().\n";
				}
				return ( UCC::Process::NONE );
			}
			bool IsInString( UCC::Process::RawData canidate )
			{
				if( canidate.parsedData == "'" || canidate.parsedData == "\"" )
					return ( true );
				else
				{
					const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
					if( MERGED_DATA_SIZE > 0 )
					{
						bool isInString = false;
						std::string type = "";
						const int END = UCC::Process::Algorithm::Search::FindDataIndex( &canidate );
						for( UINT i = 0; i < END && i < MERGED_DATA_SIZE; ++i )
						{
							if( UCC::Process::Globals::mergedData[ i ].parsedData == "'" || 
									UCC::Process::Globals::mergedData[ i ].parsedData == "\"" )
							{
								if( isInString == true && type == UCC::Process::Globals::mergedData[ i ].parsedData ) {
									type = "";
									isInString = false;
								}
								else if( isInString == false ) {
									isInString = true;
									type = UCC::Process::Globals::mergedData[ i ].parsedData;
								}
							}
						}
						return isInString;
					}
				}
				std::cerr << UCC_INTERNAL_ERROR << "No condition found for IsInString().\n";
				return ( false );
			}
        }
    }
    namespace Debug
    {
        namespace StringParsing
        {
            std::string ParseToString( UCC::Process::SCOPE_INFORMATION scopeInformation )
            {
                std::string output;
                switch( ( UINT ) scopeInformation )
                {
                    case 0: {
                        output = "BEFORE_SCOPE";
                        break;
					}
                    case 1: {
                        output = "OUT_OF_SCOPE";
                        break;
					}
                    case 2: {
                        output = "NORMAL";
                        break;
					}
					default: {
						break;
					}
                }
                return output;
            }
        }
    }
	namespace UCPP
	{
		namespace Borrow
		{
			void Debug()
			{
				static int dbg = 0;
				++dbg;
				std::cerr << "UBD:" << dbg << "\n";
			}
			UINT InsertAllocated( UINT after, UINT begin, UINT end, std::string pointerName )
			{
				std::vector< UCC::Process::RawData > allocated;
				UCC::Process::RawData dummyData;
				for( UINT i = begin; i < UCC::Process::Globals::mergedData.size() && i < ( end + 1 ); ++i )
					allocated.push_back( UCC::Process::Globals::mergedData[ i ] );
				for( UINT i = 0; i < allocated.size(); ++i )
					UCC::Process::Globals::mergedData.erase( ( UCC::Process::Globals::mergedData.begin() + begin ) );
				UCC::Process::Globals::mergedData[ begin - 1 ].parsedData = pointerName;
				for( UINT i = 0; i < allocated.size(); ++i )
				{
					UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + ( ( after + i ) + 1 ) ), allocated[ i ] );
					if( ( i + 1 ) == allocated.size() )
					{
						UCC::Process::RawData semiColon;
						semiColon.parsedData = ";";
						semiColon.originalLocation = UCC::Process::Globals::location;
						++UCC::Process::Globals::location;
						UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + ( ( after + i ) + 2 ) ), semiColon );
						return ( semiColon.originalLocation + 1 );
					}
				}
				return ( 1 );
			}
			//TODO: Fill these in.//
			std::string InsertIntoBlock( UCC::Process::RawData toInsert, UINT end )
            {
              	const UINT DATA_INDEX = UCC::Process::Algorithm::Search::FindDataIndex( &toInsert );
            	UCC::Process::RawData lastLine = UCC::Process::Algorithm::Search::GetLastLine( toInsert );
              	const UINT LAST_LINE_INDEX = UCC::Process::Algorithm::Search::FindDataIndex( &lastLine );
              	std::stringstream borrow;
              	std::stringstream pointer;
             	pointer << UCC_BLOCK_POINTER << std::rand() << " ";
              	borrow << " \n " << AUTO << " * " << pointer.str() << " = " << NEW << " ";
				//Re - using toInsert.//
              	toInsert.parsedData = borrow.str();
				toInsert.originalLocation = UCC::Process::Globals::location;
				++UCC::Process::Globals::location;
				InsertAllocated( LAST_LINE_INDEX, DATA_INDEX, end, pointer.str() );
              	UCC::Process::Globals::mergedData.insert( ( ( UCC::Process::Globals::mergedData.begin() + LAST_LINE_INDEX ) + 1 ), toInsert );
              	return pointer.str();
			}
			std::string InsertIntoClass( UCC::Process::RawData toInsert, UINT end )
			{
              	const UINT DATA_INDEX = UCC::Process::Algorithm::Search::FindDataIndex( &toInsert );
                UCC::Process::DOUBLE_RAW_DATA scopeData = 
                    UCC::Process::ScopeAnylsis::GetScopeMarkers( toInsert, "{", "}" );
				UCC::Process::RawData class__ = UCC::Process::Algorithm::Search::GetLastClassDecloration( toInsert );
            	UCC::Process::RawData lastLine = UCC::Process::Algorithm::Search::GetLastLine( toInsert );
              	const UINT LAST_LINE_INDEX = UCC::Process::Algorithm::Search::FindDataIndex( &lastLine );
              	std::stringstream expression;
              	std::stringstream pointer;
				std::stringstream decloration;
             	pointer << UCC_CLASS_POINTER << std::rand() << " ";
              	decloration << " \n " << toInsert.parsedData << " * " << pointer.str();
				expression << " \n " << DELETE << " " << pointer.str() << " ; \n " << pointer.str() << " = " << NEW << " ";
				toInsert.originalLocation = UCC::Process::Globals::location;
				++UCC::Process::Globals::location;
				InsertAllocated( LAST_LINE_INDEX, DATA_INDEX, end, pointer.str() );
				//Re - using toInsert.//
              	toInsert.parsedData = expression.str();
              	UCC::Process::Globals::mergedData.insert( ( ( UCC::Process::Globals::mergedData.begin() + LAST_LINE_INDEX ) + 1 ), toInsert );
				toInsert.originalLocation = UCC::Process::Globals::location;
				++UCC::Process::Globals::location;
				toInsert.parsedData = "public";
				UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + UCC::Process::ScopeAnylsis::GetEndOfScope( UCC::Process::Globals::mergedData[ 
					UCC::Process::Algorithm::Search::GetNext( &class__, "{" ) ], "}" ) ), toInsert );
				toInsert.originalLocation = UCC::Process::Globals::location;
				++UCC::Process::Globals::location;
				toInsert.parsedData = ":";
				UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + UCC::Process::ScopeAnylsis::GetEndOfScope( UCC::Process::Globals::mergedData[ 
					UCC::Process::Algorithm::Search::GetNext( &class__, "{" ) ], "}" ) ), toInsert );
				toInsert.originalLocation = UCC::Process::Globals::location;
				++UCC::Process::Globals::location;
				toInsert.parsedData = decloration.str();
				UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + UCC::Process::ScopeAnylsis::GetEndOfScope( UCC::Process::Globals::mergedData[ 
					UCC::Process::Algorithm::Search::GetNext( &class__, "{" ) ], "}" ) ), toInsert );
				toInsert.originalLocation = UCC::Process::Globals::location;
				++UCC::Process::Globals::location;
				toInsert.parsedData = ";";
				UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + UCC::Process::ScopeAnylsis::GetEndOfScope( UCC::Process::Globals::mergedData[ 
					UCC::Process::Algorithm::Search::GetNext( &class__, "{" ) ], "}" ) ), toInsert );
              	return pointer.str();
			}
			std::string Insert( UCC::Process::RawData toInsert, UINT end, UCC::Process::DECL_SCOPE scopeType )
            {
                if( scopeType == UCC::Process::BLOCK_DECL )
                	return InsertIntoBlock( toInsert, end );
                else if( scopeType ==  UCC::Process::CLASS_DECL )
              	  	return InsertIntoClass( toInsert, end );
                return ( UCC::Process::UNDEFINED_DATA );
			}
			void AddDeleteClass( UCC::Process::RawData toDelete, std::string pointerName )
			{
				std::stringstream delete__;
				delete__ << DELETE << " " << pointerName << " ";
				UCC::Process::RawData class__ = UCC::Process::Algorithm::Search::GetLastClassDecloration( toDelete );
				const int BEGIN = UCC::Process::Algorithm::Search::GetNext( &class__, "{" );
				const int END = UCC::Process::ScopeAnylsis::GetEndOfScope( UCC::Process::Globals::mergedData[ BEGIN ], "}" );
				const int FALSE = ( - 1 );
				/*This will be set to a value (the location of the tilde) other than "FALSE" 
				if there is a destructor present in the class.*/
				int destructorExists = FALSE;
				if( END != 0 && END != ( - 1 ) && BEGIN != 0 && BEGIN != ( - 1 ) )
				{
					const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
					//Just looks nicer.//
					if( BEGIN < MERGED_DATA_SIZE && END < MERGED_DATA_SIZE )
					{
						for( UINT i = BEGIN; i < MERGED_DATA_SIZE; ++i )
						{
							if( UCC::Process::Globals::mergedData[ i ].parsedData == "~" ) {
								destructorExists = i;
								break;
							}
						}
					}
				}
				if( destructorExists != FALSE )
				{
					const int END_PARENTHISY = UCC::Process::ScopeAnylsis::GetEndOfScope( 
						UCC::Process::Globals::mergedData[ UCC::Process::Algorithm::Search::GetNext( &UCC::Process::Globals::mergedData[ destructorExists ], "(" ) ], ")" );
					if( END_PARENTHISY != 0 && END_PARENTHISY != ( - 1 ) )
					{
						const int END_OF_DESTRUCTOR = UCC::Process::ScopeAnylsis::GetEndOfScope( 
																									UCC::Process::Globals::mergedData[ UCC::Process::Algorithm::Search::GetNext( 
																									&UCC::Process::Globals::mergedData[ END_PARENTHISY ], "{" ) ], "}" );
						if( END_OF_DESTRUCTOR != 0 && END_OF_DESTRUCTOR != ( - 1 ) )
						{
							UCC::Process::RawData toInsert;
							toInsert.parsedData = ";";
							toInsert.originalLocation = UCC::Process::Globals::location;
							++UCC::Process::Globals::location;//( END_OF_DESTRUCTOR - 1 );
							UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END_OF_DESTRUCTOR ), toInsert );
							toInsert.parsedData = delete__.str();
							UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END_OF_DESTRUCTOR ), toInsert );
						}
					}
				}
				else
				{
					std::string className = "";
					bool hasConstructor = false;
					if( ( UCC::Process::Algorithm::Search::FindDataIndex( &class__ ) + 1 ) < UCC::Process::Globals::mergedData.size() )
                    {
						if( UCC::Process::Globals::mergedData[ ( UCC::Process::Algorithm::Search::FindDataIndex( &class__ ) + 1 ) ].rawDataType == UCC::Process::NAME )
							className = UCC::Process::Globals::mergedData[ ( UCC::Process::Algorithm::Search::FindDataIndex( &class__ ) + 1 ) ].parsedData;
                        else if( ( END + 1 ) < UCC::Process::Globals::mergedData.size() ) {
						    if( UCC::Process::Globals::mergedData[ ( END + 1 ) ].rawDataType == UCC::Process::NAME )
							    className = UCC::Process::Globals::mergedData[ ( END + 1 ) ].parsedData;
					    }
                        else
					    {
						    UCC::compilerFailure = true;
						    std::cerr << UCC::Process::UCPP_ERROR << "Can not use 'borrow' in an anonymous class (or struct).\n";
						    return;
					    }
					}
					else if( ( END + 1 ) < UCC::Process::Globals::mergedData.size() ) {
						if( UCC::Process::Globals::mergedData[ ( END + 1 ) ].rawDataType == UCC::Process::NAME )
							className = UCC::Process::Globals::mergedData[ ( END + 1 ) ].parsedData;
					}
					else
					{
						UCC::compilerFailure = true;
						std::cerr << UCC::Process::UCPP_ERROR << "Can not use 'borrow' in an anonymous class (or struct).\n";
						return;
					}
					const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
					if( BEGIN < MERGED_DATA_SIZE && END < MERGED_DATA_SIZE )
					{
						for( UINT i = BEGIN; i < MERGED_DATA_SIZE; ++i )
						{
							if( UCC::Process::Globals::mergedData[ i ].parsedData == className )
							{
								UCC::Process::DOUBLE_RAW_DATA scopeData = 
									UCC::Process::ScopeAnylsis::GetScopeMarkers( UCC::Process::Globals::mergedData[ i ], "{", "}" );
								if( scopeData.first.originalLocation == BEGIN )
								{
									if( ( i + 1 ) < MERGED_DATA_SIZE ) {
										if( UCC::Process::Globals::mergedData[ ( i + 1 ) ].parsedData == "(" )
											hasConstructor = true;
									}
								}
							}
						}
					}
					UCC::Process::RawData toInsert;
					toInsert.originalLocation = UCC::Process::Globals::location;
					++UCC::Process::Globals::location;
					toInsert.parsedData = "public";			
					toInsert.parsedData = "}";
					UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
					toInsert.parsedData = ";";
					UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
					toInsert.parsedData = delete__.str();
					UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
					toInsert.parsedData = "{";
					UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
					toInsert.parsedData = ")";
					UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
					toInsert.parsedData = "(";
					UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
					toInsert.parsedData = className;
					UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
					toInsert.parsedData = "~";
					UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
					if( hasConstructor == false )
					{
						toInsert.parsedData = "}";
						UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
						toInsert.parsedData = "{";
						UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
						toInsert.parsedData = ")";
						UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
						toInsert.parsedData = "(";
						UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
						toInsert.parsedData = className;
						UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + END ), toInsert );
					}
				}
			}
			void AddDeleteBlock( UCC::Process::RawData toDelete, std::string pointerName )
            {
                UCC::Process::DOUBLE_RAW_DATA scopeData = 
                    UCC::Process::ScopeAnylsis::GetScopeMarkers( toDelete, "{", "}" );
              	const int BEGIN = UCC::Process::Algorithm::Search::FindDataIndex( &scopeData.first );
              	const int END = UCC::Process::Algorithm::Search::FindDataIndex( &scopeData.second );
				UCC::Process::RawData beforeOpen;
				beforeOpen.parsedData = UCC::Process::UNDEFINED_DATA;
				beforeOpen.originalLocation = 1;
				if( ( BEGIN - 1 ) >= 0 )
					beforeOpen = UCC::Process::Globals::mergedData[ ( BEGIN - 1 ) ];
    	        std::stringstream delete__;
                UCC::Process::RawData insert;
				insert.originalLocation = 1;
				delete__ << " \n " << DELETE << " " << pointerName << "; \n ";
              	insert.parsedData = delete__.str();
				UINT detections = 0;
              	if( BEGIN != UCC::Process::NONE && END != UCC::Process::NONE )
                {
					bool skip = false;
                    for( UINT i = ( UCC::Process::Algorithm::Search::FindDataIndex( &toDelete ) + 1 ); i < ( END - 1 ) && i < UCC::Process::Globals::mergedData.size(); ++i )
                    {
                        if( i > UCC::Process::Algorithm::Search::FindDataIndex( &toDelete ) )
                        {
                        	if( UCC::Process::Globals::mergedData[ i ].parsedData == "return" || UCC::Process::Globals::mergedData[ i ].parsedData == "break" || 
    							UCC::Process::Globals::mergedData[ i ].parsedData == "continue" )
    						{
    							if( beforeOpen.parsedData != UCC::Process::UNDEFINED_DATA && skip == false )
    							{
    								if( ( ( int ) ( i - 1 ) ) >= 0 )
    								{
    									UCC::Process::DOUBLE_RAW_DATA returnData = 
    										UCC::Process::ScopeAnylsis::GetScopeMarkers( UCC::Process::Globals::mergedData[ i ], "{", "}" );
    									if( UCC::Process::Globals::mergedData[ i ].parsedData == "break" || UCC::Process::Globals::mergedData[ i ].parsedData == "continue" ) {
    										if( scopeData.first.originalLocation != returnData.first.originalLocation )
    											continue;
    									}
    									const int LAST_INDEX = ( UCC::Process::Algorithm::Search::FindDataIndex( &returnData.first ) - 1 );
    									if( LAST_INDEX >= 0 )
    									{
    										UCC::Process::RawData last = UCC::Process::Globals::mergedData[ ( LAST_INDEX ) ];
    										if( last.parsedData == ")" ) {
    											if( UCC::Process::AttributeAnylsis::IsAPartOfControl( last ) == false && beforeOpen.originalLocation != last.originalLocation )
    												continue;
    										}
    									}
    								}
    								UCC::Process::RawData lastLine = 
    									UCC::Process::Algorithm::Search::GetLastLine( UCC::Process::Globals::mergedData[ i ] );
    								const UINT LAST_LINE_INDEX = UCC::Process::Algorithm::Search::FindDataIndex( &lastLine );
									insert.originalLocation = UCC::Process::Globals::location;
									++UCC::Process::Globals::location;
    								UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + ( LAST_LINE_INDEX + 1 ) ), insert );
    								++detections;
    								skip = true;
    							}
    							else
    								skip = false;
    						}
						}
                    }
                  	UCC::Process::RawData lastLine = UCC::Process::Algorithm::Search::GetLastLine( UCC::Process::Globals::mergedData[ ( END + detections ) ] );
                    const UINT LAST_LINE_INDEX = UCC::Process::Algorithm::Search::FindDataIndex( &lastLine );
					insert.originalLocation = UCC::Process::Globals::location;
					++UCC::Process::Globals::location;
                    UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + ( LAST_LINE_INDEX + 1 ) ), insert );
                }
	            else {
                	UCC::compilerFailure = true;
                  	std::cerr << UCC::Process::UCPP_ERROR << "Bracket mismatch!\n";
                }
			}
			void Delete( UCC::Process::RawData toDelete, std::string pointerName, UCC::Process::DECL_SCOPE scopeType )
            {
                if( scopeType == UCC::Process::BLOCK_DECL )
                	AddDeleteBlock( toDelete, pointerName );
                else if( scopeType ==  UCC::Process::CLASS_DECL )
              	  	AddDeleteClass( toDelete, pointerName );
			}
            void GenerateBorrow( UCC::Process::RawData toGenerate, UINT end )
			{
				UCC::Process::DECL_SCOPE scopeType = UCC::Process::Algorithm::Search::GetDeclorationType( toGenerate );
				if( scopeType == UCC::Process::OTHER_DECL ) {
					std::cerr << UCC::Process::UCPP_ERROR << "Can not use 'borrow' out of a scope.\n";
					compilerFailure = true;
				}
            	Delete( toGenerate, Insert( toGenerate, end, scopeType ), scopeType );
			}
            void DoBorrow()
            {
              	if( UCC::Process::Globals::mergedData.size() > 0 )
                {
                	for( UINT i = 0; i < UCC::Process::Globals::mergedData.size(); ++i )
                    {
                    	if( UCC::Process::Globals::mergedData[ i ].parsedData == BORROW && 
								UCC::Process::ScopeAnylsis::IsInString( UCC::Process::Globals::mergedData[ i ] ) == false )
                        {
							//Find the data to be allocated.//
							UCC::Process::RawData lastLine = UCC::Process::Algorithm::Search::GetLastLine( UCC::Process::Globals::mergedData[ i ] );
		                    const UINT LAST_LINE_INDEX = UCC::Process::Algorithm::Search::FindDataIndex( &lastLine );
							int nextLineIndex = UCC::Process::NONE;
							for( UINT j = 1; j < UCC::Process::Globals::mergedData.size(); ++j )
							{
								if( UCC::Process::Algorithm::Search::GetLastLine( UCC::Process::Globals::mergedData[ 
									( ( LAST_LINE_INDEX + j ) > UCC::Process::Globals::mergedData.size() ) ? 
									LAST_LINE_INDEX : ( LAST_LINE_INDEX + j ) ] ).originalLocation != lastLine.originalLocation )
								{
										nextLineIndex = ( ( ( LAST_LINE_INDEX + j ) > UCC::Process::Globals::mergedData.size() ) ? 
											LAST_LINE_INDEX : ( LAST_LINE_INDEX + j ) );
										break;
								}
							}
							int end = UCC::Process::NONE;
							const int FIRST_OPEN_CURLY_BRACKET = UCC::Process::Algorithm::Search::GetNext( &UCC::Process::Globals::mergedData[ i ], "{" );
							const int FIRST_OPEN_SQUARE_BRACKET = UCC::Process::Algorithm::Search::GetNext( &UCC::Process::Globals::mergedData[ i ], "[" );
							const int FIRST_OPEN_CARROT_BRACKET = UCC::Process::Algorithm::Search::GetNext( &UCC::Process::Globals::mergedData[ i ], "<" );
							const int FIRST_OPEN_PARENTHISY = UCC::Process::Algorithm::Search::GetNext( &UCC::Process::Globals::mergedData[ i ], "(" );
							int closingCurly = UCC::Process::NONE;
							int closingSquare = UCC::Process::NONE;
							int lowestElement = UCC::Process::NONE;
							if( FIRST_OPEN_CURLY_BRACKET > UCC::Process::NONE )
                                closingCurly = UCC::Process::ScopeAnylsis::GetEndOfScope( UCC::Process::Globals::mergedData[ FIRST_OPEN_CURLY_BRACKET ], "}" );
							if( FIRST_OPEN_SQUARE_BRACKET > UCC::Process::NONE )
								closingSquare = UCC::Process::ScopeAnylsis::GetEndOfScope( UCC::Process::Globals::mergedData[ FIRST_OPEN_SQUARE_BRACKET ], "]" );
							if( closingCurly > closingSquare && ( ( FIRST_OPEN_CURLY_BRACKET == ( int ) ( i - 1 ) ) || closingCurly == ( i + 1 ) ) && closingCurly > FIRST_OPEN_CURLY_BRACKET )
								end = closingCurly;
							else if( closingCurly < closingSquare && ( ( FIRST_OPEN_SQUARE_BRACKET == ( int ) ( i - 1 ) ) || closingSquare == ( i + 1 ) ) && 
									closingSquare > FIRST_OPEN_SQUARE_BRACKET )
								end = closingSquare;
							else
							{
								if( FIRST_OPEN_CARROT_BRACKET < FIRST_OPEN_PARENTHISY && FIRST_OPEN_CARROT_BRACKET >= i && 
										FIRST_OPEN_CARROT_BRACKET != ( - 1 ) && FIRST_OPEN_CARROT_BRACKET != 0 )
									end = FIRST_OPEN_CARROT_BRACKET;
								else if( FIRST_OPEN_PARENTHISY >= i && FIRST_OPEN_PARENTHISY != ( - 1 ) && FIRST_OPEN_PARENTHISY != 0 )
									end = UCC::Process::ScopeAnylsis::GetEndOfScope( UCC::Process::Globals::mergedData[ FIRST_OPEN_PARENTHISY ], ")" );
								else
									end = ( i + 1 );
							}
							if( UCC::Process::Algorithm::Search::GetNext( &UCC::Process::Globals::mergedData[ i ], ";" ) != ( - 1 ) && 
								UCC::Process::Algorithm::Search::GetNext( &UCC::Process::Globals::mergedData[ i ], ";" ) < end && 
								end > UCC::Process::Algorithm::Search::GetNext( &UCC::Process::Globals::mergedData[ i ], ";" ) )
									end = ( UCC::Process::Algorithm::Search::GetNext( &UCC::Process::Globals::mergedData[ i ], ";" ) - 1 );
							//Insert borrow.//
                          	if( ( i + 1 ) < UCC::Process::Globals::mergedData.size() && end < UCC::Process::Globals::mergedData.size() ) {
                        	    GenerateBorrow( UCC::Process::Globals::mergedData[ i + 1 ], end );
                                i = 0;
                          	}
                          	else
                                UCC::compilerFailure = true;
                        }
                    }
                }
            }
		}
      	namespace Repl
        {
          	//TODO: Fill these in.//
        	void DoRepl()
            {
            	for( UINT i = 0; i < UCC::Process::Globals::mergedData.size(); ++i )
                {
                	if( UCC::Process::Globals::mergedData[ i ].parsedData == "repl" && 
							UCC::Process::ScopeAnylsis::IsInString( UCC::Process::Globals::mergedData[ i ] ) == false ) {
                    	InsertRepl( UCC::Process::Globals::mergedData[ i ] );
                      	i = 0;
                    }
                }
            }
	        void InsertRepl( UCC::Process::RawData replLocation )
            {
                UCC::Process::RawData toRename;
              	bool insert = true;
				int location = UCC::Process::Algorithm::Search::FindDataIndex( &replLocation );
				UCC::Process::RawData class__ =   
                    UCC::Process::Algorithm::Search::GetLastClassDecloration( replLocation );
              	if( class__.originalLocation != 0 && class__.originalLocation > ( - 1 ) )
                {
					if( class__.originalLocation != 0 && class__.originalLocation != ( - 1 ) )
					{
						UCC::Process::RawData openBracket = UCC::Process::Globals::mergedData[ UCC::Process::Algorithm::Search::GetNext( &class__, "{" ) ];
						if( openBracket.originalLocation != 0 && openBracket.originalLocation > ( - 1 ) )
						{
							UCC::Process::DOUBLE_RAW_DATA scopeData = UCC::Process::ScopeAnylsis::GetScopeMarkers( 
								replLocation, "{", "}" );
							if( scopeData.first.originalLocation == openBracket.originalLocation ) {
								insert = true;
							}
						}
					}
                }
    			if( insert == true )
                {
                  	UCC::Process::RawData lastLine = UCC::Process::Algorithm::Search::GetLastLine( replLocation );
                  	for( UINT i = ( location + 5 ); i < UCC::Process::Globals::mergedData.size(); ++i )
                    {
                        if( lastLine.originalLocation != UCC::Process::Algorithm::Search::GetLastLine( 
								UCC::Process::Globals::mergedData[ i ] ).originalLocation ) {
                        	lastLine = UCC::Process::Globals::mergedData[ ( i - 1 ) ];
                            break;
                        }
                    }
                  	if( ( location + 5 ) < UCC::Process::Globals::mergedData.size() )
                    {
                  		int endOfRepl = UCC::Process::ScopeAnylsis::GetEndOfScope( 
                        	UCC::Process::Globals::mergedData[ ( location + 1 ) ], ")" );
                  		if( endOfRepl >= 0 )
                    	{
                    		if( UCC::Process::AttributeAnylsis::UsedThis( UCC::Process::Globals::mergedData[ endOfRepl ] ) == true ) {
                                	std::cerr << UCC::Process::UCPP_ERROR << "Can not use repl on class data member.\n";
                                    UCC::compilerFailure = true;
                    		}
                            else
                            {
                                const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
                                UCC::Process::RawData newName = UCC::Process::Globals::mergedData[ ( UCC::Process::Algorithm::Search::GetNext( &replLocation, ")" ) - 1 ) ];
                                toRename = UCC::Process::Globals::mergedData[ ( UCC::Process::Algorithm::Search::GetNext( &replLocation, "(" ) + 1 ) ];
                                UCC::Process::DOUBLE_RAW_DATA scopeData = 
                                    UCC::Process::ScopeAnylsis::GetScopeMarkers( replLocation, "{", "}" );
                                const UINT CLOSING_BRACKET = UCC::Process::Algorithm::Search::FindDataIndex( &scopeData.second );
                                for( UINT i = location; i < MERGED_DATA_SIZE && i < CLOSING_BRACKET && ( int ) CLOSING_BRACKET > location; ++i )
                                {
                                    if( UCC::Process::AttributeAnylsis::UsedThis( UCC::Process::Globals::mergedData[ i ] ) == false )
                                    {
                                        if( UCC::Process::Globals::mergedData[ i ].parsedData == newName.parsedData && 
												UCC::Process::ScopeAnylsis::IsInString( UCC::Process::Globals::mergedData[ i ] ) == false )
                                            UCC::Process::Globals::mergedData[ i ].parsedData = toRename.parsedData;
                                        else if( UCC::Process::Globals::mergedData[ i ].parsedData == toRename.parsedData && 
                                                 i > UCC::Process::Algorithm::Search::FindDataIndex( &lastLine ) ) {
                                            UCC::compilerFailure = false;
                                            std::cerr << UCC::Process::UCPP_ERROR << "Can not reference old name of renamed varible.\n";
                                        }
                                    } 
                                    else if( UCC::Process::Globals::mergedData[ i ].parsedData == newName.parsedData || 
											UCC::Process::Globals::mergedData[ i ].parsedData == toRename.parsedData )
									{
										if( i > UCC::Process::Algorithm::Search::FindDataIndex( &lastLine ) )
										{
											UCC::compilerFailure = false;
											std::cerr << UCC::Process::UCPP_ERROR << 
												"Can not use repl( toRename, newName ) with class data member or ambiguas refrernce to varible.\n";
										}
                                    }
                                }
                            }
                    	}
                  		else {
                  			std::cerr << UCC::Process::UCPP_ERROR << "Ill formed repl( toRename, newName ).\n";
                  			compilerFailure = true;
                		}
                    }
                    else {
                  			std::cerr << UCC::Process::UCPP_ERROR << "Ill formed repl( toRename, newName ).\n";
                  			compilerFailure = true;
                	}
                }
              	else {
                  	std::cerr << UCC::Process::UCPP_ERROR << "Can not use repl( toRename, newName ) on class member.\n";
                  	compilerFailure = true;
                }
                UCC::Process::RawData toInsert;
                toInsert.originalLocation = location;
                toInsert.parsedData = toRename.parsedData;
				for( UINT i = 6; i > 0; --i )
					UCC::Process::Globals::mergedData.erase( ( UCC::Process::Globals::mergedData.begin() + ( location + ( i - 1 ) ) ) );
                UCC::Process::Globals::mergedData.insert( ( UCC::Process::Globals::mergedData.begin() + location ), toInsert );
            }
        }
	}
    namespace Formatting
    {
		void FormatPreProcessor()
		{
			bool inPreProcessor = false;
			UINT begining = 0;
            for( UINT i = 0; i < UCC::Process::Globals::mergedData.size(); ++i )
            {
                if( UCC::Process::Globals::mergedData[ i ].parsedData == "#" && inPreProcessor == false && 
						UCC::Process::ScopeAnylsis::IsInString( UCC::Process::Globals::mergedData[ i ] ) == false ) {
					inPreProcessor = true;
					begining = i;
				}
				if( UCC::Process::Globals::mergedData[ i ].parsedData == "\n" && inPreProcessor == true && 
						UCC::Process::ScopeAnylsis::IsInString( UCC::Process::Globals::mergedData[ i ] ) == false )
				{
					const UINT END = i;
					std::stringstream preProsessor;
					preProsessor << "\n";
					for( UINT j = begining ; j <= END && j < UCC::Process::Globals::mergedData.size(); ++j )
						preProsessor << UCC::Process::Globals::mergedData[ j ].parsedData;
					for( int j = ( END - begining ); j >= 0; --j )
						UCC::Process::Globals::mergedData.erase( ( UCC::Process::Globals::mergedData.begin() + begining ) );
					UCC::Process::Globals::mergedData[ begining ].parsedData = preProsessor.str();
					i = 0;
					inPreProcessor = false;
				}
			}
		}
        void FitConditionals()
        {
            UINT unmatchedBrackets = 0;
            for( UINT i = 0; i < UCC::Process::Globals::mergedData.size(); ++i )
            {
                if( UCC::Process::Globals::mergedData[ i ].parsedData == "(" )
                {
                    if( ( ( ( ( int ) i ) - 1 ) >= 0 ) )
                    {
                        if( UCC::Process::Globals::mergedData[ ( i - 1 ) ].parsedData == "if" || 
                                UCC::Process::Globals::mergedData[ ( i - 1 ) ].parsedData == "for" || 
                                UCC::Process::Globals::mergedData[ ( i - 1 ) ].parsedData == "do" || 
                                UCC::Process::Globals::mergedData[ ( i - 1 ) ].parsedData == "while" || 
                                UCC::Process::Globals::mergedData[ ( i - 1 ) ].parsedData == "switch" )
                        {
                            if( UCC::Process::Globals::mergedData[ ( i - 1 ) ].parsedData == "while" )
                            {
                                if( ( ( ( ( int ) i ) - 2 ) >= 0 ) ) {
                                    if( UCC::Process::Globals::mergedData[ ( i - 2 ) ].parsedData == "}" )
                                        continue;
                                }
                            }
                            int endOfScope = UCC::Process::ScopeAnylsis::GetEndOfScope( UCC::Process::Globals::mergedData[ i ], ")" );
                            if( endOfScope != 0 && endOfScope != ( - 1 ) && ( endOfScope + 1 ) < UCC::Process::Globals::mergedData.size() )
                            {
                                if( UCC::Process::Globals::mergedData[ ( endOfScope + 1 ) ].parsedData != "{" )
                                {
                                    UCC::Process::RawData toInsert;
                                    toInsert.originalLocation = UCC::Process::Globals::location;
									++UCC::Process::Globals::location;
                                    toInsert.parsedData = "{";
                                    UCC::Process::Globals::mergedData.insert( ( ( UCC::Process::Globals::mergedData.begin() + endOfScope ) + 1 ), 
                                        toInsert );
                                    ++unmatchedBrackets;
                                }
                            }
                        }
                    }
                }
                if( UCC::Process::Globals::mergedData[ i ].parsedData == ";" && unmatchedBrackets > 0 )
                {
                    if( ( i + 1 ) < UCC::Process::Globals::mergedData.size() )
                    {
                        for( UINT offset = 0; unmatchedBrackets > 0; ++offset )
                        {
                            UCC::Process::RawData toInsert;
                            toInsert.originalLocation = UCC::Process::Globals::location;
							++UCC::Process::Globals::location;//( UCC::Process::Globals::mergedData[ i ].originalLocation + ( 1 + offset ) );
                            toInsert.parsedData = "}";
                            UCC::Process::Globals::mergedData.insert( ( ( UCC::Process::Globals::mergedData.begin() + i ) + 1 ), 
                                toInsert );
                            --unmatchedBrackets;
                        }
                    }
                }
            }
        }
		void DeleteComments()
		{
			std::map< std::string, std::string > assosiated;
			assosiated[ "/" ] = "\n";
			assosiated[ "*" ] = "*";
			for( UINT i = 0; i < UCC::Process::Globals::mergedData.size(); ++i )
			{
				if( UCC::Process::Globals::mergedData[ i ].parsedData == "/" && ( i + 1 ) < UCC::Process::Globals::mergedData.size() )
				{
					if( ( UCC::Process::Globals::mergedData[ ( i + 1 ) ].parsedData == "*" || UCC::Process::Globals::mergedData[ ( i + 1 ) ].parsedData == "/" ) && 
							UCC::Process::ScopeAnylsis::IsInString( UCC::Process::Globals::mergedData[ i ] ) == false )
					{
						std::string endChar = assosiated[ UCC::Process::Globals::mergedData[ ( i + 1 ) ].parsedData ];
						if( ( i + 2 ) < UCC::Process::Globals::mergedData.size() )
						{
							for( UINT j = ( i + 2 ); j < UCC::Process::Globals::mergedData.size(); ++j )
							{
								if( UCC::Process::Globals::mergedData[ j ].parsedData == endChar )
								{
									for( UINT o = 0; o < ( j - ( i - 2 ) ); ++o )
										UCC::Process::Globals::mergedData.erase( ( UCC::Process::Globals::mergedData.begin() + i ) );
									i = 0;
									break;
								}
							}
						}
					}
				}
			}
		}
		void DeleteNewLines()
		{
			for( UINT i = 0; i < UCC::Process::Globals::mergedData.size(); ++i )
			{
				if( UCC::Process::Globals::mergedData[ i ].parsedData == "\n" && 
						UCC::Process::ScopeAnylsis::IsInString( UCC::Process::Globals::mergedData[ i ] ) == false ) {
					UCC::Process::Globals::mergedData.erase( ( UCC::Process::Globals::mergedData.begin() + i ) );
					i = 0;
				}
			}
		}
        std::string FormatForWrite()
		{
			std::stringstream writeData;
			const UINT MERGED_DATA_SIZE = UCC::Process::Globals::mergedData.size();
            for( UINT i = 0; i < MERGED_DATA_SIZE; ++i )
			{
				if(	UCC::Process::Parseing::HasOperator( UCC::Process::Globals::mergedData[ i ].originalLocation ) != 
						UCC::Process::NO_OP && UCC::Process::ScopeAnylsis::IsInString( UCC::Process::Globals::mergedData[ i ] ) == false )
					writeData << UCC::Process::Globals::mergedData[ i ].parsedData;
				else
					writeData << " " << UCC::Process::Globals::mergedData[ i ].parsedData << " ";
			}
			return writeData.str();
        }
    }
}
