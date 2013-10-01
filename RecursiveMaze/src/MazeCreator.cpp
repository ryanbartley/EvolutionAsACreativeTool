//
//  MazeCreator.cpp
//  RecursiveMaze
//
//  Created by Ryan Bartley on 9/30/13.
//
//

#include "MazeCreator.h"

void MazeCreator::generate()
{
	killArray();
	maze = new BYTE[sideCellAmount * sideCellAmount];
	memset(maze, 0, sideCellAmount * sideCellAmount);
	currentX = rand() % sideCellAmount; currentY = rand() % sideCellAmount;
}

std::vector<ci::PolyLine2f> MazeCreator::carve()
{
	std::vector<ci::PolyLine2f> mazeLine;
	ci::PolyLine2f singleLine;
	int previousX = 0, previousY = 0;
	while( true )
	{
		if( (currentX > previousX + 2) || (currentY > previousY + 2) ) {
			mazeLine.push_back(singleLine);
			singleLine.getPoints().clear();
		}
		else {
			previousX = currentX;
			previousY = currentY;
		}
		int d = getDirection();
		if( d < NOR ) return mazeLine;
		
		int nx = currentX * CELL_SIZE,
		ny = currentY * CELL_SIZE;
		
		switch( d )
		{
			case NOR:
				maze[currentX + sideCellAmount * currentY] |= NOR; currentY--;
				maze[currentX + sideCellAmount * currentY] = SOU | SOU << 4;
				singleLine.push_back( ci::Vec2f( nx, ny ) );
				singleLine.push_back( ci::Vec2f( nx + CELL_SIZE + 1, ny ) );
				break;
			case EAS:
				maze[currentX + sideCellAmount * currentY] |= EAS; currentX++;
				maze[currentX + sideCellAmount * currentY] = WES | WES << 4;
				singleLine.push_back( ci::Vec2f( nx + CELL_SIZE, ny ) );
				singleLine.push_back( ci::Vec2f( nx + CELL_SIZE, ny + CELL_SIZE + 1) );
				break;
			case SOU:
				maze[currentX + sideCellAmount * currentY] |= SOU; currentY++;
				maze[currentX + sideCellAmount * currentY] = NOR | NOR << 4;
				singleLine.push_back( ci::Vec2f( nx, ny + CELL_SIZE ) );
				singleLine.push_back( ci::Vec2f( nx + CELL_SIZE + 1, ny + CELL_SIZE ) );
				break;
			case WES:
				maze[currentX + sideCellAmount * currentY] |= WES; currentX--;
				maze[currentX + sideCellAmount * currentY] = EAS | EAS << 4;
				singleLine.push_back( ci::Vec2f( nx, ny) );
				singleLine.push_back( ci::Vec2f( nx, ny + CELL_SIZE + 1 ) );
		}
		
	}
}

int MazeCreator::getDirection()
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
		d = ( maze[currentX + sideCellAmount * currentY] & 0xf0 ) >> 4;
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

bool MazeCreator::testDir( int d )
{
	switch( d )
	{
		case NOR: return ( currentY - 1 > -1 && !maze[currentX + sideCellAmount * ( currentY - 1 )] );
		case EAS: return ( currentX + 1 < sideCellAmount && !maze[currentX + 1 + sideCellAmount * currentY] );
		case SOU: return ( currentY + 1 < sideCellAmount && !maze[currentX + sideCellAmount * ( currentY + 1 )] );
		case WES: return ( currentX - 1 > -1 && !maze[currentX - 1 + sideCellAmount * currentY] );
	}
	return false;
}