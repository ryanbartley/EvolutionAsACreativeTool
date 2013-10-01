//
//  MazeCreator.h
//  RecursiveMaze
//
//  Created by Ryan Bartley on 9/30/13.
//
//

#pragma once



enum directions { NONE, NOR = 1, EAS = 2, SOU = 4, WES = 8 };
typedef uint8_t BYTE;
const int CELL_SIZE = 8;

class MazeCreator {

public:
    MazeCreator( int side ) : maze(nullptr), numCells(side)
    {
        generate();
        display();
    }
        
    ~MazeCreator() { killArray(); }
        
private:
    void generate()
    {
        killArray();
        maze = new BYTE[numCells * numCells];
        memset(maze, 0, numCells * numCells);
        currentX = rand() % numCells; currentY = rand() % numCells;
        carve();
    }
    
    void carve()
    {
        while( true )
        {
            int d = getDirection();
            if( d < NOR ) return;
            
            switch( d )
            {
                case NOR:
                    maze[currentX + numCells * currentY] |= NOR; currentY--;
                    maze[currentX + numCells * currentY] = SOU | SOU << 4;
                    break;
                case EAS:
                    maze[currentX + numCells * currentY] |= EAS; currentX++;
                    maze[currentX + numCells * currentY] = WES | WES << 4;
                    break;
                case SOU:
                    maze[currentX + numCells * currentY] |= SOU; currentY++;
                    maze[currentX + numCells * currentY] = NOR | NOR << 4;
                    break;
                case WES:
                    maze[currentX + numCells * currentY] |= WES; currentX--;
                    maze[currentX + numCells * currentY] = EAS | EAS << 4;
            }
        }
    }
    
    void display()
    {
        for( int y = 0; y < numCells; y++ )
        {
            int yy = y * numCells;
            for( int x = 0; x < numCells; x++ )
            {
                BYTE b = maze[x + yy];
                int nx = x * CELL_SIZE,
                ny = y * CELL_SIZE;
                
                if( !( b & NOR ) )
                {
                    //MoveToEx( dc, nx, ny, NULL );
                    //LineTo( dc, nx + CELL_SIZE + 1, ny );
                }
                if( !( b & EAS ) )
                {
                    //MoveToEx( dc, nx + CELL_SIZE, ny, NULL );
                    //LineTo( dc, nx + CELL_SIZE, ny + CELL_SIZE + 1 );
                }
                if( !( b & SOU ) )
                {
                    //MoveToEx( dc, nx, ny + CELL_SIZE, NULL );
                    //LineTo( dc, nx + CELL_SIZE + 1, ny + CELL_SIZE );
                }
                if( !( b & WES ) )
                {
                    //MoveToEx( dc, nx, ny, NULL );
                    //LineTo( dc, nx, ny + CELL_SIZE + 1 );
                }
            }
        }
    }

    int getDirection()
    {
        int d = 1 << rand() % 4;
        while( true )
        {
            for( int x = 0; x < 4; x++ )
            {
                if( testDir( d ) ) return d;
                d <<= 1;
                if( d > 8 ) d = 1;
            }
            d = ( maze[currentX + numCells * currentY] & 0xf0 ) >> 4;
            if( !d ) return -1;
            switch( d )
            {
                case NOR: currentY--; break;
                case EAS: currentX++; break;
                case SOU: currentY++; break;
                case WES: currentX--; break;
            }
            d = 1 << rand() % 4;
        }
    }

    bool testDir( int d )
    {
        switch( d )
        {
            case NOR: return ( currentY - 1 > -1 && !maze[currentX + numCells * ( currentY - 1 )] );
            case EAS: return ( currentX + 1 < numCells && !maze[currentX + 1 + numCells * currentY] );
            case SOU: return ( currentY + 1 < numCells && !maze[currentX + numCells * ( currentY + 1 )] );
            case WES: return ( currentX - 1 > -1 && !maze[currentX - 1 + numCells * currentY] );
        }
        return false;
    }

    void killArray() { if( maze ) delete [] maze; }

BYTE*    maze;
int      numCells, currentX, currentY;
};
