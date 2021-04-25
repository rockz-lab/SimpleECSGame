#include "Camera.h"

#include "Utils/math.h"

#include "glm/gtc/matrix_transform.hpp "

// See here:
// https://lmb.informatik.uni-freiburg.de/people/reisert/opengl/doc/glOrtho.html

Camera2D::Camera2D(float width, float aspect)
{
	m_ortho = glm::mat3();

	float right = width;
	float left = 0;
	float height = width / aspect;
	float top = height;
	float bottom = 0;

	m_ortho[0][0] = 2 / (right - left);
	m_ortho[1][1] = 2 / (top - bottom);

	m_ortho[2][0] = -(right + left) / (right - left);
	m_ortho[2][1] = -(top + bottom) / (top - bottom);

	m_ortho[2][2] = 1;

	glm::mat4 test = glm::ortho(0.0f, width, 0.0f, height);

	std::cout << test.length() << std::endl;
	transform = m_ortho;
	m_pos = { 0, 0 };
}

void Camera2D::SetPos(const glm::vec2& pos)
{
	transform = m_ortho;
	m_ortho[2][0] += pos.x;
	m_ortho[2][1] += pos.y;

	m_pos = pos;
}

void Camera2D::SetRot(float angle_rad)
{
	auto rotation = makeRotation(angle_rad);
	// not sure about order
	transform = rotation * m_ortho;
	transform[2][0] += m_pos.x;
	transform[2][1] += m_pos.y;
}

void Camera2D::Move(const glm::vec2& offset)
{
	m_pos += offset;
	transform[2][0] += offset.x;
	transform[2][1] += offset.y;
}

void Camera2D::Rotate(float angle_rad)
{
	auto rotation = makeRotation(angle_rad);
	transform *= rotation;
}

void Camera2D::Zoom(float scale)
{
	glm::mat3 uniformScale = glm::mat3();
	uniformScale[2][0] = scale;
	uniformScale[2][1] = scale;
	transform = transform * scale;
}

//glm::mat3 Camera2D::MakeRotation(float angle_rad)
//{
//	glm::mat3 rotation = glm::mat3();
//
//	rotation[0][0] = cos(angle_rad);
//	rotation[0][1] = -sin(angle_rad);
//	rotation[1][0] = sin(angle_rad);
//	rotation[1][1] = cos(angle_rad);
//	return rotation;
//}
