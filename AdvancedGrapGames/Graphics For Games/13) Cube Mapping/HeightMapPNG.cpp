#include "HeightMapPNG.h"

HeightMapPNG::HeightMapPNG(string name) {
	numVertices = RAW_WIDTH * RAW_HEIGHT;
	numIndices = (RAW_WIDTH - 1) * (RAW_HEIGHT - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	int size = RAW_WIDTH;
	unsigned char* data = SOIL_load_image(name.c_str(), &size, &size, 0, SOIL_LOAD_RGB);

	for (int x = 0; x < RAW_WIDTH; ++x) {
		for (int z = 0; z < RAW_HEIGHT; ++z) {
			float fX = float(x) / (RAW_WIDTH / float(size - 1));
			float fY = float(z) / (RAW_WIDTH / float(size - 1));
			int offset = (x * RAW_WIDTH) + z;

			int iX = (int)fX;
			int iY = (int)fY;
			vertices[offset] = Vector3(	x * HEIGHTMAP_X,
												(data[3 * (iX * size + iY)]
												+ data[3 * ((iX + 1) * size + iY)]
												+ data[3 * (iX * size + (iY + 1))]
												+ data[3 * ((iX + 1) * size + (iY + 1))]) * HEIGHTMAP_Y,
										z * HEIGHTMAP_Z);
			textureCoords[offset] = Vector2(x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);
		}
	}

	delete data;

	numIndices = 0;

	for (int x = 0; x < RAW_WIDTH - 1; ++x) {
		for (int z = 0; z < RAW_HEIGHT - 1; ++z) {
			int a = (x * (RAW_WIDTH)) + z;
			int b = ((x + 1) * (RAW_WIDTH)) + z;
			int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
			int d = (x * (RAW_WIDTH)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}

	GenerateNormals();
	GenerateTangents();
	BufferData();
}