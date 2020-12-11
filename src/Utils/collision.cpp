#include "collision.h"

glm::vec2 sec::projectPointOntoLine(const glm::vec2& point, const glm::vec2& p1, const glm::vec2& p2)
{
	glm::vec2 dir = glm::normalize(p2 - p1);
	glm::vec2 normal = { dir.y, -dir.x };

	float dotprod = glm::dot(dir, (point - p1));

	return p1 + glm::normalize(dir) * dotprod;
}

