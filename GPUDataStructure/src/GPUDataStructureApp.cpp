#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "GPU.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GPUDataStructureApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void getGPUResultBuffer( uint8_t * result, int numResults );
    
    GPU *mGpu;
};

void GPUDataStructureApp::setup()
{
    mGpu = new GPU( std::bind( &GPUDataStructureApp::getGPUResultBuffer, this, std::placeholders::_1, std::placeholders::_2 ) );
    
    uint8_t x[] = { 1, 2, 3, 4, 12, 23 };
    uint8_t y[] = { 5, 6, 7, 8, 155, 19 };
    
    mGpu->bufferData( x, y, 6 );
}

void GPUDataStructureApp::getGPUResultBuffer( uint8_t * result, int numResults )
{
    for(int i = 0; i < numResults; ++i)
        cout << (int)result[i] << endl;
    
    delete [] result;
}

void GPUDataStructureApp::mouseDown( MouseEvent event )
{
    delete    mGpu;
}

void GPUDataStructureApp::update()
{
}

void GPUDataStructureApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( GPUDataStructureApp, RendererGl )
