#pragma once

#include "StdAfx.h"

class FrameBuffer {
private:
	GLuint fbo, tbo, dbo;
protected:
	int width, height;
public:
	FrameBuffer(int Width, int Height);
	~FrameBuffer();

	void startNewRendering();
	void stopRendering();

	GLuint getTbo() { return tbo; }
};