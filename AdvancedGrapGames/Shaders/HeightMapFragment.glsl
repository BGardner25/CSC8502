#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D texGrass;
uniform sampler2D texSnow;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;
uniform float randomVal;

in Vertex {
    vec3 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} IN;

out vec4 fragColour;

// blend function sourced from Andrey Mishkinis: https://www.gamasutra.com/blogs/AndreyMishkinis/20130716/196339/Advanced_Terrain_Texture_Splatting.php
// Last accessed 19/11/2019
vec3 blendTex(vec4 t0, float a0, vec4 t1, float a1) {
	float depth = 0.2;
	float ma = max(t0.a + a0, t1.a + a1) - depth;

	float b0 = max(t0.a + a0 - ma, 0);
	float b1 = max(t1.a + a1 - ma, 0);

	return (t0.rgb * b0 + t1.rgb * b1) / (b0 + b1);
}

void main (void) {
	float gradient = clamp((dot(normalize(IN.normal), vec3(0, 1, 0)) - 0.5), 0.0, 1.0);

	// blends the rock and grass textures with rock being applied to steeper gradients
	vec3 blendRockGrass = blendTex(texture(diffuseTex, IN.texCoord), 0.2, texture(texGrass, IN.texCoord), gradient);
	vec3 blendRockSnow = blendTex(vec4(blendRockGrass, 1.0), 0.05, texture(texSnow, IN.texCoord), gradient);

	vec4 diffuse = vec4(0, 0, 0, 0);
	if(IN.worldPos.y > 2600 && gradient > 0.12) {
		diffuse.rgb = blendRockSnow + vec3(0.7,0.7,0.7);
	}
	else {
		diffuse.rgb = blendRockGrass;
	}
	//diffuse.rgb = blendRockGrass;
	diffuse.a = 1.0;

    vec3 incident = normalize(lightPos - IN.worldPos);
    float lambert = max(0.0, dot(incident, IN.normal));
    float dist = length(lightPos - IN.worldPos);
    float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
    vec3 viewDir = normalize(cameraPos - IN.worldPos);
    vec3 halfDir = normalize(incident + viewDir);

    float rFactor = max(0.0, dot(halfDir, IN.normal));
    float sFactor = pow(rFactor, 50.0);
    vec3 colour = (diffuse.rgb * lightColour.rgb);
    colour += (lightColour.rgb * sFactor) * 0.33;
    fragColour = vec4(colour * atten * lambert, diffuse.a);
    fragColour.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
}