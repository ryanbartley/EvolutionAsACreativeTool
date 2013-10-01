#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "MazeCreator.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RecursiveMazeApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	MazeCreatorRef mc;
	std::vector<ci::PolyLine2f> maze;
};

void RecursiveMazeApp::setup()
{
	mc = MazeCreatorRef( new MazeCreator( 60 ) );
	maze = mc->carve();
	
}

void RecursiveMazeApp::mouseDown( MouseEvent event )
{
}

void RecursiveMazeApp::update()
{
}

void RecursiveMazeApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::pushMatrices();
	gl::setMatricesWindow( getWindowSize() );
	gl::multModelView( Matrix44f::createScale( Vec2i( 1, 1 ) ) );
	for( auto line = maze.begin(); line != maze.end(); ++line ) {
		line->setClosed(false);
		gl::draw(*line);
	}
	gl::popMatrices();
	
}

CINDER_APP_NATIVE( RecursiveMazeApp, RendererGl )
