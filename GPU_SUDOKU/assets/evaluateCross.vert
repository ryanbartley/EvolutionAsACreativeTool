#version 150 core
#extension all : warn

// make sure to flip the texture to the
// newly created one
uniform isamplerBuffer tex_mating_info;

out vec2 vs_board_percent;

void main() {
	int index = gl_VertexID * 81;
	int total = 0;
	for( int i = index; i < index+81; i++) {
		ivec3 piece = texelFetch( tex_mating_info, i ).xyz;
		total += piece.x + piece.y + piece.z;
	}

	vs_board_percent = vec2( float(gl_VertexID), float(total) / 243.0);
//	tf_board_percent = vec2( 1, 2 );
}