#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "Include.h"

template< typename T >
UINT GetArraySize( T* tArr )
{
    if( sizeof( T ) != 0 && sizeof( tArr ) != 0 )
        return ( sizeof( tArr ) * sizeof( T ) );
    else
        return ( 0 );
}

#endif // UTILITY_H_INCLUDED
