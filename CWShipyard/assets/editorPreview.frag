#version 120
uniform sampler2D texBase;
uniform vec4 previewColor;

void main(){
	vec4 baseColor = texture2D(texBase,gl_TexCoord[0].xy);
	gl_FragColor = baseColor * previewColor;
}