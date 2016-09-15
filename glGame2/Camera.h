#pragma once
#include "StdAfx.h"

class Camera
{
protected:
	glm::vec3 position, direction, head;
	glm::mat4 matrix;
	glm::vec3 up;
public:
	Camera() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		direction = glm::vec3(0.0f, 0.0f, 1.0f);
		head = glm::vec3(0.0f, 1.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);

		updateMatrix();
	}
	Camera(glm::vec3 Position, glm::vec3 Direction, glm::vec3 Head) {
		position = Position;
		direction = glm::normalize(Direction);
		head = glm::normalize(Head);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		updateMatrix();
	}
	~Camera(){}

	void move(glm::vec3 dP);
	virtual void updateMatrix();
	virtual glm::mat4 getMatrix() {
		return matrix;
	}
	void movePlayer(glm::vec3 dp);
	void setDirection(float horizontalAngle, float verticalAngle);
	void setDirection(glm::vec3 dir);
	void setRightDircection(float horizontalAngle);

	void placeAt(glm::vec3 pos);

	void setUpDirection(glm::vec3 Up) {
		up = Up; // That makes the sum of all error equal to zero
	}
	glm::vec3 getDirection() { return direction; }
	glm::vec3 getPosition() { return position; }
};

