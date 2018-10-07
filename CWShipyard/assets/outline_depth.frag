#version 120
uniform sampler2D texBase;
uniform sampler2D depthBuffer;
uniform ivec2 texSize;
uniform vec4 outlineColor;

void main(){
	float current = texture2D(depthBuffer, gl_TexCoord[0].xy).r;
	ivec2 currentPixel = ivec2(gl_TexCoord[0].xy) * texSize;
	if(texelFetch(depthBuffer, currentPixel + ivec2(1, 0), 0).r > current) gl_FragColor = outlineColor;
	else if(texelFetch(depthBuffer, currentPixel + ivec2(-1, 0), 0).r > current) gl_FragColor = outlineColor;
	else if(texelFetch(depthBuffer, currentPixel + ivec2(0, 1), 0).r > current) gl_FragColor = outlineColor;
	else if(texelFetch(depthBuffer, currentPixel + ivec2(0, -1), 0).r > current) gl_FragColor = outlineColor;
	else gl_FragColor = texture2D(texBase, gl_TexCoord[0].xy);
}