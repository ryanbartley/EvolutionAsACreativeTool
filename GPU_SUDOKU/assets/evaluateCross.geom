#version 150 core
#extension all : warn

layout (points) in;
layout (points, max_vertices = 82) out;

in vec2[] vs_board_percent;

uniform isamplerBuffer tex_mating_info;

layout (stream = 0) out vec2	tf_board_percent;
layout (stream = 1) out int		tf_gene_pool;

void main () {
	
	tf_board_percent = vs_board_percent[0];
	EmitStreamVertex(0);
	EndStreamPrimitive(0);
	
	tf_gene_pool = 10;
	EmitStreamVertex(1);
	EndStreamPrimitive(1);
	
//	int boardindex	  = int(vs_board_percent[0].x * 81);
//	int howManyBoards = int(vs_board_percent[0].y * 10);
//	
//	
////	for( int i = 0; i < howManyBoards; i++ ) {
//		for( int j = 0; j < 81; j++ ) {
//			int mating = texelFetch( tex_mating_info, boardindex + j).w;
//			tf_gene_pool = mating;
//			EmitStreamVertex(1);
//		}
//		EndStreamPrimitive(1);
//	}
	
}