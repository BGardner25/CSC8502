#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	cameraTwo = new Camera();
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	heightMap = new HeightMapPNG(TEXTUREDIR"heightmap512.jpg");
	quad = Mesh::GenerateQuad();
	skyQuad = Mesh::GenerateQuad();
	splitQuad = Mesh::GenerateQuad();

	camera->SetPosition(POINT_2);
	cameraTwo->SetPosition(POINT_2);

	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 5000.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) * 30);
	pointLight = new Light(Vector3(18604.0f, 8000.0f, 26643.50f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), 200000.0f);

	basicShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectLightFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"HeightMapVertex.glsl", SHADERDIR"HeightMapFragment.glsl");
	cylinderShader = new Shader(SHADERDIR"CylinderVert.glsl", SHADERDIR"CylinderFrag.glsl");
	cylinderTwoShader = new Shader(SHADERDIR"CylinderTwoVert.glsl", SHADERDIR"CylinderTwoFrag.glsl");
	sceneShader = new Shader(SHADERDIR"bumpVertex.glsl", SHADERDIR"bufferFragment.glsl");
	combineShader = new Shader(SHADERDIR"combineVert.glsl", SHADERDIR"combineFrag.glsl");
	pointLightShader = new Shader(SHADERDIR"pointLightVert.glsl", SHADERDIR"PointLightFrag.glsl");
	waterShader = new Shader(SHADERDIR"TessVert.glsl", SHADERDIR"reflectLightFragment.glsl", "", SHADERDIR"WaterTCS.glsl", SHADERDIR"WaterTES.glsl");
	splitShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	if (!basicShader->LinkProgram() || !skyboxShader->LinkProgram() || !lightShader->LinkProgram() 
			|| !reflectShader->LinkProgram() || !cylinderShader->LinkProgram() || !cylinderTwoShader->LinkProgram() 
			|| !sceneShader->LinkProgram() || !combineShader->LinkProgram() || !pointLightShader->LinkProgram()
			|| !waterShader->LinkProgram() || !splitShader->LinkProgram())
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

	cube = new OBJMesh();
	if (!cube->LoadOBJMesh(MESHDIR"centeredcube.obj"))
		return;
	cube->SetPrimitiveType(GL_PATCHES);

	lightObj = new OBJMesh();
	if (!lightObj->LoadOBJMesh(MESHDIR"ico.obj"))
		return;

	dragon = new OBJMesh();
	if (!dragon->LoadOBJMesh(MESHDIR"dragon.obj"));

	cylinder->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"BlueStoneTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	cube->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"BlueStoneTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	dragon->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Texture2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!cubeMap || !quad->GetTexture() || !heightMap->GetTexture() || !heightMap->GetBumpMap()  || !heightMap->GetBumpMapThree()
			|| !heightMap->GetTextureTwo()	|| !heightMap->GetTextureThree() || !cylinder->GetTexture() || !cube->GetTexture() || !dragon->GetTexture())
		return;

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	SetTextureRepeating(heightMap->GetTextureTwo(), true);
	//SetTextureRepeating(heightMap->GetBumpMapTwo(), true);
	SetTextureRepeating(heightMap->GetTextureThree(), true);
	SetTextureRepeating(heightMap->GetBumpMapThree(), true);
	SetTextureRepeating(cylinder->GetTexture(), true);
	//SetTextureRepeating(cube->GetTexture(), true);

	rootNode = new SceneNode();

	SceneNode* cylinderNode = new SceneNode();
	cylinderNode->SetShader(cylinderTwoShader);
	cylinderNode->SetBoundingRadius(10000.0f);
	cylinderNode->SetTransform(Matrix4::Translation(Vector3(18604.0f, 500.0f, 26643.50f)));
	cylinderNode->SetModelScale(Vector3(1000.0f, 1500.0f, 1000.0f));
	cylinderNode->SetMesh(cylinder);
	rootNode->AddChild(cylinderNode);

	SceneNode* cylinderTwo = new SceneNode();
	cylinderTwo->SetShader(cylinderShader);
	cylinderTwo->SetBoundingRadius(10000.0f);
	cylinderTwo->SetTransform(Matrix4::Translation(Vector3(18604.0f, 3500.0f, 26643.50f)));
	cylinderTwo->SetModelScale(Vector3(800.0f, 1500.0f, 800.0f));
	cylinderTwo->SetMesh(cylinder);
	rootNode->AddChild(cylinderTwo);

	rootNode->AddChild(new Dragon(dragon));
	rootNode->AddChild(new Cube(cube));

	heightVal = 0.0f;
	waterRotate = 0.0f;
	w = &parent;
	fps = 0.0f;
	lastTime = 0.0f;
	currentTime = 0.0f;
	timePassed = 0.0f;
	frames = 0.0f;
	autoMove = true;
	time = 0.0f;
	rotation = 0.0f;
	cameraPointIndex = 0;
	splitScreen = false;

	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 60.0f);

	//SetupPointLights();
	SetupSplitScreen();

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
	delete cameraTwo;

	delete heightMap;
	delete quad;
	delete cylinder;
	delete cube;
	delete lightObj;
	delete dragon;
	delete skyQuad;
	delete splitQuad;

	delete basicShader;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete cylinderShader;
	delete sceneShader;
	delete combineShader;
	delete pointLightShader;
	delete waterShader;
	delete splitShader;

	delete light;
	delete pointLight;
	currentShader = 0;
	
	glDeleteTextures(2, splitColourTex);
	glDeleteTextures(2, splitDepthTex);
	glDeleteFramebuffers(2, splitFBO);
}

void Renderer::UpdateScene(float msec) {
	if (!splitScreen) {
		if (autoMove) {
			CameraPath(msec);
			camera->UpdateCamera(msec, true);
		}
		else {
			camera->UpdateCamera(msec);
		}
	}
	else {
		rootNode->Update(msec);
		camera->UpdateCamera(msec);
		cameraTwo->UpdateCamera(msec);
	}
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	rootNode->Update(msec);
	waterRotate += msec / 1000.0f;
	rotation = msec * 0.01f;
	heightVal += msec / 20000.0f;
	time += msec;
	//cout << "\n\n\n\n" << camera->GetPosition() << "\n\n\n\n";

	lastTime = w->GetTimer()->GetMS();
	frames++;
	timePassed += (lastTime - currentTime);
	if (timePassed >= 1000.0f) {
		fps = frames;
		frames = 0.0f;
		timePassed = 0.0f;
	}
	currentTime = w->GetTimer()->GetMS();
	if (w->GetKeyboard()->KeyTriggered(KEYBOARD_M)) {
		if (autoMove == false) {
			camera->SetChangePitch(0.0f),	cameraTwo->SetChangePitch(0.0f);
			camera->SetChangeYaw(0.0f),		cameraTwo->SetChangeYaw(0.0f);
			cameraPointIndex = 0;
			totalTime = 0;
			camera->SetPitch(0.0f),			cameraTwo->SetPitch(0.0f);
			camera->SetYaw(0.0f),			cameraTwo->SetYaw(0.0f);
		}
		autoMove = !autoMove;
	}
	if (w->GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
		autoMove = false;
		if (autoMove == false) {
			camera->SetChangePitch(0.0f),	cameraTwo->SetChangePitch(0.0f);
			camera->SetChangeYaw(0.0f),		cameraTwo->SetChangeYaw(0.0f);
			camera->SetPosition(START_POS), cameraTwo->SetPosition(START_POS);
			camera->SetYaw(8.0f),			cameraTwo->SetYaw(8.0f);
			camera->SetPitch(17.0f),		cameraTwo->SetPitch(17.0f);
		}
	}
	if (w->GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
		autoMove = false;
		if (autoMove == false) {
			camera->SetChangePitch(0.0f),	cameraTwo->SetChangePitch(0.0f);
			camera->SetChangeYaw(0.0f),		cameraTwo->SetChangeYaw(0.0f);
			camera->SetPosition(SECOND_POS), cameraTwo->SetPosition(SECOND_POS);
			camera->SetYaw(-12.0f),			cameraTwo->SetYaw(-12.0f);
			camera->SetPitch(30.0f),		cameraTwo->SetPitch(30.0f);
		}
	}
	if (w->GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
		autoMove = false;
		if (autoMove == false) {
			camera->SetChangePitch(0.0f),	cameraTwo->SetChangePitch(0.0f);
			camera->SetChangeYaw(0.0f),		cameraTwo->SetChangeYaw(0.0f);
			camera->SetPosition(THIRD_POS), cameraTwo->SetPosition(THIRD_POS);
			camera->SetYaw(-15.0f),			cameraTwo->SetYaw(-15.0f);
			camera->SetPitch(-10.0f),		cameraTwo->SetPitch(-10.0f);
		}
	}
	if (w->GetKeyboard()->KeyTriggered(KEYBOARD_P))
		splitScreen = !splitScreen;
}

void Renderer::RenderScene() {
	BuildNodeLists(rootNode);
	SortNodeLists();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	
	if(splitScreen) {
		viewMatrix = camera->BuildViewMatrix();
		glBindFramebuffer(GL_FRAMEBUFFER, splitFBO[0]);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		DrawScene();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		viewMatrix = cameraTwo->BuildViewMatrix();
		glBindFramebuffer(GL_FRAMEBUFFER, splitFBO[1]);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		DrawScene();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		DrawSplitScreen();
	}
	else
		DrawScene();

	//FillBuffers();
	//DrawPointLight();
	//CombineBuffers();

	DrawUI();

	glUseProgram(0);
	SwapBuffers();
	ClearNodeLists();
}

void Renderer::DrawScene() {
	DrawSkybox();
	DrawHeightMap();
	DrawWater();
	DrawNodes();
}

void Renderer::CameraPath(float msec) {
	if (time > 20000 && cameraPointIndex + 3 < cameraPoints.size()) {
		totalTime += msec / 2000.0f;
		Vector3 pos = camera->GetCatRomPos(totalTime, cameraPoints[cameraPointIndex], cameraPoints[cameraPointIndex + 1], 
														cameraPoints[cameraPointIndex + 2], cameraPoints[cameraPointIndex + 3]);
		if (cameraPointIndex < 2)
			camera->SetChangePitch(-0.0060f);
		else if (cameraPointIndex < 4)
			camera->SetChangePitch(0.0060f);
		else
			camera->SetChangePitch(0.0f);
		if (cameraPointIndex < 9 )
			camera->SetChangeYaw(0.012f);
		else if (cameraPointIndex < 12)
			camera->SetChangeYaw(0.032f);
		else if (cameraPointIndex > 13 && cameraPointIndex < 17)
			camera->SetChangeYaw(0.0020f);
		else if (cameraPointIndex > 17)
			camera->SetChangeYaw(0.0f);
		camera->SetPosition(pos);
		cameraTwo->SetPosition(pos);
		if (totalTime > 1.0) {
			cameraPointIndex++;
			totalTime = 0.0;
		}
	}
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
		// all nodes are either 3 or 4 verts
		if (n->GetPrimitiveType() == GL_PATCHES) {
			if (n->GetNumVertices() == 3)
				glPatchParameteri(GL_PATCH_VERTICES, 3);
			else if(n->GetNumVertices() == 4)
				glPatchParameteri(GL_PATCH_VERTICES, 4);
		}
		SetCurrentShader(n->GetShader());
		SetShaderLight(*light);
		
		glUseProgram(currentShader->GetProgram());
		
		textureMatrix = n->GetTextureMatrix();
		UpdateShaderMatrices();
		// setup all needed uniforms
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 1);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "normTex"), 2);
		glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "heightVal"), heightVal);
		glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "rotation"), rotation);
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
	SetCurrentShader(basicShader);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	DrawText("FPS: " + to_string(fps), Vector3(0.0f, 0.0f, 0.0f), 16.0f);
	DrawText("M: TOGGLE FREE LOOK/AUTOCAM", Vector3(0.0f, 18.0f, 0.0f), 14.0f);
	DrawText("1: VIEW PILLARS", Vector3(0.0f, 34.0f, 0.0f), 14.0f);
	DrawText("2: VIEW DRAGON", Vector3(0.0f, 50.0f, 0.0f), 14.0f);
	DrawText("3: VIEW CUBE TURNING INTO SPHERE", Vector3(0.0f, 66.0f, 0.0f), 14.0f);
	DrawText("P: TOGGLE SPLITSCREEN", Vector3(0.0f, 82.0f, 0.0f), 14.0f);

	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);
	glUseProgram(0);
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);
	glCullFace(GL_FRONT);

	UpdateShaderMatrices();
	skyQuad->Draw();
	
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
	quad->SetPrimitiveType(GL_PATCHES);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	SetCurrentShader(waterShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 6);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "waterRotate"), waterRotate);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "time"), time);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X * 0.50f);
	float heightY = (RAW_WIDTH - 1) * HEIGHTMAP_Y * 0.22;
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z * 0.50f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate * 0.5, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

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

void Renderer::SetupSplitScreen() {
	for (int i = 0; i < splitNum; ++i) {
		glGenFramebuffers(1, &splitFBO[i]);

		GenerateScreenTexture(splitDepthTex[i], true);
		GenerateScreenTexture(splitColourTex[i]);

		glBindFramebuffer(GL_FRAMEBUFFER, splitFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, splitColourTex[i], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, splitDepthTex[i], 0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void Renderer::DrawSplitScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	modelMatrix.ToIdentity();
	SetCurrentShader(splitShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.5f, -0.5f, -0.1)) * Matrix4::Scale(Vector3(1.5f, 0.5f, 0.0));
	UpdateShaderMatrices();
	splitQuad->SetTexture(splitColourTex[0]);
	splitQuad->Draw();

	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.5f, 0.5f, -0.1)) * Matrix4::Scale(Vector3(1.5f, 0.5f, 0.0));
	UpdateShaderMatrices();
	splitQuad->SetTexture(splitColourTex[1]);
	splitQuad->Draw();
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

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 6);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "normTex"), 7);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glActiveTexture(GL_TEXTURE7);
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

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 6);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "emissiveTex"), 7);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "specularTex"), 8);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, lightEmissiveTex);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

	quad->Draw();

	glUseProgram(0);
}