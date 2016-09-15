#include "Arrow.h"

UniformColoredModle* Arrow::modle;
bool Arrow::wasInitialised = false;
GLuint Arrow::ebo = 0;

void Arrow::init() {
	if (wasInitialised)
		return;
	wasInitialised = true;

	modle = new UniformColoredModle();

	static const float rsData[] = {
		0.0f, 0.0f,//0
		0.05f, 0.0f,//1
		0.05f, 0.0f,//1
		0.05f, 0.8f,//2
		0.05f, 0.8f,//2
		0.1f, 0.8f,//3
		0.1f, 0.8f,//3
		0.0f, 1.0f //4
	};
	/*static const float rsNormals[] = {
		0.0f, -1.0f,//0
		0.0f, -1.0f,//1
		1.0f, 0.0f, //1
		1.0f, 0.0f, //2
		0.0f, -1.0f,//2
		0.0f, -1.0f,//3
		1 / sqrt(2), 1 / sqrt(2),//3
		1 / sqrt(2), 1 / sqrt(2) //4
	};*/

	int rsVertCount = sizeof(rsData) / (2 * sizeof(float));
	int rsFloatCount = rsVertCount * 2;	

	const int refolutionIncrements = 8;

	const int totalVertexCount = refolutionIncrements * rsVertCount;
	float* vertData = new float[3 * totalVertexCount];
	const float deltaAngle = (2*M_PI) / refolutionIncrements;
	int index = 0;
	for (int i = 0; i < refolutionIncrements; i++) {
		float theta = deltaAngle*i;
		for (int j = 0; j < rsFloatCount; j += 2) {
			vertData[index++] = rsData[j + 1];
			vertData[index++] = sin(theta)*rsData[j];
			vertData[index++] = cos(theta)*rsData[j];
		}
	}

	glGenVertexArrays(1, &modle->vao);
	glBindVertexArray(modle->vao);

	index -= 1;
	modle->vbo = loadToVBO(index, vertData);

	delete[] vertData;

	const static unsigned int indData[] = { 0, 1, 9,	2, 3, 10,	3, 11, 4,	5, 4, 12,	12, 6, 14,	6, 7, 14 };
	const int indDataLength = sizeof(indData) / sizeof(unsigned int);
	unsigned int* indecies = new unsigned int[refolutionIncrements * 6 * 3]; //Depends on Data
	
	int indeciesI = 0;
	for (int i = 0; i < refolutionIncrements; i++) {
		int offset = i * rsVertCount;
		for (int j = 0; j < indDataLength; j++) {
			indecies[indeciesI++] = (indData[j] + offset) % totalVertexCount;
		}
	}
	indeciesI -= 1;
	ebo = loadToEBO(indeciesI, indecies);
	modle->length = indeciesI;
	
	delete[] indecies;
}

void Arrow::close() {
	if (!wasInitialised)
		return;
	wasInitialised = false;
	delete modle;
}

void Arrow::draw(glm::vec3 pos, glm::vec3 vector, glm::vec4 col) {
	
	glm::mat4 r(1.0f);
	float l = glm::length(vector);

	glm::vec3 dir = vector / l;

	r = glm::translate(r, pos);

	glm::vec3 native_arrow_dir = glm::vec3(1.0f, 0.0f, 0.0f);
	float phi = acos(glm::dot(native_arrow_dir, dir));
	glm::vec3 axis = glm::cross(native_arrow_dir, dir);
	if (phi > 0.0001) {
		r = glm::rotate(r, phi, axis);
	} else {
		r = r*glm::mat4(-1.0f);
	}

	r = glm::scale(r, glm::vec3(l, l, l));

	/*glm::mat4 r(1.0f);
	float l = glm::length(vector);
	glm::vec3 dir = vector / l;
	float rot_angle = acos(dir.x);
	r = glm::translate(r, pos);
	if (fabs(rot_angle) > 0.00000001f) {
		glm::vec3 rotAxis = glm::normalize(glm::cross(dir, glm::vec3(1.0f, 0.0f, 0.0f)));
		r = glm::rotate(r, rot_angle, rotAxis);
		r[0][2] *= -1;
		r[1][2] *= -1;
		r[2][2] *= -1;
	}
	r = glm::scale(r, glm::vec3(l, l, l));*/
	
	modle->col = col;
	modle->draw(GL_LINES, ebo, r);
}