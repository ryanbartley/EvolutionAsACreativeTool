#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Vao.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

typedef std::vector<std::pair<gl::VaoRef, std::vector<gl::VboRef> > > PingPongBuffers;

class GeoShaderTFApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void update();
	void draw();
	void setupBuffers();
	void loadShaders();
	
	gl::GlslProgRef	mGlsl;
	PingPongBuffers mBuffers;
};

void GeoShaderTFApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'r' )
		loadShaders();
}

void GeoShaderTFApp::setupBuffers()
{
	float * numbers = new float[3];
	
	for( int i = 0; i < 3; i++ ) {
		numbers[i] = 0.0f;
	}
	
	
}

void GeoShaderTFApp::loadShaders()
{
	
}

void GeoShaderTFApp::setup()
{
	loadShaders();
	setupBuffers();
}

void GeoShaderTFApp::mouseDown( MouseEvent event )
{
}

void GeoShaderTFApp::update()
{
	glDrawArraysInstanced( GL_POINTS, 0, 81, 100 );
}

void GeoShaderTFApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( GeoShaderTFApp, RendererGl )
