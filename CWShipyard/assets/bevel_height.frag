#version 120
uniform sampler2D texBase;
uniform ivec2 texSize;
uniform int limit;

void main(){
	if(texture2D(texBase, gl_TexCoord[0].xy).a > 0){
		vec2 down = vec2(0, 1) / texSize;
		vec2 right = vec2(1, 0) / texSize;
		float shortest = limit;
		for(int i = -limit; i <= limit; i++){
			for(int j = -limit; j <= limit; j++){
				ivec2 test = ivec2(i, j);
				if(texture2D(texBase, gl_TexCoord[0].xy + (i * right) + (j * down)).a == 0){
					//shortest = min(shortest, length((vec2)test));
					shortest = min(shortest, abs(i) + abs(j));
				}
			}
		}
		float height = shortest / 255;
		gl_FragColor = vec4(height, height, height, 1);
	} else gl_FragColor = vec4(0, 0, 0, 1);
}