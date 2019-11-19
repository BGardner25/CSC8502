#pragma once
#include "OGLRenderer.h"

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, 
	NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, 
	MAX_BUFFER
};
class Mesh {
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw();
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad();

	void SetTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() { return texture; }

	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap() { return bumpTexture; }

	void SetTextureTwo(GLuint tex) { textureTwo = tex; }
	GLuint GetTextureTwo() { return textureTwo; }
	void SetBumpMapTwo(GLuint tex) { bumpTextureTwo = tex; }
	GLuint GetBumpMapTwo() { return bumpTextureTwo; }
	void SetTextureThree(GLuint tex) { textureThree = tex; }
	GLuint GetTextureThree() { return textureThree; }
	void SetBumpMapThree(GLuint tex) { bumpTextureThree = tex; }
	GLuint GetBumpMapThree() { return bumpTextureThree; }

protected:
	void GenerateNormals();
	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3& a, const Vector3& b, const Vector3& c, 
								const Vector2& ta, const Vector2& tb, const Vector2& tc);
	void BufferData();

	GLuint numIndices;
	unsigned int* indices;

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;

	Vector3* vertices;
	Vector4* colours;
	Vector3* normals;
	Vector3* tangents;

	GLuint texture;
	GLuint bumpTexture;
	Vector2* textureCoords;

	GLuint textureTwo;
	GLuint bumpTextureTwo;

	GLuint textureThree;
	GLuint bumpTextureThree;
};
