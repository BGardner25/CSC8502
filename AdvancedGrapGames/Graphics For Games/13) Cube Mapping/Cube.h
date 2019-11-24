#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/OBJMesh.h"

class Cube : public SceneNode {
public:
	Cube(OBJMesh* cubeMesh);
	~Cube(void);

	virtual void Update(float msec);

protected:
	Shader* cubeShader;

	SceneNode* cubeNode;
};

