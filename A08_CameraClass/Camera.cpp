#include "Camera.h"
#include <GL\glut.h>
#include <windows.h>

Camera::Camera() {
	cameraPerp = glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
	cameraOrtho = glm::ortho(-12.0f, 12.0f, -25.0f, 25.0f, 0.1f, 1000.0f);

	cameraTarget = vector3(0.0f, 0.0f, 0.0f);
	cameraUp = vector3(0.0f, 1.0f, 0.0f);
	cameraPosition = vector3(0.0f, 0.0f, -15.0f);

	pitch = 0.0f; //x
	yaw = 0.0f; //y
	roll = 0.0f; //z
};

matrix4 Camera::GetView(vector2 mouse) { //Should get the View Matrix from your class 
	//cameraOrient = glm::quat(vector3(pitch, yaw, roll));
	cameraView = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
	cameraOrient = glm::quat(vector3(glm::radians(mouse.x), glm::radians(mouse.y), roll));
	return glm::toMat4(cameraOrient) * glm::translate(cameraPosition);
};


matrix4 Camera::GetProjection(bool bOrtographic) { //Should get the Projection Matrix from your class, this function should let me choose between a Perspective or an Orthographic projection depending on the bool argument.
	if (bOrtographic) {
		return cameraOrtho;
	}
	else {
		return cameraPerp;
	}
};


void Camera::SetPosition(vector3 v3Position) {//Sets the position of the camera
	cameraPosition = v3Position;
};


void Camera::SetTarget(vector3 v3Target) {//Sets the target of the camera
	cameraTarget = v3Target;
};

void Camera::SetUp(vector3 v3Up) {//Sets the up vector of the camera
	cameraUp = v3Up;
};

void Camera::MoveForward(float fIncrement) {//Move the camera forward (or backwards) depending on the “view vector”
	cameraPosition.y += fIncrement;
	cameraTarget.y += fIncrement;
};

void Camera::MoveSideways(float fIncrement) {//Move the camera based on the “right” vectordepending on the “view vector”
	cameraPosition.x += fIncrement;
	cameraTarget.x += fIncrement;
};

void Camera::MoveVertical(float fIncrement) {//Move the camera up or down depending on the “up vector”
	cameraPosition.z += fIncrement;
	cameraTarget.z += fIncrement;
};

void Camera::ChangePitch(float fIncrement) {//Orient your camera (meaning the forward and up vectors) changing its angle in x
	pitch += fIncrement;
};

void Camera::ChangeRoll(float fIncrement) {//Orient your camera (meaning the forward and up vectors) changing its angle in z
	roll += fIncrement;
};
void Camera::ChangeYaw(float fIncrement) {//Orient your camera (meaning the forward and up vectors) changing its angle in y
	yaw += fIncrement;
};