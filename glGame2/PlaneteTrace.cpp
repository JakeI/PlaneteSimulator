#include "PlaneteTrace.h"

PlaneteTrace::Pos::Pos() {
	set(0.0f);
}

void PlaneteTrace::Pos::set(glm::vec3 v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

void PlaneteTrace::Pos::set(float f) {
	x = f;
	y = f;
	z = f;
}

PlaneteTrace::PlaneteTrace() : UniformColoredModle() {
	usedLength = 0;
	courserPos = 0;

	traceFull = false;
	for (int i = 0; i < traceLength; i++) {
		trace[i].set(0.0f);
	}
}

PlaneteTrace::~PlaneteTrace() {

}

void PlaneteTrace::setColor(glm::vec4 col) {
	this->col = col;
}

void PlaneteTrace::draw() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	int countComponents = traceLength * 3;
	float* t = new float[countComponents];
	for (int i = 0; i < countComponents; i++) {
		t[i] = 1.0f;
	}
	int j = 0;
	if (traceFull) {
		for (int i = courserPos; i < traceLength; i++) {
			t[j++] = trace[i].x;
			t[j++] = trace[i].y;
			t[j++] = trace[i].z;
		}
	}
	for (int i = 0; i < courserPos; i++) {
		t[j++] = trace[i].x;
		t[j++] = trace[i].y;
		t[j++] = trace[i].z;
	}

	vbo = loadToVBO(j, (float*)t);
	length = j / 3 - 1;
	delete t;

	glDisable(GL_DEPTH_TEST);
	UniformColoredModle::draw(GL_LINE_STRIP);
	glEnable(GL_DEPTH_TEST);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void PlaneteTrace::add(glm::vec3 pos) {
	trace[courserPos++].set(pos);
	if (courserPos >= traceLength) {
		courserPos = 0;
		traceFull = true;
	}
}