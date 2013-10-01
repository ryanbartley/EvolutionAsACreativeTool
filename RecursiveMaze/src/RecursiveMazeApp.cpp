#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RecursiveMazeApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void RecursiveMazeApp::setup()
{
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
}

CINDER_APP_NATIVE( RecursiveMazeApp, RendererGl )
