#include "SceneNode.h"

SceneNode::SceneNode(Mesh* mesh, Vector4 colour) {
	this->mesh = mesh;
	this->colour = colour;
	texture = 0;
	shader = 0;
	type = GL_TRIANGLES;
	numVertices = 0;
	parent = NULL;
	isTransparent = false;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	textureMatrix.ToIdentity();
}

SceneNode::~SceneNode(void) {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->parent = this;
}

void SceneNode::Draw(const OGLRenderer& r) {
	if (mesh)
		mesh->Draw();
}

void SceneNode::Update(float msec) {
	if (parent)
		worldTransform = parent->worldTransform * transform;
	else
		worldTransform = transform;
	
	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i)
		(*i)->Update(msec);
}