#version 120
uniform sampler2D texBase;
uniform sampler2D normal;
uniform sampler2D emissive;
uniform vec3 lightVector;
uniform vec3 lightColor;
uniform float ambient;

void main(){

	vec3 lightNorm = normalize(lightVector);
	vec3 n = normalize((texture2D(normal,gl_TexCoord[0].xy).rgb * 2.0) - 1.0);
	vec4 baseColor = texture2D(texBase,gl_TexCoord[0].xy);
	vec4 ems = texture2D(emissive,gl_TexCoord[0].xy);

	//diffuse
	float diffuse = clamp(-dot(lightNorm, n), 0, 1);

	//specular
	vec3 reflected = reflect(lightNorm, n);
	float spec = pow(max(reflected.z, 0.0), 32);

	gl_FragColor = vec4(clamp((baseColor.rgb * lightColor * (diffuse + ambient)) + (lightColor * spec) + ems.rgb, 0, 1), baseColor.a);
}