uniform sampler2D texBase;
uniform float depth;

void main(){
	if(texture2D(texBase, gl_TexCoord[0].xy).a > 0){
		gl_FragColor = vec4(depth, depth, depth, 1.0);
	} else gl_FragColor = vec4(0, 0, 0, 0);
}