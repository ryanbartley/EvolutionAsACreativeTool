//
//  GPU.cpp
//  GPUDataStructure
//
//  Created by Ryan Bartley on 9/15/13.
//
//

#include "GPU.h"

void GPU::add( uint8_t x, uint8_t y, uint8_t & outGoing )
{
    auto tran1 = mTransistors.unUsed();
    auto tran2 = mTransistors.unUsed();
    auto result = mTransistors.unUsed();
    
    tran1->mCapacitorVals |= x;
    tran2->mCapacitorVals |= y;
    
    *result = *tran1 + *tran2;
    outGoing = result->mCapacitorVals;
    
    tran1->dataUsed = false;
    tran2->dataUsed = false;
    result->dataUsed = false;
}

void GPU::bufferData( uint8_t x[], uint8_t y[], int numComparisons )
{
    auto result = new uint8_t[numComparisons];
    memset(result, numComparisons, 0);
    for( int i = 0; i < numComparisons; ++i ) {
        mPipeline.dispatch( std::bind( &GPU::add, this, x[i], y[i], std::ref(result[i]) ) );
    }
    callback( result, numComparisons );
}