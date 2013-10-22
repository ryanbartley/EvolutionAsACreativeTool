#version 150 core
#extension all : warn


// This input vector contains the vertex position in xyz, and the
// mass of the vertex in w
layout (location = 0) in ivec4 position_puzzle;
// This is our connection vector
layout (location = 2) in ivec4 mating_info;

// This is a TBO that will be bound to the same buffer as the
// position_mass input attribute
uniform isamplerBuffer tex_mating_info;

// The outputs of the vertex shader are the same as the inputs
out ivec4 tf_mating_info;

int index_from_puzzle = position_puzzle.w * 81;
int index_to_cur_x_y	= ((position_puzzle.y * 9) + position_puzzle.x) + index_from_puzzle;

int index( vec2 curPoint )
{
	return int(curPoint.x + (curPoint.y * 9));
}

int retrieveIndex( vec2 destination )
{
	return index_from_puzzle + index( destination );
}



void main(void)
{
	int i;
	bool flipToZero = false;
	int x = position_puzzle.x;
	// Traverse y
	int xVal = 1;
	for( i = 0; i < 9; i++ ) {
		int index = retrieveIndex( vec2( x, i ) );
		if( index != index_to_cur_x_y ) {
			vec4 other_m_i = texelFetch( tex_mating_info, index ).xyzw;
			if( other_m_i.w == mating_info.w ) {
				xVal = 0;
				flipToZero = true;
				break;
			}
		}
	}
	
	int y = position_puzzle.y;
	int yVal = 1;
	for( i = 0; i < 9; i++ ) {
		int index = retrieveIndex( vec2( i, y ) );
		if( index != index_to_cur_x_y ) {
			vec4 other_m_i = texelFetch( tex_mating_info, index ).xyzw;
			if( other_m_i.w == mating_info.w ) {
				yVal = 0;
				flipToZero = true;
				break;
			}
		}
	}
	
//	int blockIndex = position_puzzle.z * 9;
	int zVal = 1;
//	for( i = 0; i < 9; i += 3 ) {
//		for( y = 0; y < 3; y++ ) {
//			for( x = 0; x < 3; x++ ) {
//				int index = retrieveIndex( vec2(x, y) );
//				if( index != index_to_cur_x_y ) {
//					vec4 other_m_i = texelFetch( tex_mating_info, index ).xyzw;
//					if( other_m_i.w == mating_info.w) {
//						zVal = 0;
//						flipToZero = true;
//					}
//				}
//			}
//		}
//	}

	tf_mating_info = ivec4(xVal, yVal, zVal, mating_info.w);
//	int index = retrieveIndex( vec2( 8, 9 ) );
//	vec4 numbers = texelFetch( tex_mating_info, index_to_cur_x_y ).xyzw;
	
//
//	tf_mating_info = mating_info;
}
