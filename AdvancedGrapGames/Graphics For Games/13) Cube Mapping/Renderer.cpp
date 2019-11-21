/*#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	quad = Mesh::GenerateQuad();

	camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500.0f, RAW_WIDTH * HEIGHTMAP_X));

	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
					Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) / 2.0);

	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");

	if (!reflectShader->LinkProgram() || !skyboxShader->LinkProgram() || !lightShader->LinkProgram())
		return;

	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg",	TEXTUREDIR"rusted_east.jpg",
									TEXTUREDIR"rusted_up.jpg",		TEXTUREDIR"rusted_down.jpg",
									TEXTUREDIR"rusted_south.jpg",	TEXTUREDIR"rusted_north.jpg", 
									SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!cubeMap || !quad->GetTexture() || !heightMap->GetTexture() || !heightMap->GetBumpMap())
		return;

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

Renderer::~Renderer(void) {
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	currentShader = 0;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 1000.0f;
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawHeightMap();
	DrawWater();

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

void Renderer::DrawHeightMap() {
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->Draw();

	glUseProgram(0);
}

void Renderer::DrawWater() {
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	float heightY = 256 * HEIGHTMAP_Y / 3.0f;
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
					Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
					Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
					Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}*/

#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	heightMap = new HeightMapPNG(TEXTUREDIR"heightmap512.jpg");
	quad = Mesh::GenerateQuad();

	camera->SetPosition(START_POS);

	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 5000.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) * 30);
	pointLight = new Light(Vector3(18604.0f, 8000.0f, 26643.50f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), 20.0f);

	fontShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"HeightMapVertex.glsl", SHADERDIR"HeightMapFragment.glsl");
	cylinderShader = new Shader(SHADERDIR"CylinderVert.glsl", SHADERDIR"CylinderFrag.glsl");
	sceneShader = new Shader(SHADERDIR"bumpVertex.glsl", SHADERDIR"bufferFragment.glsl");
	combineShader = new Shader(SHADERDIR"combineVert.glsl", SHADERDIR"combineFrag.glsl");
	pointLightShader = new Shader(SHADERDIR"pointLightVert.glsl", SHADERDIR"PointLightFrag.glsl");

	if (!fontShader->LinkProgram() || !skyboxShader->LinkProgram() || !lightShader->LinkProgram() 
			|| !reflectShader->LinkProgram() || !cylinderShader->LinkProgram() || !sceneShader->LinkProgram() 
				|| !combineShader->LinkProgram() || !pointLightShader->LinkProgram())
		return;

	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Rock_Color.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetTextureTwo(SOIL_load_OGL_texture(TEXTUREDIR"WildGrass.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//heightMap->SetBumpMapTwo(SOIL_load_OGL_texture(TEXTUREDIR"Ground_Grass_NORM.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetTextureThree(SOIL_load_OGL_texture(TEXTUREDIR"SnowLightCoverB_S.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMapThree(SOIL_load_OGL_texture(TEXTUREDIR"SnowLightCoverB_N.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"spires_rt.tga", TEXTUREDIR"spires_lf.tga",
									TEXTUREDIR"spires_up.tga", TEXTUREDIR"spires_dn.tga",
									TEXTUREDIR"spires_bk.tga", TEXTUREDIR"spires_ft.tga",
									SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	cylinder = new OBJMesh();
	if (!cylinder->LoadOBJMesh(MESHDIR"cylinder.obj"))
		return;

	lightObj = new OBJMesh();
	if (!lightObj->LoadOBJMesh(MESHDIR"ico.obj"))
		return;

	cylinder->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"BlueStoneTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!cubeMap || !quad->GetTexture() || !heightMap->GetTexture() || !heightMap->GetBumpMap()  || !heightMap->GetBumpMapThree()
			|| !heightMap->GetTextureTwo()	|| !heightMap->GetTextureThree() || !cylinder->GetTexture())
		return;

	

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	SetTextureRepeating(heightMap->GetTextureTwo(), true);
	//SetTextureRepeating(heightMap->GetBumpMapTwo(), true);
	SetTextureRepeating(heightMap->GetTextureThree(), true);
	SetTextureRepeating(heightMap->GetBumpMapThree(), true);
	SetTextureRepeating(cylinder->GetTexture(), true);

	rootNode = new SceneNode();

	/*SceneNode* water = new SceneNode();
	water->SetShader(reflectShader);
	water->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	water->SetBoundingRadius(80000.0f);
	water->SetMesh(quad);*/

	SceneNode* cylinderNode = new SceneNode();
	cylinderNode->SetShader(cylinderShader);
	cylinderNode->SetBoundingRadius(10000.0f);
	cylinderNode->SetTransform(Matrix4::Translation(Vector3(18604.0f, 500.0f, 26643.50f)));
	cylinderNode->SetModelScale(Vector3(1000.0f, 1500.0f, 1000.0f));
	cylinderNode->SetMesh(cylinder);
	rootNode->AddChild(cylinderNode);

	heightVal = 0.0f;
	waterRotate = 0.0f;
	w = &parent;
	fps = 0.0f;
	lastTime = 0.0f;
	currentTime = 0.0f;
	timePassed = 0.0f;
	frames = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 30000.0f, (float)width / (float)height, 45.0f);

	SetupPointLights();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

Renderer::~Renderer(void) {
	delete rootNode;
	delete camera;

	delete heightMap;
	delete quad;
	delete cylinder;
	delete lightObj;

	delete fontShader;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete cylinderShader;
	delete sceneShader;
	delete combineShader;
	delete pointLightShader;

	delete light;
	delete pointLight;
	currentShader = 0;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	rootNode->Update(msec);
	waterRotate += msec / 1000.0f;
	rotation = msec * 0.01f;
	heightVal += msec / 15000.0f;
	totalTime += msec;
	if (totalTime >= 1000.0f)
		cout << camera->GetPosition(), totalTime = 0;

	lastTime = w->GetTimer()->GetMS();
	frames++;
	timePassed += (lastTime - currentTime);
	if (timePassed >= 1000.0f) {
		fps = frames;
		frames = 0.0f;
		timePassed = 0.0f;
	}
	currentTime = w->GetTimer()->GetMS();
}

void Renderer::RenderScene() {
	BuildNodeLists(rootNode);
	SortNodeLists();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawHeightMap();
	DrawWater();

	DrawNodes();

	//FillBuffers();
	//DrawPointLight();
	//CombineBuffers();

	DrawUI();
	glUseProgram(0);
	SwapBuffers();
	ClearNodeLists();
}

void Renderer::SetupPointLights() {
	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &pointLightFBO);

	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	GenerateScreenTexture(bufferDepthTex, true);
	GenerateScreenTexture(bufferColourTex);
	GenerateScreenTexture(bufferNormalTex);
	GenerateScreenTexture(lightEmissiveTex);
	GenerateScreenTexture(lightSpecularTex);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightEmissiveTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GenerateScreenTexture(GLuint& into, bool depth) {
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA8,
		width, height, 0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::FillBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();

	heightMap->Draw();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawPointLight() {
	SetCurrentShader(pointLightShader);

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "normTex"), 4);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bufferNormalTex);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

	Vector3 translate = Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f));

	Matrix4 pushMatrix = Matrix4::Translation(translate);
	Matrix4 popMatrix = Matrix4::Translation(-translate);

	float radius = pointLight->GetRadius();

	modelMatrix = pushMatrix *
		Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
		popMatrix *
		Matrix4::Translation(pointLight->GetPosition()) *
		Matrix4::Scale(Vector3(radius, radius, radius));

	pointLight->SetPosition(modelMatrix.GetPositionVector());

	SetShaderLight(*pointLight);

	UpdateShaderMatrices();

	float dist = (pointLight->GetPosition() - camera->GetPosition()).Length();
	if (dist < radius)
		glCullFace(GL_FRONT);
	else
		glCullFace(GL_BACK);

	lightObj->Draw();

	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void Renderer::CombineBuffers() {
	SetCurrentShader(combineShader);

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "emissiveTex"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "specularTex"), 4);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, lightEmissiveTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

	quad->Draw();

	glUseProgram(0);
}

void Renderer::BuildNodeLists(SceneNode* from) {
	if (frameFrustum.InsideFrustum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f)
			transparentNodeList.push_back(from);
		else
			nodeList.push_back(from);
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i)
		BuildNodeLists((*i));
}

void Renderer::SortNodeLists() {
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes() {
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i)
		DrawNode((*i));
	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i)
		DrawNode((*i));
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		SetCurrentShader(n->GetShader());
		// different light for shadow mapping i.e. point light?
		SetShaderLight(*light);
		
		glUseProgram(currentShader->GetProgram());
		//UpdateShaderMatrices();
		textureMatrix = n->GetTextureMatrix();
		UpdateShaderMatrices();
		// setup all needed uniforms
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 1);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "normTex"), 2);
		glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "heightVal"), heightVal);
		glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false,
													(float*)&(n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale())));
		
		n->Draw(*this);
	}
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::DrawUI() {
	SetCurrentShader(fontShader);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	DrawText("FPS: " + to_string(fps), Vector3(0.0f, 0.0f, 0.0f), 16.0f);

	projMatrix = Matrix4::Perspective(1.0f, 30000.0f, (float)width / (float)height, 45.0f);
	glUseProgram(0);
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);
	glCullFace(GL_FRONT);

	UpdateShaderMatrices();
	quad->Draw();
	
	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
}

void Renderer::DrawHeightMap() {
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "texGrass"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTextureTwo"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "texSnow"), 4);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTextureThree"), 5);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "heightVal"), heightVal);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->Draw();

	glUseProgram(0);
}

void Renderer::DrawWater() {
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X * 0.50f);
	float heightY = (RAW_WIDTH - 1) * HEIGHTMAP_Y * 0.22;
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z * 0.50f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}

void Renderer::DrawCylinder() {
	SetCurrentShader(cylinderShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "heightVal"), heightVal);

	modelMatrix = Matrix4::Translation(Vector3(18604.0f, 500.0f, 26643.50f)) *
					Matrix4::Scale(Vector3(1000.0f, 1500.0f, 1000.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	cylinder->Draw();

	glUseProgram(0);
}

void Renderer::DrawText(const std::string& text, const Vector3& position, const float size, const bool perspective) {
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);

	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}