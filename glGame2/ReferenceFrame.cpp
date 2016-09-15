
#include "ReferenceFrame.h"

ReferenceFrame::ReferenceFrame() {
	vilocity = glm::vec3(0.0f);
	position = glm::vec3(0.0f);
	scale = 1.0f;
}

ReferenceFrame::ReferenceFrame(glm::vec3 vilocity, glm::vec3 position) {
	this->vilocity = vilocity;
	this->position = position;
}

ReferenceFrame::~ReferenceFrame() {

}

//project vector a along vector b
glm::vec3 proj(glm::vec3 a, glm::vec3 b) {
	b = glm::normalize(b);
	float l = glm::dot(a, b);
	return l*b;
}

glm::vec3 ReferenceFrame::transform(glm::vec3 v, glm::vec3 atR) {
	if (vilocity == glm::vec3(0.0f, 0.0f, 0.0f) && position == glm::vec3(0.0f, 0.0f, 0.0f))
		return 5.0f*scale*v;
	glm::vec3 dirinv = glm::normalize(atR - position);

	glm::vec3 normal = glm::cross(v, dirinv);

	glm::vec4 b = glm::rotate(glm::mat4(1.0f), (float)M_PI_2, glm::vec3(normal))*glm::vec4(dirinv, 1.0f);
	glm::vec3 c(b.x, b.y, b.z);

	glm::vec3 out = -proj(scale*(v + vilocity), c);

	/*//Show arrow system
	Arrow::draw(position, dirinv, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	Arrow::draw(position, glm::normalize(normal), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	Arrow::draw(position, c, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	*/

	return scale*out;
}