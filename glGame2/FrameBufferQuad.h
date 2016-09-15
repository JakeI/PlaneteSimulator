#pragma once

#include "TexturedQuad.h"
#include "FrameBuffer.h"
#include "RotatableCamera.h"

typedef int SCALE_DELTA_TYPE;
#define SCALE_DELTA_NONE		(SCALE_DELTA_TYPE) 0
#define SCALE_DELTA_INCREASE	(SCALE_DELTA_TYPE) 2
#define SCALE_DELTA_DECREASE	(SCALE_DELTA_TYPE) 4

class FrameBufferQuad : public TexturedQuad, public FrameBuffer {
private:
	float scale;
public:
	FrameBufferQuad();
	~FrameBufferQuad();

	void changeScale(SCALE_DELTA_TYPE s);
	void draw();

	void callUpdateScreenSize(RotatableCamera* rc);
};