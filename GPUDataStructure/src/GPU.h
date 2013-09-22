//
//  GPU.h
//  GPUDataStructure
//
//  Created by Ryan Bartley on 9/15/13.
//
//

#pragma once

#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <functional>
#include "Transistor.h"

const long transNum = 100;

class GPU {
public:
    GPU(std::function<void(uint8_t *, int)> callback) : mPipeline(), mWork( mPipeline ), mTransistors(), callback(callback)
    {
        for( int i = 0; i < transNum; ++i) {
            mTransistors.insert();
        }
        
        for( int i = 0; i < 3; ++i) {
            mParallelProcesses.push_back( std::thread( boost::bind( &boost::asio::io_service::run, &mPipeline ) ) );
        }
    }
    
    ~GPU()
    {
        mPipeline.stop();
        for( auto threadIt = mParallelProcesses.begin(); threadIt < mParallelProcesses.end(); ++threadIt) {
            threadIt->join();
        }
    }
    void add( uint8_t x, uint8_t y, uint8_t & outGoing );
    
    void bufferData( uint8_t x[], uint8_t y[], int numComparisons );
    
private:
    ConTransistors              mTransistors;
    std::vector<std::thread>    mParallelProcesses;
    boost::asio::io_service     mPipeline;
    boost::asio::io_service::work mWork;
    std::function<void(uint8_t *, int)> callback;
};


