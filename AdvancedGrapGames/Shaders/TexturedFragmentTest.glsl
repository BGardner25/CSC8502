#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void){
	gl_FragColor = texture(diffuseTex, IN.texCoord) * IN.colour;
}