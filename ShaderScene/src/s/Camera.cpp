#define GLM_SWIZZLE

#include "Camera.h"
#include "AppGlobals.h"

#include <GLFW\glfw3.h>
#include <glm\ext.hpp>

Camera::Camera(float fovY, float aspectRatio, float nearPlane, float farPlane) :
	m_speed(1.f),
	m_up(0, 1.f, 0),
	m_transform(1.f),
	m_view(1.f)
{
	setPerspective(fovY, aspectRatio, nearPlane, farPlane);
}

Camera::~Camera() {

}

void Camera::setPerspective(float fovY, float aspectRatio, float nearPlane, float farPlane) {
	m_projection = glm::perspective(fovY, aspectRatio, nearPlane, farPlane);
	m_projectionView = m_projection * m_view;
}

void Camera::update() {

	GLFWwindow* window = glfwGetCurrentContext();

	// Multiply speed by 2 if holding shift
	float currentSpeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? AppGlobals::deltaTime * m_speed * 2 : AppGlobals::deltaTime * m_speed;

	// Translate - just moves position "vector" of matrix
	if (glfwGetKey(window, 'W') == GLFW_PRESS)
		m_transform[3] -= m_transform[2] * currentSpeed;
	if (glfwGetKey(window, 'S') == GLFW_PRESS)
		m_transform[3] += m_transform[2] * currentSpeed;
	if (glfwGetKey(window, 'D') == GLFW_PRESS)
		m_transform[3] += m_transform[0] * currentSpeed;
	if (glfwGetKey(window, 'A') == GLFW_PRESS)
		m_transform[3] -= m_transform[0] * currentSpeed;
	if (glfwGetKey(window, 'E') == GLFW_PRESS)
		m_transform[3] += m_transform[1] * currentSpeed;
	if (glfwGetKey(window, 'Q') == GLFW_PRESS)
		m_transform[3] -= m_transform[1] * currentSpeed;

	// Rotation
	static bool leftMouseDown = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		// Record mouse position
		static double prevX = 0;
		static double prevY = 0;

		// Only record it for the first frame where right click is held
		if (leftMouseDown == false)
		{
			leftMouseDown = true;
			glfwGetCursorPos(window, &prevX, &prevY);
		}

		// Get change in mouse position
		double mouseX = 0, mouseY = 0;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double changeX = mouseX - prevX;
		double changeY = mouseY - prevY;

		prevX = mouseX;
		prevY = mouseY;

		glm::mat4 rotMat;

		// Pitch
		if (changeY != 0)
		{
			rotMat = glm::axisAngleMatrix(m_transform[0].xyz(), (float)-changeY / 150.0f);
			m_transform[0] = rotMat * m_transform[0];
			m_transform[1] = rotMat * m_transform[1];
			m_transform[2] = rotMat * m_transform[2];
		}

		// Yaw
		if (changeX != 0)
		{
			rotMat = glm::axisAngleMatrix(m_up, (float)-changeX / 150.0f);
			m_transform[0] = rotMat * m_transform[0];
			m_transform[1] = rotMat * m_transform[1];
			m_transform[2] = rotMat * m_transform[2];
		}
	}
	else
	{
		leftMouseDown = false;
	}

	m_view = glm::inverse(m_transform);
	m_projectionView = m_projection * m_view;

}
