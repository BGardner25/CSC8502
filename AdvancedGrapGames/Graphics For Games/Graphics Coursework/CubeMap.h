#pragma once
#include "../nclgl/OGLRenderer.h"

class CubeMap {
public:
	CubeMap(const char* right, const char* left,
			const char* top, const char* bottom,
			const char* back, const char* front);
	~CubeMap(void);

	Shader* GetShader() { return skyboxShader; }
protected:
	void DrawSkybox();

	Shader* skyboxShader;
	GLuint cubeMap;
};

