#include "Loader.h"

GLuint loadToVBO(int countFloats, float* data) {
	GLuint id = 0;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*countFloats, data, GL_STATIC_DRAW);
	return id;
}

GLuint loadToEBO(int countInts, unsigned int* indecies) {
	GLuint id = 0;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, countInts*sizeof(unsigned int),
		indecies, GL_STATIC_DRAW);
	return id;
}

void addTriangleToBuffer(unsigned int* trinagle, unsigned int* buffer) {
	buffer[0] = trinagle[0];
	buffer[1] = trinagle[1];

	buffer[2] = trinagle[1];
	buffer[3] = trinagle[2];

	buffer[4] = trinagle[2];
	buffer[5] = trinagle[0];
}

void triangleStripToLines(int countUIntTriangleStrip,
	unsigned int* triangleStripIndex, unsigned int* prealocketedLinesBuffer)
{

	if (countUIntTriangleStrip < 3)
		return;
	
	int stoppingPoint = countUIntTriangleStrip - 3;
	int bi = 0; //bufferIndex
	for (int i = 0; i < stoppingPoint; i++) {
		addTriangleToBuffer(&triangleStripIndex[i], &prealocketedLinesBuffer[bi]);
		bi += 6;
	}
	return;
}

void trianglesToLines(int countUIntTriangles, unsigned int* trianglesIndex,
	unsigned int* prealockatedLinesBuffer)
{
	if (countUIntTriangles < 3 || countUIntTriangles % 3 != 0)
		return;

	int bi = 0;
	for (int i = 0; i < (countUIntTriangles - 3); i += 3) {
		addTriangleToBuffer(&trianglesIndex[i], &prealockatedLinesBuffer[bi]);
		bi += 6;
	}
}

int loadBMP(char* file, char** image, textureDim* dim) {
	std::ifstream in(file, std::ios::in | std::ios::binary);
	if (in.fail()) {
		/*in.open("TextureText\\CourrierNew.bmp", std::ios::in | std::ios::binary);
		if (in.fail()) {
			MessageBox(NULL, "couldn't load parameter nor CourrierNew Font texture", "Error", MB_OK);
		}*/
		return -1;
	}
	unsigned char header[54];
	in.read((char*)header, 54);
	dim->width = *(int*)&header[18];
	dim->hight = *(int*)&header[22];
	int len = dim->width * dim->hight;
	*image = new char[3*len];

	/*BitMapHeader bfh = { 0 };
	in.read((char*)&bfh, sizeof(BitMapHeader));
	in.ignore(bfh.OffsetBits - sizeof(BitMapHeader)); */

	in.read(*image, 3 * len);

	in.close();

	return 0;
}

GLuint loadTexture(textureDim* dim, textureFlags* flags, char* data) {

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	GLuint texture = 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, flags->internalFormat, dim->width, dim->hight, 0,
		flags->format, flags->type, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

bool eq(float a, float b, float epsilon) {
	return a < b + epsilon && a > b - epsilon;
}

bool eq(glm::vec3 a, glm::vec3 b, float epsilon) {
	return eq(a.x, b.x, epsilon) && eq(a.y, b.y, epsilon) 
		&& eq(a.z, b.z, epsilon);
}