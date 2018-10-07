#version 120
uniform sampler2D texBase;
uniform vec4 paintColor;

void main(){
	vec4 baseColor = texture2D(texBase,gl_TexCoord[0].xy);
	gl_FragColor = vec4(paintColor.rgb, baseColor.a);
}