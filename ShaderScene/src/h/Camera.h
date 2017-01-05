#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

// Camera.h
// Keeps track of and updates matrices and relevant data
// representing the perspective/transform of the viewer.

class Camera {
public:
	Camera(float fovY, float aspectRatio, float nearPlane, float farPlane);
	virtual ~Camera();

	void update();
	
	void setSpeed(float speed) { m_speed = speed; }
	void setPerspective(float fovY, float aspectRatio, float nearPlane, float farPlane);

    void setTransform(glm::mat4 newTransform) { m_transform = newTransform; }

    glm::vec4* getPosition() { return &m_transform[3]; }

	const glm::mat4* getProjectionView() { return &m_projectionView; }
    const glm::mat4* getProjection() { return &m_projection; }

private:

	float m_speed;

	glm::vec3 m_up;
	glm::mat4 m_transform;
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_projectionView;
};