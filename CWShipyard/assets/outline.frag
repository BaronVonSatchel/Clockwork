#version 120
uniform sampler2D texBase;
uniform ivec2 texSize;
uniform vec4 outlineColor;
uniform float threshold;

void main(){
	vec4 color = texture2D(texBase, gl_TexCoord[0].xy);
	if(color.a < threshold) {
		vec2 down = vec2(0, 1) / texSize;
		vec2 right = vec2(1, 0) / texSize;

		ivec2 currentPixel = ivec2(gl_TexCoord[0].xy) * texSize;
		if(texture2D(texBase, gl_TexCoord[0].xy - down).a > 0) gl_FragColor = outlineColor;
		else if(texture2D(texBase, gl_TexCoord[0].xy + down).a > 0) gl_FragColor = outlineColor;
		else if(texture2D(texBase, gl_TexCoord[0].xy - right).a > 0) gl_FragColor = outlineColor;
		else if(texture2D(texBase, gl_TexCoord[0].xy + right).a > 0) gl_FragColor = outlineColor;
		else gl_FragColor = color;
	} else gl_FragColor = color;
}