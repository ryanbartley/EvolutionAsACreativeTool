//
//  Transistor.h
//  GPUDataStructure
//
//  Created by Ryan Bartley on 9/15/13.
//
//

#pragma once

const int CapacitorHigh = 1;
const int CapacitorLow  = 0;

typedef uint8_t _8Capacitors;

class ConTransistors {
public:
    
    ConTransistors() : mHead(new Transistor()), unusedDataLock(new std::mutex) {}
    
    ~ConTransistors()
    {
        while ( mHead->mNext != nullptr ) {
            auto temp = mHead;
            mHead = mHead->mNext;
            delete temp;
        }
    }
    
    struct Transistor
    {
        Transistor() : mCapacitorVals(0), mNext(nullptr), dataUsed(false) {}
        
        Transistor operator+( const Transistor & other )
        {
            Transistor result;
            int CapacitorNum = 0;
            result.mCapacitorVals |= this->mCapacitorVals;
            while( CapacitorNum < 8 ) {
                if( CapacitorHigh << CapacitorNum & other.mCapacitorVals ) {
                    if( CapacitorHigh << CapacitorNum & result.mCapacitorVals ) {
                        result.mCapacitorVals ^= CapacitorHigh << CapacitorNum;
                        int CapacitorCarry = CapacitorNum + 1;
                        while ( CapacitorCarry < 8 ) {
                            if ((CapacitorHigh << CapacitorCarry) & result.mCapacitorVals) {
                                result.mCapacitorVals ^= CapacitorHigh << CapacitorCarry;
                            }
                            else {
                                result.mCapacitorVals ^= CapacitorHigh << CapacitorCarry;
                                CapacitorCarry = 8;
                            }
                            CapacitorCarry++;
                        }
                    }
                    else {
                        result.mCapacitorVals ^= CapacitorHigh << CapacitorNum;
                    }
                }
                CapacitorNum++;
            }
            return result;
        }
      
        uint8_t         mCapacitorVals;
        bool            dataUsed;
        Transistor      *mNext;
    };
    
    void insert()
    {
        auto transistor = new Transistor();
        transistor->mNext = mHead;
        mHead = transistor;
    }
    
    Transistor* unUsed()
    {
        std::lock_guard<std::mutex> lck(*unusedDataLock);
        Transistor* trans = mHead;
        while( trans->mNext != nullptr && trans->dataUsed ) {
            trans = trans->mNext;
        }
        
        if( !trans->dataUsed ) {
            trans->mCapacitorVals = 0;
            trans->dataUsed = true;
            return trans;
        }
        insert();
        trans = trans->mNext;
        trans->dataUsed = true;
        return trans;
    }

private:
    Transistor *mHead;
    std::shared_ptr<std::mutex> unusedDataLock;
};