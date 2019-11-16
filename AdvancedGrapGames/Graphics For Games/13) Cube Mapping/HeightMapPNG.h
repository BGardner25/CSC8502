#pragma once
#include "../nclgl/Mesh.h"

#define RAW_WIDTH 541
#define RAW_HEIGHT 541

#define HEIGHTMAP_X 64.0f
#define HEIGHTMAP_Z 64.0f
#define HEIGHTMAP_Y 4.0f
#define HEIGHTMAP_TEX_X 1.0f / HEIGHTMAP_X
#define HEIGHTMAP_TEX_Z 1.0f / HEIGHTMAP_Z

class HeightMapPNG : public Mesh {
public:
	HeightMapPNG(string name);
	~HeightMapPNG() {}
};

