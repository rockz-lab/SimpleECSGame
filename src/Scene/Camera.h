#pragma once

#include <glm/glm.hpp>

// Idea: Make classes that hold no state just give back the info
// State should all be in the ECS
class Camera2D
{
public:
	Camera2D(float sceneWidth, float aspect);

	void SetPos(const glm::vec2& pos);
	void SetRot(float angle_rad);

	void Move(const glm::vec2& offset);
	void Rotate(float angle_rad);

	void Zoom(float scale);
	glm::mat3 transform;

private:

	glm::mat3 m_ortho;
	glm::vec2 m_pos;
};