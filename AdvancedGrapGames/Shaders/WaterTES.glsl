#version 400

layout(quads, cw) in;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float waterRotate;
uniform float time;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN[];

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} OUT;

vec2 mixVec2(vec2 a, vec2 b, vec2 c, vec2 d) {
	vec2 p0 = mix(a, c, gl_TessCoord.x);
	vec2 p1 = mix(b, d, gl_TessCoord.x);

	return mix(p0, p1, gl_TessCoord.y);
}

vec3 mixVec3(vec3 a, vec3 b, vec3 c, vec3 d) {
	vec3 p0 = mix(a, c, gl_TessCoord.x);
	vec3 p1 = mix(b, d, gl_TessCoord.x);

	return mix(p0, p1, gl_TessCoord.y);
}

void main(void) {
	OUT.texCoord = mixVec2(IN[0].texCoord,
							IN[1].texCoord,
							IN[2].texCoord,
							IN[3].texCoord);

	vec3 normal = mixVec3(IN[0].normal,
							IN[1].normal,
							IN[2].normal,
							IN[3].normal);
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal)); 

	vec3 pos = mixVec3(gl_in[0].gl_Position.xyz,
						gl_in[1].gl_Position.xyz,
						gl_in[2].gl_Position.xyz,
						gl_in[3].gl_Position.xyz);

	vec4 worldPos = modelMatrix * vec4(pos.x, pos.y, pos.z, 1.0);
	worldPos.y += sin((time / 300) + ((worldPos.x + worldPos.z))) * sin(waterRotate * 2 + (worldPos.z / 20)) * 50;
	
	OUT.worldPos = vec3(worldPos);

	gl_Position = projMatrix * viewMatrix * worldPos;
}