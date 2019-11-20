#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/OBJMesh.h"
#include "../nclgl/TextMesh.h"
#include "HeightMapPNG.h"
#include <algorithm>

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	
	void DrawUI();
	void DrawSkybox();
	void DrawHeightMap();
	void DrawWater();
	void DrawCylinder();
	void DrawCylinderPointLight();

	SceneNode* rootNode;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	Shader* fontShader;
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* pointLightShader;

	HeightMapPNG* heightMap;
	Mesh* quad;				// for water

	Light* light;
	Light* pointLightCylinder;
	Camera* camera;

	GLuint cubeMap;

	float waterRotate;		// to animate water
	float heightVal;		// to raise terrain
	float totalTime = 0.0f;
	const Vector3 START_POS = Vector3(19210.0f, 981.0f, 34512.0f);

	OBJMesh* cylinder;
	Shader* cylinderShader;

	void DrawText(const std::string& text, const Vector3& position, const float size = 10.0f, const bool perspective = false);
	Font* basicFont;

	Window* w;
	float fps;
	float lastTime;
	float currentTime;
	float timePassed;
	float frames;
};

