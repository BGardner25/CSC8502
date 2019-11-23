#version 400

layout(triangles, cw) in;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float heightVal;

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

vec2 vec2Mix(vec2 v0, vec2 v1, vec2 v2) {
	vec2 p0 = gl_TessCoord.x * v0;
	vec2 p1 = gl_TessCoord.y * v1;
	vec2 p2 = gl_TessCoord.z * v2;

	return p0 + p1 + p2;
}

vec3 vec3Mix(vec3 v0, vec3 v1, vec3 v2) {
	vec3 p0 = gl_TessCoord.x * v0;
	vec3 p1 = gl_TessCoord.y * v1;
	vec3 p2 = gl_TessCoord.z * v2;

	return p0 + p1 + p2;
}

void main(void) {
	vec3 pos = vec3Mix(gl_in[0].gl_Position.xyz,
						gl_in[1].gl_Position.xyz,
						gl_in[2].gl_Position.xyz);

	vec3 cubeNormalise = normalize(pos);
	
	vec4 worldPos = modelMatrix * vec4(pos, 1) + vec4(mix(pos, cubeNormalise * heightVal, 1), 1.0);
	
	OUT.texCoord = vec2Mix(IN[0].texCoord,
							IN[1].texCoord,
							IN[2].texCoord);

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec3 normal =  normalize(vec3Mix(IN[0].normal,
							IN[1].normal,
							IN[2].normal));

	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.worldPos = worldPos.xyz;

	gl_Position = projMatrix * viewMatrix * worldPos;
}