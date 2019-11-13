#include "CubeMap.h"

CubeMap::CubeMap(const char* right, const char* left,
				const char* top, const char* bottom,
				const char* back, const char* front) {
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");

	cubeMap = SOIL_load_OGL_cubemap(right, left, top, bottom, back, front, 
										SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!skyboxShader->LinkProgram() || !cubeMap)
		return;
}

CubeMap::~CubeMap(void) {
	delete skyboxShader;
}