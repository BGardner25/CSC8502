#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	quad = Mesh::GenerateQuad();

	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");

	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
									TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
									TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
									SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!skyboxShader->LinkProgram() || !cubeMap)
		return;
}

Renderer::~Renderer(void) {
	delete quad;
	delete skyboxShader;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();

	SwapBuffers();
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}