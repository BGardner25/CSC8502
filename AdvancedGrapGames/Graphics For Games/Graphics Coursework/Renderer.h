#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();
protected:
	void DrawSkybox();

	Camera* camera;

	Shader* skyboxShader;

	Mesh* quad;

	GLuint cubeMap;
};

