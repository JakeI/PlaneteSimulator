#include "Sphere.h"

void Sphere::genArrayBufferIfnExisting() {
	if (!glIsVertexArray(modle->bufferParam.vao))
		glGenVertexArrays(1, &modle->bufferParam.vao);
	glBindVertexArray(modle->bufferParam.vao);
}

void Sphere::deleteBufferIfExisting(GLuint* id) {
	if (glIsBuffer(*id))
		glDeleteBuffers(1, id);
}

LitColoredModle* Sphere::modle;
bool Sphere::wasInitialised;

lcm::BufferParam Sphere::bpBig, Sphere::bpSmall;

void Sphere::init() {
	if (wasInitialised)
		return;
	wasInitialised = true;

	modle = new LitColoredModle();

	// gen Vertecie and Normals
	genArrayBufferIfnExisting();
	deleteBufferIfExisting(&modle->bufferParam.vboPos);
	deleteBufferIfExisting(&modle->bufferParam.vboNorm);
	initPositionData();
	initIndecies();
}

void Sphere::close() {
	
	if (!wasInitialised)
		return;
	wasInitialised = false;

	if (glIsVertexArray(modle->bufferParam.vao))
		glDeleteVertexArrays(1, &modle->bufferParam.vao);
	deleteBufferIfExisting(&modle->bufferParam.vboPos);
	deleteBufferIfExisting(&modle->bufferParam.vboNorm);
	deleteBufferIfExisting(&modle->bufferParam.ebo);

	delete modle;

}

void Sphere::initIndecies(lcm::BufferParam* bp, int skip) {

	int nEnd = countVertecies;
	int countTriangleIndecies = 2 * (nEnd+1) / skip;
	unsigned int* elms = new unsigned int[countTriangleIndecies];
	int i = 0;
	for (int n = 0; n < nEnd; n+=skip) {
		elms[i++] = n;
		elms[i++] = (n + /*skip**/countVertPerCircle) % countVertecies;

		/*if ((int)((n+skip) / countVertPerCircle) % skip == 0) {
			n += skip;
		} else {
			n = countVertPerCircle*((int)ceil((n+skip) / countVertPerCircle)+skip-1);
		}*/
	}
	bp->ebo = loadToEBO(i - 1, elms);
	bp->length = i-1;
	bp->useEBO = true;
	bp->mode = GL_TRIANGLE_STRIP;

	delete[] elms;

}

void Sphere::initIndecies() {

	/*int countTriangleIndecies = countVertecies * 2 + 1;
	unsigned int* triangleIndecies = new unsigned int[countTriangleIndecies];

	int i = 0;
	bool direction = true;
	int highCounter = countVertPerCircle - 1, lowCounter = 0;
	for (int j = 0; j < countVertecies; j++) {
		triangleIndecies[i++] = lowCounter++;
		triangleIndecies[i++] = highCounter;
		if (highCounter < countVertecies - 1) {
			if (direction)
				highCounter = highCounter + 1;
			else
				highCounter -= 1;
		}
		else {
			highCounter = countVertPerCircle;
			direction = false;
		}
	}

	triangleIndecies[i++] = 0;

	bpBig.ebo = loadToEBO(countTriangleIndecies, triangleIndecies);*/
	/*int linesIndecies = (countTriangleIndecies - 3) * 6;
	unsigned int* linesIndexBuffer = new unsigned int[linesIndecies];

	triangleStripToLines(countTriangleIndecies, triangleIndecies, linesIndexBuffer);
	eboID = loadToEBO(linesIndecies, linesIndexBuffer);
	countIndecies = linesIndecies;
	modle->length = linesIndecies;
	
	delete[] linesIndexBuffer;*/

	initIndecies(&bpBig, 1);
	initIndecies(&bpSmall, 4);


	//delete[] triangleIndecies;
}

void Sphere::initPositionData() {
	float* positions = new float[countVertecies * 3];

	/*float zeroSin = sinTriangle(0.0f);

	glm::vec3 ex(1.0f, 0.0f, 0.0f);
	glm::vec3 er(0.0f, 1.0f, 0.0f);
	glm::vec2 eryz(er.y, er.z);
	float angle = M_PI / plainRotSteps;
	glm::mat2 rotation((float)cos(angle), (float)-sin(angle),
		(float)sin(angle), (float)cos(angle));

	float step = 2.0f / (countVertPerCircle);
	int i = 0;
	for (int countCircles = 0; countCircles < plainRotSteps; countCircles++) {
		eryz = rotation * eryz;
		er.y = eryz.x;
		er.z = eryz.y;
		er = normalize(er);
		for (float angle2XY = 0.0f; angle2XY < 2; angle2XY += step) { // TODO: make steps non-constant to spread vertecies batter
			glm::vec3 pos = normalize(cosTriangle(angle2XY)*ex + sinTriangle(angle2XY)*er);
			positions[i++] = pos.x;
			positions[i++] = pos.y;
			positions[i++] = pos.z;
		}
	}*/

	int maxJ = countVertecies / countVertPerCircle;
	float deltaTheta = (2 * M_PI) / countVertPerCircle;
	float deltaPhi = M_PI / (maxJ-1);
	int k = 0;

	for (int j = 0; j < maxJ; j++) {
		float phi = deltaPhi*j; 
		for (int i = 0; i < countVertPerCircle; i++) {
			float theta = deltaTheta*i;
			positions[k++] = sin(phi)*cos(theta);
			positions[k++] = sin(phi)*sin(theta);
			positions[k++] = cos(phi);
		}
	}

	bpBig.vboPos = loadToVBO(countVertecies * 3, positions);
	bpBig.vboNorm = bpBig.vboPos; // Pos and Norm are the same thing for a sphere

	bpSmall.vboPos = bpBig.vboPos;
	bpSmall.vboNorm = bpBig.vboNorm;
	
	delete[] positions;
}

Sphere::Sphere() {
	radius = 1.0f;
}

Sphere::~Sphere() {
	
}

void Sphere::draw(glm::vec3 pos, Sphere::SpereSize ss) {
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), pos);
	matrix = glm::scale(matrix, glm::vec3(radius, radius, radius));
	switch (ss) {
	case SMALL:
		modle->bufferParam.set(&bpSmall);
		break;
	case BIG: default:
		modle->bufferParam.set(&bpBig);
		break;
	}
	//bpSmall.length = 10*globalDebugParam;
	modle->draw(&drawParam, matrix);
}