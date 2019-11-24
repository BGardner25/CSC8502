#pragma once
#include"Matrix4.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Mesh.h"
#include<vector>

class SceneNode {
public:
	SceneNode(Mesh* m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	~SceneNode(void);

	void SetTransform(const Matrix4& matrix) { transform = matrix; }
	const Matrix4& GetTransform() const { return transform; }
	Matrix4 GetWorldTransform() const { return worldTransform; }

	void SetTextureMatrix(const Matrix4& tMatrix) { textureMatrix = tMatrix; }
	Matrix4 GetTextureMatrix() const { return textureMatrix; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 c) { colour = c; }

	GLuint GetTexture() const { return texture; }
	void SetTexture(GLuint tex) { texture = tex; }

	Vector3 GetModelScale() const { return modelScale; }
	void SetModelScale(Vector3 s) { modelScale = s; }

	Mesh* GetMesh() const { return mesh; }
	void SetMesh(Mesh* m) { mesh = m; }

	Shader* GetShader() const { return shader; }
	void SetShader(Shader* s) { shader = s; }

	GLuint GetPrimitiveType() const { return type; }
	void SetPrimitiveType(GLuint t) { type = t; }

	GLuint GetNumVertices() const { return numVertices; }
	void SetNumVertices(GLuint numVert) { numVertices = numVert; }

	void AddChild(SceneNode* s);

	virtual void Update(float msec);
	virtual void Draw(const OGLRenderer& r);

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd() { return children.end(); }

	float GetBoundingRadius() const { return boundingRadius; }
	void SetBoundingRadius(float f) { boundingRadius = f; }

	float GetCameraDistance() const { return distanceFromCamera; }
	void SetCameraDistance(float f) { distanceFromCamera = f; }

	bool GetIsTransparent() const { return isTransparent; }
	void SetIsTransparent(bool isTransparent) { this->isTransparent = isTransparent; }

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
	}
protected:
	SceneNode* parent;
	Mesh* mesh;
	Shader* shader;
	Matrix4 worldTransform;
	Matrix4 transform;
	Vector3 modelScale;
	Vector4 colour;
	GLuint texture;
	std::vector<SceneNode*> children;

	float distanceFromCamera;
	float boundingRadius;

	Matrix4 textureMatrix;
	Matrix4 rotation;

	GLuint type;
	GLuint numVertices;

	bool isTransparent;
};

