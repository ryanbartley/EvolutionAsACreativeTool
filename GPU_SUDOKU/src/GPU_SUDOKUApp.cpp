#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vao.h"
#include "cinder/gl/Vbo.h"

#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

typedef std::vector<std::pair<gl::VaoRef, std::vector<gl::VboRef> > > PingPongBuffers;

enum BUFFER_TYPE {
	POSITIONANDPUZZLE = 0,
	SCREENPOSITION = 1,
	MATINGINFO = 2,
	PUZZLEINCREMENT = 3
};

enum
{
	CELLS_X			= 90,
	CELLS_Y			= 90,
	CELLS_TOTAL		= (CELLS_X * CELLS_Y),
	CELLS_IN_BLOCK  = 9,
	CELLS_IN_PUZZLE = 81,
	TOTAL_PUZZLES	= 100
};

struct SudokuPiece {
	float row;
	float column;
	float block;
	float puzzle;
	float value;
	float percentage;
};

class GPU_SUDOKUApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void setupBoards();
	void loadShaders();
	void keyDown( KeyEvent event );
	
	PingPongBuffers mBuffers;
	gl::GlslProgRef mFitnessGlsl, mCrossover, mRenderGlsl;
	int				mIterationsPerFrame, mIterationIndex;
	GLuint			mMutTbo[2], transformFeed[2];
	bool			fitness, firstTimePrint;
	GLuint			transform[2];
};

void GPU_SUDOKUApp::setup()
{
	fitness = false;
	firstTimePrint = true;
	setupBoards();
	loadShaders();
}

void GPU_SUDOKUApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'r' )
		loadShaders();
	else if( event.getChar() == 'b' )
		setupBoards();
	else if( event.getChar() == 'f') {
		fitness = !fitness;
		firstTimePrint = true;
	}
}

void GPU_SUDOKUApp::mouseDown( MouseEvent event )
{
}

void GPU_SUDOKUApp::update()
{
	if( fitness ) {
		mFitnessGlsl->bind();
//		GLuint query;
//		glGenQueries(1, &query);
		gl::enable( GL_RASTERIZER_DISCARD );
	

		mBuffers[mIterationIndex & 1].first->bind();
		glBindTexture( GL_TEXTURE_BUFFER, mMutTbo[mIterationIndex & 1] );
		mIterationIndex++;
		glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, mBuffers[mIterationIndex & 1].second[MATINGINFO]->getId() );
		
//		glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
		glBeginTransformFeedback(GL_POINTS);
		
		
		glDrawArrays(GL_POINTS, 0, CELLS_TOTAL);
		glEndTransformFeedback();
//		glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
		
//		GLuint vertices;
//		glGetQueryObjectuiv(query, GL_QUERY_RESULT, &vertices);
		
		glFlush();
		if( firstTimePrint ) {
			GLint matefeedback[81*5];
			glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(matefeedback), matefeedback);
		
			for (int i = 0; i < 81 * 5; i+=4) {
				printf("Mating Info:");
				for( int j = i; j < i + 4; j++ )
					printf(" %i ", matefeedback[j]);
				printf( "\n" );
			}
			firstTimePrint = false;
		}
		
//		printf("%u vertices written!\n", vertices);
	
		gl::disable( GL_RASTERIZER_DISCARD );
	}
	
}

void GPU_SUDOKUApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::viewport( 0, 0, getWindowWidth(), getWindowHeight() );
	mRenderGlsl->bind();
	
	glPointSize(3.0f);
	glDrawArrays( GL_POINTS, 0, 81 );
	
}

void GPU_SUDOKUApp::loadShaders()
{
	static const char * tf_varyings[] = {
		"tf_mating_info"
	};

	gl::GlslProg::Format fitnessFormat;
	fitnessFormat.vertex( loadAsset( "fitness.vert" ) ).fragment( loadAsset( "fitness.frag" ) ).feedbackFormat( GL_SEPARATE_ATTRIBS ).feedbackVaryings( tf_varyings, 1 ).transformFeedback();
	
	try {
		if( mFitnessGlsl ) mFitnessGlsl.reset();
		mFitnessGlsl = gl::GlslProg::create( fitnessFormat );
	} catch ( gl::GlslNullProgramExc ex ) {
		cout << "Fitness Shader failure: " << ex.what() << endl;
//		quit();
	} catch ( gl::GlslProgCompileExc ex ) {
		cout << "Fitness Shader failure: " << ex.what() << endl;
//		quit();
	}
	
	gl::GlslProg::Format renderFormat;
	renderFormat.vertex( loadAsset( "render.vert" ) )
	.fragment( loadAsset( "render.frag" ) );
	
	try {
		if( mRenderGlsl ) mRenderGlsl.reset();
		mRenderGlsl = gl::GlslProg::create( renderFormat );
	} catch ( gl::GlslProgCompileExc ex ) {
		cout << "Render Shader failure: " << ex.what() << endl;
//		quit();
	}
}

void GPU_SUDOKUApp::setupBoards()
{
	int i, j, k, puzzle = 0, moveX = 0, moveY = 0;
	
	Vec4i * positionAndPuzzle	= new Vec4i[CELLS_TOTAL];
	Vec4f * screenPosition		= new Vec4f[CELLS_TOTAL];
	Vec4i * matingInfo			= new Vec4i[CELLS_TOTAL];
	GLuint * puzzleIncrement	= new GLuint[100];
	
	for( i = 0; i < CELLS_TOTAL; i+=CELLS_IN_PUZZLE ) {
		int x = 0, y = 0, block = -1;
		
		for( j = 0; j < CELLS_IN_PUZZLE; j+= CELLS_IN_BLOCK ) {
			for( k = 0; k < CELLS_IN_BLOCK; k++ ) {
				if( x % 3 == 0 ) block++;
				positionAndPuzzle[i + j + k] = Vec4i( x, y, block, puzzle );
				screenPosition[i + j + k] = Vec4f( lmap((float)(moveX * 9) + x, 0.0f, 89.0f, -0.98f, 0.98f), //Correct
												   lmap((float)(moveY * 9) + y, 0.0f, 89.0f, 0.98f, -0.98f), //Swapped
												   0.0f,
												   1.0f);
				matingInfo[i + j + k] = Vec4i( 1, 1, 1, rand() % 9 + 1 );
				x++;
			}
			y++;
			if( y % 3 != 0 )
				block -= 3;
			x = 0;
		}
		puzzle++;
		if( moveX < 9 )
			moveX++;
		else
			moveX = 0;
		if( puzzle % 10 == 0 && moveY < 9 )
			moveY++;
	}
	
	for( int i = 0; i < TOTAL_PUZZLES; i++ ) {
		puzzleIncrement[i] = 0;
	}
	
	mBuffers.resize(2);

	for( int i = 0; i < 2; i++ ) {
		mBuffers[i].first = gl::Vao::create();
		mBuffers[i].first->bind();
		
		mBuffers[i].second.resize(4);
		
		if( i < 1 ) {
			// This info will never change so we just write it once and link it to the other for use
			mBuffers[i].second[POSITIONANDPUZZLE] = gl::Vbo::create( GL_ARRAY_BUFFER, CELLS_TOTAL * sizeof(Vec4i), positionAndPuzzle, GL_STATIC_DRAW );
			mBuffers[i].second[POSITIONANDPUZZLE]->bind();
			glVertexAttribIPointer( 0, 4, GL_INT, 0, nullptr );
			gl::enableVertexAttribArray(0);
	
			mBuffers[i].second[SCREENPOSITION] = gl::Vbo::create( GL_ARRAY_BUFFER, CELLS_TOTAL * sizeof(Vec4f), screenPosition, GL_STATIC_DRAW );
			mBuffers[i].second[SCREENPOSITION]->bind();
			gl::vertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, nullptr );
			gl::enableVertexAttribArray(1);
		}
		else {
			mBuffers[i].second[POSITIONANDPUZZLE] = mBuffers[0].second[POSITIONANDPUZZLE];
			mBuffers[i].second[POSITIONANDPUZZLE]->bind();
			glVertexAttribIPointer( 0, 4, GL_INT, 0, nullptr );
			gl::enableVertexAttribArray(0);
			
			mBuffers[i].second[SCREENPOSITION] = mBuffers[0].second[SCREENPOSITION];
			mBuffers[i].second[SCREENPOSITION]->bind();
			gl::vertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, nullptr );
			gl::enableVertexAttribArray(1);
		}
		// THIS HOWEVER WILL CHANGE SO I NEED TO Buffer it and use Dynamic Copy
		mBuffers[i].second[MATINGINFO] = gl::Vbo::create( GL_ARRAY_BUFFER, CELLS_TOTAL * sizeof(Vec4i), matingInfo, GL_DYNAMIC_COPY );
		mBuffers[i].second[MATINGINFO]->bind();
		glVertexAttribIPointer( 2, 4, GL_INT, 0, NULL );
		gl::enableVertexAttribArray(2);
		
	}
	
	delete [] positionAndPuzzle;
	delete [] screenPosition;
	delete [] matingInfo;
	
	glGenTextures(2, mMutTbo);
	glBindTexture( GL_TEXTURE_BUFFER, mMutTbo[0] );
	glTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA32I, mBuffers[0].second[MATINGINFO]->getId() );
	glBindTexture( GL_TEXTURE_BUFFER, mMutTbo[1] );
	glTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA32I, mBuffers[1].second[MATINGINFO]->getId() );
}

CINDER_APP_NATIVE( GPU_SUDOKUApp, RendererGl )




