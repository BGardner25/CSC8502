#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "Renderer.h"

int main() {
	Window w("CSC8502 Coursework", 1920, 1080, true);
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	bool isWireFrame = false;
	
	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_INSERT)) {
			isWireFrame = !isWireFrame;
			isWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}