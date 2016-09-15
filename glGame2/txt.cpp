#include "txt.h"

void TxtShaderProg::LoadUniformID() {
	fbColorID = glGetUniformLocation(ProgrammID, "fourgroundBackground");
}
void TxtShaderProg::start(GLuint vao, GLuint vboXY, GLuint vboUV, float fbColor[8]) {
	glUniform4fv(fbColorID, 2, fbColor);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboXY);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboUV);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}
void TxtShaderProg::stop() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void TxtShaderProg::draw(GLuint vao, GLuint vboXY, GLuint vboUV, int length, GLenum mode, float fbColor[8]) {
	use(true);
	drawPrestarted(vao, vboXY, vboUV, length, mode, fbColor);
	use(false);
}

void TxtShaderProg::drawPrestarted(GLuint vao, GLuint vboXY, GLuint vboUV, int length, GLenum mode, float fbColor[8]) {
	start(vao, vboXY, vboUV, fbColor);
	glDrawArrays(mode, 0, length);
	stop();
}

void TxtShaderProg::useTexture(GLuint tbo) {
	glBindTexture(GL_TEXTURE_2D, tbo);
}

TxtShaderProg* txt::shader = nullptr;
unsigned int txt::nextFreeKey = 1;
txt::indexedMap txt::areas;
bool txt::wasInitialised = false;
GLuint txt::tbo = 0;

void txt::init() {
	shader = new TxtShaderProg();
	wasInitialised = true;

	char* image = nullptr;
	textureDim dim;
	if (0 != loadBMP("CourrierNew.bmp", &image, &dim)) {
		MessageBox(NULL, "ERROR: Couldn't read ascii atlas", "ERROR", MB_OK);
		return;
	}
	
	int len = dim.hight*dim.width;
	char* imageAlpha = new char[len];
	for (int i = 0; i < len; i++) {
		int index = 3 * i;
		imageAlpha[i] = (image[index + 0] == 0 && image[index + 1] == 0
			&& image[index + 2] == 0) ? 0x0 : 0xFF;
	}
	delete[] image;

	textureFlags flags;
	flags.internalFormat = GL_ALPHA4;
	flags.format = GL_ALPHA;
	flags.type = GL_UNSIGNED_BYTE;
	tbo = loadTexture(&dim, &flags, imageAlpha);
	delete[] imageAlpha;
}
void txt::close() {
	delete shader;
	wasInitialised = false;
}
void txt::draw() {
	if (!wasInitialised)
		return;

	shader->use(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader->useTexture(tbo);
	

	for (std::pair<unsigned int, txt*> text : txt::areas) {
		text.second->drawThisText();
	}

	glDisable(GL_BLEND);
	shader->use(false);
}

txt::txt() {
	key = nextFreeKey;
	areas.insert(std::pair<unsigned int, txt*>(nextFreeKey++, this));
	vao = 0;
	vboXY = 0;
	vboUV = 0;
	length = 0;
	wasWritten = false;
	setColors(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

txt::~txt() {
	areas.erase(key);
	glDeleteBuffers(1, &vboXY);
	glDeleteBuffers(1, &vboUV);
	glDeleteVertexArrays(1, &vao);
}

txt& txt::operator<<(const StreamElements &s) {
	switch (s) {
	case endl:
		sText << std::endl;
		break;
	case dec:
		sText << std::dec;
		break;
	case hex:
		sText << std::hex;
		break;
	case oct:
		sText << std::oct;
		break;
	}
	return *this;
}

void txt::write(float X, float Y, float fFontSize) { // TODO: Break Down intio multiple funktions
	wasWritten = true;
	glDeleteBuffers(1, &vboXY);
	glDeleteBuffers(1, &vboUV);
	
	if (vao == 0)
		glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	std::string s = sText.str();
	length = 4 * s.size();
	int length_2 = 2 * length;
	float* xy = new float[length_2];
	float* uv = new float[length_2];

	const float one_16 = 1 / 16.0f;
	float fVerticalFontSize = 2.0f*fFontSize;
	float xOrigen = X;
	float yOrigen = Y;

	for (int i = 0, index = 0; index < length_2; i++) {

		char ascii = s[i];

		if (ascii == NULL)
			break;

		else if (ascii == '\n') {
			yOrigen -= fVerticalFontSize;
			xOrigen = X;
			continue;
		}

		float y = 1.0f - (one_16*((ascii / 16) + 1));
		float x = (one_16*(ascii % 16));

		uv[index] = x;
		uv[index+1] = y;
		xy[index++] = xOrigen;
		xy[index++] = yOrigen;

		uv[index] = x + one_16;
		uv[index + 1] = y;
		xy[index++] = xOrigen + fFontSize;
		xy[index++] = yOrigen;

		uv[index] = x + one_16;
		uv[index + 1] = y + one_16;
		xy[index++] = xOrigen + fFontSize;
		xy[index++] = yOrigen + fVerticalFontSize;

		uv[index] = x;
		uv[index + 1] = y + one_16;
		xy[index++] = xOrigen;
		xy[index++] = yOrigen + fVerticalFontSize;

		xOrigen += fFontSize;
	}

	vboXY = loadToVBO(length_2, xy);
	vboUV = loadToVBO(length_2, uv);
	
	delete[] xy;
	delete[] uv;

}

void txt::drawThisText() {
	if (!wasWritten)
		return;

	shader->drawPrestarted(vao, vboXY, vboUV, length, GL_QUADS, fourBackGroundColor);
}

void txt::setColors(glm::vec4 fourground, glm::vec4 background) {
	fourBackGroundColor[0] = fourground.r;
	fourBackGroundColor[1] = fourground.g;
	fourBackGroundColor[2] = fourground.b;
	fourBackGroundColor[3] = fourground.a;
	fourBackGroundColor[4] = background.r;
	fourBackGroundColor[5] = background.g;
	fourBackGroundColor[6] = background.b;
	fourBackGroundColor[7] = background.a;
}

void txt::clearText() {
	sText.str("");
	wasWritten = false;
	glDeleteBuffers(1, &vboXY);
	glDeleteBuffers(1, &vboUV);
}