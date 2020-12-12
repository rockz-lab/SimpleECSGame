#pragma once
#include <glm/glm.hpp>

namespace sec
{
	// circle - cirlce

	template <typename T>
    bool circle_circle(const T& r1, const T& r2, const glm::vec2& center1, const glm::vec2& center2, T* penet_squared = 0)
	{
		// optional parameter penetration, passed as a ptr reference

        glm::vec2 dir = center1 - center2;
        float penetration_value = (r1 + r2) * (r1 + r2) - (dir.x*dir.x + dir.y*dir.y);

        if (penet_squared != nullptr)

            *penet_squared = penetration_value;

        if (*penet_squared >= 0)
			return true;

		return false;
	}
	// circle - line

	glm::vec2 projectPointOntoLine(const glm::vec2& point, const glm::vec2& p1, const glm::vec2& p2);



	template <typename T>
	bool circle_line(const T& radius, const glm::vec2& circleCenter, const glm::vec2& linePoint1, const glm::vec2& linePoint2, T* penetration = 0)
	{
		// calculate normal vector to Line

		glm::vec2 dir = linePoint2 - linePoint1;
		//glm::vec2 normal = { dir.y, -dir.x };

		// project the circle center onte the line
		float dotprod = glm::dot(glm::normalize(dir), (circleCenter - linePoint1));

		glm::vec2 linePoint = linePoint1 + glm::normalize(dir) * dotprod;

		// case: Projection is inside the line segment
		float penetration_value;
		if (dotprod > 0 && dotprod < glm::length(dir))
		{
			// collision if the distance from circle to line smaller than radius
			penetration_value = radius - glm::distance(circleCenter, linePoint);
			if (penetration_value >= 0)
			{
				*penetration = penetration_value;
				return true;
			}
		}
		// case: Check p1 end
		else if (dotprod <= 0)
		{
			penetration_value = radius - glm::distance(circleCenter, linePoint1);
			if (penetration_value >= 0)
			{
				*penetration = penetration_value;
				return true;
			}
		}
		// case: check p2 end
		else if (dotprod > glm::length(dir))
		{
			penetration_value = radius - glm::distance(circleCenter, linePoint2);
			if (penetration_value >= 0)
			{
				*penetration = penetration_value;
				return true;
			}
		}
		return false;
	}

// convex Polygons

// line - line intersect

// 

}
