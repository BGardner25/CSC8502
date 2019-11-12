#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJMesh.h"
#include "../nclgl/heightMap.h"

// generate LIGHTNUM squared lights
#define LIGHTNUM 8

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();
protected:
	// fill g-buffer render pass
	void FillBuffers();
	// lighting render pass
	void DrawPointLights();
	// combination render pass
	void CombineBuffers();

	void GenerateScreenTexture(GLuint& into, bool depth = false);

	Shader* sceneShader;			// shader to fill g-buffers
	Shader* pointLightShader;		// shader to calc lighting
	Shader* combineShader;			// shader to combine it all together

	Light* pointLights;				// array of lighting data
	Mesh* heightMap;				// terrain
	OBJMesh* sphere;				// light volume
	Mesh* quad;						// to draw full screen quad
	Camera* camera;

	float rotation;					// amount to increase rotation by

	GLuint bufferFBO;				// FBO for g-buffer pass
	GLuint bufferColourTex;			// albedo
	GLuint bufferNormalTex;			// normals
	GLuint bufferDepthTex;			// depth

	GLuint pointLightFBO;			// FBO for lighting pass
	GLuint lightEmissiveTex;		// store emissive lighting
	GLuint lightSpecularTex;		// store specular lighting
};

