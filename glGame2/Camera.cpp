#include "Camera.h"

void Camera::move(glm::vec3 dP)
{
	position += dP;
}

void Camera::updateMatrix()
{
	matrix = glm::lookAt(position, direction+position, head);
}

void Camera::movePlayer(glm::vec3 dp) {
	position += dp.x*direction + dp.y*head + dp.z*cross(direction, head);
}

void Camera::setDirection(float horizontalAngle, float verticalAngle){
	/*float addH = atan2(head.z, head.x);
	if (addH != 0.0f) {
		int a = 42;
	}
	horizontalAngle += addH;*/
	//verticalAngle += atan2(head.y, sqrt(head.x*head.x + head.z*head.z)) + M_PI_2;
	direction = glm::vec3(cosf(verticalAngle)*sinf(horizontalAngle),
		sinf(verticalAngle), cosf(verticalAngle)*cosf(horizontalAngle));
}

void Camera::setDirection(glm::vec3 dir) {
	direction = glm::normalize(-dir);
}

void Camera::setRightDircection(float horizontalAngle) {

	glm::vec3 z = glm::normalize(glm::cross(up, direction));
	head = glm::normalize(glm::cross(direction, z));

	/*if (up.x != 0.0f) {
		a = glm::normalize(glm::vec3(-up.y / up.x, 1.0f, 0.0f));
	} else if(up.y != 0.0f){
		a = glm::normalize(glm::vec3(1.0f, -up.x/up.y, 0.0f));
	} else if (up.z != 0.0f) {
		a = glm::normalize(glm::vec3(1.0f, 0.0f, -up.x/up.z));
	} else { // Error this case schouldn't happen
		a = glm::vec3(1.0f, 0.0f, 0.0f);
	}*/

	/*
	Working! more or less
	glm::vec3 a = glm::normalize(glm::vec3(up.y, -up.x, 0.0f));
	glm::vec3 b = -glm::normalize(glm::cross(up, a));
	head = glm::cross(sinf(horizontalAngle - M_PI_2)*a +
		cosf(horizontalAngle - M_PI_2)*b, direction);
	*/

	/*glm::vec4 right = glm::rotate(glm::mat4(1.0f), (float)(horizontalAngle - M_PI_2), up)
		*glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	/*head = glm::cross(glm::vec3(sin(horizontalAngle - M_PI_2), 
		0, cos(horizontalAngle - M_PI_2)), direction);
	head = glm::cross(glm::vec3(right.x, right.y, right.z), direction);*/
}

void Camera::placeAt(glm::vec3 pos) {
	position = pos;
}
