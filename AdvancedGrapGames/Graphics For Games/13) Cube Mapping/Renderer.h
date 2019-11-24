#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/OBJMesh.h"
#include "../nclgl/TextMesh.h"
#include "HeightMapPNG.h"
#include <algorithm>
#include "Cube.h"
#include "Dragon.h"

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
	void DrawPointLight();
	void FillBuffers();
	void CombineBuffers();

	void GenerateScreenTexture(GLuint& into, bool depth = false);
	void SetupPointLights();
	void CameraPath(float msec);

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
	Shader* waterShader;

	HeightMapPNG* heightMap;
	Mesh* quad;				// for water
	Mesh* skyQuad;

	Light* light;
	Light* pointLight;
	Camera* camera;

	GLuint cubeMap;

	float waterRotate;		// to animate water
	float heightVal;		// to raise terrain
	float totalTime = 0.0f;
	const Vector3 START_POS = Vector3(19210.0f, 981.0f, 34512.0f);
	const Vector3 POINT_2 = Vector3(19874.3, 1371.43, 33257.3);
	const Vector3 POINT_3 = Vector3(19331.6, 1371.43, 31398.8);
	const Vector3 POINT_4 = Vector3(18294.3, 1371.43, 29495);

	const Vector3 POINT_5 = Vector3(16021.1, 1371.43, 28640);
	const Vector3 POINT_6 = Vector3(15890.9, 1371.43, 26756.1);
	const Vector3 POINT_7 = Vector3(16676.6, 1371.43, 25251);

	const Vector3 POINT_8 = Vector3(18280.7, 2384.82, 23415.5);
	const Vector3 POINT_9 = Vector3(20954.9, 3244.75, 22409.4);
	const Vector3 POINT_10 = Vector3(22238.6, 3244.75, 20344.2);

	const Vector3 POINT_11 = Vector3(21244.9, 3484.04, 18116.2);
	const Vector3 POINT_12 = Vector3(18997.3, 2176.84, 14779.2);
	const Vector3 POINT_13 = Vector3(17947.3, 1734.25, 12823.3);

	const Vector3 POINT_14 = Vector3(16641.5, 1734.25, 11657.2);
	const Vector3 POINT_15 = Vector3(13353.6, 2716.42, 11630.5);
	const Vector3 POINT_16 = Vector3(7329.55, 4499.71, 10183.7);

	const Vector3 POINT_17 = Vector3(11622.8, 3436.15, 13480.2);
	const Vector3 POINT_18 = Vector3(8977.93, 4167, 16397.9);
	const Vector3 POINT_19 = Vector3(7483.54, 4167, 19439.7);

	const Vector3 POINT_20 = Vector3(9574.51, 4162.43, 21325.4);
	const Vector3 POINT_21 = Vector3(11715, 4625.32, 22549.8);
	const Vector3 POINT_22 = Vector3(8906.33, 4625.32, 23747.8);

	vector<Vector3> cameraPoints = { START_POS, POINT_2, POINT_3, POINT_4, 
												POINT_5, POINT_6, POINT_7,
												POINT_8, POINT_9, POINT_10,
												POINT_11, POINT_12, POINT_13,
												POINT_14, POINT_15, POINT_16,
												POINT_17, POINT_18, POINT_19, 
												POINT_20, POINT_21, POINT_22 };

	int cameraPointIndex;
	bool autoMove;
	float time;

	const Vector3 SECOND_POS = Vector3(28780.20f, 667.29f, 17322.0f);
	const Vector3 THIRD_POS = Vector3(6519.93f, 3589.71f, 33067.8f);

	OBJMesh* cylinder;
	OBJMesh* cube;
	OBJMesh* lightObj;
	OBJMesh* dragon;

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
};

