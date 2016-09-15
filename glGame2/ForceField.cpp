#include "ForceField.h"

StdCustomData ForceField::stdCustomData = { 1.0f };
glm::vec3 ForceField::defaultForceFunc(glm::vec3 x, glm::vec3* p, float* m, int l, void* costom) {
	glm::vec3 F(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < l; i++) {
		glm::vec3 r = p[i] - x;
		float R = glm::length(r);
		if (R < 0.0001)
			continue;
		F += (m[i] / (R*R*R))*r;
	}
	return F*(((StdCustomData*)costom)->graviConst);
}


ForceField::FieldPoint::FieldPoint() {
	p = glm::vec3(0.0f, 0.0f, 0.0f);
	v = glm::vec3(0.0f, 0.0f, 0.0f);
}

void ForceField::initArr(FieldPoint* arr, int len) {
	int i = 0;
	float fsx = (xMax - xMin) / xStep, fsy = (yMax - yMin) / yStep, fsz = (zMax - zMin) / zStep;
	for (int ix = 0; ix < xStep; ix++) {
		float fx = fsx*ix + xMin;
		for (int iy = 0; iy < yStep; iy++) {
			float fy = fsy*iy + yMin;
			for (int iz = 0; iz < zStep; iz++) {
				
				float fz = fsz*iz + zMin;
				glm::vec3 p(fx, fy, fz);
				glm::vec3 v = f(p, positions, masses, length, costomData);
				
				if (i >= len)
					return;
				arr[i].p = p;
				arr[i].v = v;
				i++;
			}
		}
	}
}

float ForceField::findMaxLength(FieldPoint* arr, int len) {
	float ml = 0.0f;
	for (int i = 0; i < len; i++) {
		float l = glm::length(arr[i].v);
		ml = (ml >= l) ? ml : l;
	}
	return ml;
}

void ForceField::drawArr(FieldPoint* arr, int len, float vFactor) {
	for (int i = 0; i < len; i++) {
		Arrow::draw(arr[i].p, vFactor*arr[i].v, color);
	}
}


void ForceField::draw() {
	int len = xStep*yStep*zStep;
	FieldPoint* arr = new FieldPoint[len];
	
	initArr(arr, len);
	float m = findMaxLength(arr, len);
	drawArr(arr, len, maxArrowLength / m);

	delete[] arr;
}