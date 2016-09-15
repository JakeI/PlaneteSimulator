#include "FrameBufferQuad.h"

FrameBufferQuad::FrameBufferQuad() : TexturedQuad(), FrameBuffer(1280, 720) {
	scale = 0.6f;
}

FrameBufferQuad::~FrameBufferQuad() {}

void FrameBufferQuad::changeScale(SCALE_DELTA_TYPE s) {
	if (s & SCALE_DELTA_INCREASE) { 
		scale = scale > 1.86f ? 1.9f : scale + 0.04f;
	}
	if (s & SCALE_DELTA_DECREASE) { 
		scale = scale < 0.14f ? 0.1f : scale - 0.04f;
	}
}

void FrameBufferQuad::draw() {
	glm::mat4 modle = glm::translate(glm::mat4(1.0f), glm::vec3(0.95f - scale, -0.95f, 0.0f));
	modle = glm::scale(modle, glm::vec3(scale, scale, scale));
	setTbo(getTbo());
	TexturedQuad::draw(modle);
}

void FrameBufferQuad::callUpdateScreenSize(RotatableCamera* rc) {
	RotatableCamera::ViewDimention d;
	d.hight = height;
	d.width = width;
	rc->updateScreenSize(&d);
}