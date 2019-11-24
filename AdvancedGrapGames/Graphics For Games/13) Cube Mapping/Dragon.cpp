#include "Dragon.h"

Dragon::Dragon(OBJMesh* mesh) {
	//dragonShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	dragonShader = new Shader(SHADERDIR"shadowSceneVert.glsl", SHADERDIR"shadowSceneFrag.glsl");

	if (!dragonShader->LinkProgram())
		return;

	dragonNode = new SceneNode();
	dragonNode->SetShader(dragonShader);
	dragonNode->SetBoundingRadius(100000.0f);
	dragonNode->SetTransform(Matrix4::Translation(Vector3(30000.0f, 8000.0f, 0.0f)) * Matrix4::Rotation(-30.0f, Vector3(0, 1, 0)) * 
								Matrix4::Rotation(45.0f, Vector3(1, 0, 0)));
	dragonNode->SetModelScale(Vector3(400.0f, 400.0f, 400.0f));
	dragonNode->SetMesh(mesh);
	AddChild(dragonNode);
}

Dragon::~Dragon(void) {
	delete dragonShader;
}

void Dragon::Update(float msec) {
	dragonNode->SetTransform(dragonNode->GetTransform());

	SceneNode::Update(msec);
}
