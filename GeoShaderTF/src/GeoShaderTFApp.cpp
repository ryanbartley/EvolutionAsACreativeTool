#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GeoShaderTFApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void GeoShaderTFApp::setup()
{
}

void GeoShaderTFApp::mouseDown( MouseEvent event )
{
}

void GeoShaderTFApp::update()
{
}

void GeoShaderTFApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( GeoShaderTFApp, RendererGl )
