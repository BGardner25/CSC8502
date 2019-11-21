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
	void DrawPointLight();
	void FillBuffers();
	void CombineBuffers();

	void GenerateScreenTexture(GLuint& into, bool depth = false);
	void SetupPointLights();
	void CameraPath();

	SceneNode* rootNode;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	Shader* fontShader;
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* sceneShader;
	Shader* combineShader;
	Shader* pointLightShader;
	Shader* cylinderShader;
	Shader* cylinderTwoShader;
	Shader* cubeShader;

	HeightMapPNG* heightMap;
	Mesh* quad;				// for water

	Light* light;
	Light* pointLight;
	Camera* camera;

	GLuint cubeMap;

	float waterRotate;		// to animate water
	float heightVal;		// to raise terrain
	float totalTime = 0.0f;
	const Vector3 START_POS = Vector3(19210.0f, 981.0f, 34512.0f);

	OBJMesh* cylinder;
	OBJMesh* cube;
	OBJMesh* lightObj;

	void DrawText(const std::string& text, const Vector3& position, const float size = 10.0f, const bool perspective = false);
	Font* basicFont;

	Window* w;

	// fps counter
	float fps;
	float lastTime;
	float currentTime;
	float timePassed;
	float frames;

	float rotation;

	GLuint bufferFBO;
	GLuint bufferColourTex;
	GLuint bufferNormalTex;
	GLuint bufferDepthTex;

	GLuint pointLightFBO;
	GLuint lightEmissiveTex;
	GLuint lightSpecularTex;

	// camera movement
	bool autoMove;
	float cameraTime;
	const float MOVE_SPEED = 1.0;
	const Vector3 MOVE_X = Vector3(MOVE_SPEED, 0, 0);
	const Vector3 MOVE_Y = Vector3(0, MOVE_SPEED * 0.25, 0);
	const Vector3 MOVE_Z = Vector3(0, 0, MOVE_SPEED);
};

