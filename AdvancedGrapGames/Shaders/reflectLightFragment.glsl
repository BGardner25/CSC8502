#version 150 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float lightRadius;

in Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} IN;

out vec4 fragColour;

// water reflection for cube map and per pixel lighting. also changed alpha so water is slightly transparent
void main(void) {
    vec4 diffuse = texture(diffuseTex, IN.texCoord);
    vec3 reflectIncident = normalize(IN.worldPos - cameraPos);
    vec3 incident = normalize(lightPos - IN.worldPos);

	float lambert = max(0.0, dot(incident, IN.normal));

    float dist = length(lightPos - IN.worldPos);
    float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	vec3 viewDir = normalize(cameraPos - IN.worldPos);
    vec3 halfDir = normalize(incident + viewDir);

    vec4 reflection = texture(cubeTex, reflect(reflectIncident, normalize(IN.normal)));

	float rFactor = max(0.0, dot(halfDir, IN.normal));
    float sFactor = pow(rFactor, 50.0);
	vec3 colour = (diffuse.rgb * lightColour.rgb);
	colour += (lightColour.rgb * sFactor) * 0.33;
    
	fragColour = vec4((colour * atten * lambert), diffuse.a * 0.7);
	fragColour.rgb += ((diffuse.rgb + reflection.rgb) * lightColour.rgb) * 0.25;
}