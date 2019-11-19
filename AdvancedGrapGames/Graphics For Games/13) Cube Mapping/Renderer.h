#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
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

	void DrawHeightMap();
	void DrawWater();
	void DrawSkybox();

	SceneNode* rootNode;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	HeightMapPNG* heightMap;
	Mesh* quad;				// for water

	Light* light;
	Camera* camera;

	GLuint cubeMap;
	float waterRotate;
	float heightVal;

	GameTimer gameTimer;
};

