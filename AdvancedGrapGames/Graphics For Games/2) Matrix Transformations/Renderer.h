#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"


class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void SwitchToPerspective();
	void SwitchToOrthographic();

	inline void SetScale(float s) { scale = s; }
	inline void SetRotation(float r) { rotation = r; }
	inline void SetPosition(Vector3 p) { position = p; }
	inline void setFov(float f) { fov = f; }
protected:
	Mesh* triangle;
	Camera* camera = new Camera();

	float scale;
	float rotation;
	Vector3 position;

	float fov;
};

