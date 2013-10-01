//
//  MazeCreator.h
//  RecursiveMaze
//
//  Created by Ryan Bartley on 9/30/13.
//
//

#pragma once

#include "cinder/PolyLine.h"
#include "cinder/Vector.h"

enum directions { NONE, NOR = 1, EAS = 2, SOU = 4, WES = 8 };
typedef uint8_t BYTE;
const int CELL_SIZE = 8;

typedef std::shared_ptr<class MazeCreator> MazeCreatorRef;

class MazeCreator {

public:
    MazeCreator( int sideAmount ) : maze(nullptr), sideCellAmount(sideAmount)
    {
        generate();
    }
        
    ~MazeCreator() { killArray(); }
	std::vector<ci::PolyLine2f>	carve();
        
private:
    void	generate();
    
    int		getDirection();
    bool	testDir( int d );
	BYTE*	getMaze() { return maze; }

    void killArray() { if( maze ) delete [] maze; }

BYTE*    maze;
int      sideCellAmount, currentX, currentY;
};
