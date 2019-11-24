#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/OBJMesh.h"

class Dragon : public SceneNode {
public:
	Dragon(OBJMesh* mesh);
	~Dragon(void);

	virtual void Update(float msec);
protected:
	Shader* dragonShader;
	SceneNode* dragonNode;
};

