#include "Cube.h"

Cube::Cube(OBJMesh* cubeMesh) {
	cubeShader = new Shader(SHADERDIR"TessVert.glsl", SHADERDIR"PerPixelFragment.glsl", "", SHADERDIR"CubeTCS.glsl", SHADERDIR"CubeTES.glsl");

	if (!cubeShader->LinkProgram())
		return;

	cubeNode = new SceneNode();
	cubeNode->SetShader(cubeShader);
	cubeNode->SetBoundingRadius(100000.0f);
	cubeNode->SetTransform(Matrix4::Translation(Vector3(8022.42f, 2967.89f, 28263.30f)));
	cubeNode->SetModelScale(Vector3(200.0f, 200.0f, 200.0f));
	cubeNode->SetMesh(cubeMesh);
	cubeNode->SetNumVertices(3);
	cubeNode->SetPrimitiveType(GL_PATCHES);
	AddChild(cubeNode);
}

Cube::~Cube(void) {
	delete cubeShader;
}

void Cube::Update(float msec) {
	cubeNode->SetTransform(cubeNode->GetTransform() * Matrix4::Rotation(msec * 0.05f, Vector3(1.0f, 1.0f, 1.0f)));

	SceneNode::Update(msec);
}