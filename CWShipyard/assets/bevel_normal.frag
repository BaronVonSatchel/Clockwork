#version 120
uniform sampler2D texBase;
uniform ivec2 texSize;

void main(){
	if(texture2D(texBase, gl_TexCoord[0].xy).r > 0){
		//ivec2 currentPixel = ivec2(gl_TexCoord[0].xy) * texSize;

		vec2 down = vec2(0, 1) / texSize;
		vec2 right = vec2(1, 0) / texSize;

		float t = texture2D(texBase, gl_TexCoord[0].xy - down).r;
		float b = texture2D(texBase, gl_TexCoord[0].xy + down).r;
		float l = texture2D(texBase, gl_TexCoord[0].xy - right).r;
		float r = texture2D(texBase, gl_TexCoord[0].xy + right).r;
		
		float tl = texture2D(texBase, gl_TexCoord[0].xy - down - right).r;
		float tr = texture2D(texBase, gl_TexCoord[0].xy - down + right).r;
		float bl = texture2D(texBase, gl_TexCoord[0].xy + down - right).r;
		float br = texture2D(texBase, gl_TexCoord[0].xy + down + right).r;

		float dx = tr + 2*r + br - (tl + 2*l + bl);
		float dy = bl + 2*b + br - (tl + 2*t + tr);

		gl_FragColor = vec4(normalize(vec3(-0.25 * dx, -0.25 * dy, 1.0f / 255)) / 2 + 0.5, 1);
	} else gl_FragColor = vec4(0, 0, 0, 0);
}