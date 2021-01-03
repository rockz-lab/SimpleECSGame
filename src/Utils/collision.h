#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <vector>
#include <iostream>

#include "ecs/ecsTypes.h"


namespace coll
{
	namespace internal
	{
		template <typename T>
		inline bool interval_intersect(T a[2], T b[2], float &overlap)
		{
			T* lower;
			T* upper;
			// find the interval with smallest maximum
			if (a[1] < b[1])
			{
				lower = a;
				upper = b;
			}
			else
			{
				lower = b;
				upper = a;
			}

			// check with minimum of the other interval
			overlap = lower[1] - upper[0];
			if (overlap > 0)
				return true;
			return false;
		}
	}

	// circle - cirlce

	template <typename T>
    inline bool circle_circle(const T& r1, const T& r2, const glm::vec2& center1, const glm::vec2& center2, T* penet_squared = 0)
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


	inline glm::vec2 projectPointOntoLine(const glm::vec2& point, const glm::vec2& p1, const glm::vec2& p2)
	{
		glm::vec2 dir = glm::normalize(p2 - p1);
		glm::vec2 normal = { dir.y, -dir.x };

		float dotprod = glm::dot(dir, (point - p1));

		return p1 + glm::normalize(dir) * dotprod;
	}



	template <typename T>
	inline bool circle_line(const T& radius, const glm::vec2& circleCenter, const glm::vec2& linePoint1, const glm::vec2& linePoint2, T* penetration = 0)
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

	// Consideration: Define the data like a triangle Fan for rendering
	//template <int numVerts>
	struct PolygonData
	{
		std::vector<std::pair<int, int>> indices;
		std::vector<glm::vec2> vertices;
		std::vector<glm::vec2> normals;
	};



	inline void transformPolygon(float rotation, const glm::vec2 &translation, PolygonData &poly)
	{

		for (auto& n : poly.normals)
			n = glm::rotate(n, rotation);

		for (auto& v : poly.vertices)
			v = glm::rotate(v, rotation) + translation;
	}

	struct collisionData
	{
		PolygonData* referenceBody;
		PolygonData* incidentBody;
		int collisionFaceIndex;
	};


	inline bool convexPolyCollision(const PolygonData &polyA, const PolygonData &polyB, collisionData& data)
	{
		// Project all the vertices of both polygons onto the axes defined by all the normals

		auto const &normalsA = polyA.normals;
		auto const &normalsB = polyB.normals;
	
		auto it_normalsA = normalsA.begin();
		auto it_normalsB = normalsB.begin();

		float minOverlap = INFINITY;
		int minIndex = -1;
		int index = 0;
		// loop over the combined points and normals of both elements:

		while (it_normalsB != normalsB.end())
		{
			// because we want to loop over the combined arrays of normals from both polygons
			glm::vec2 separating_axis = it_normalsA != normalsA.end() ? *it_normalsA : *it_normalsB;

			float minA = INFINITY;
			float maxA = -INFINITY;
			for (auto& vertex : polyA.vertices)
			{
				float projection = glm::dot(separating_axis, vertex);

				// minimum
				if (projection > maxA)
					maxA = projection;
				// maximum
				if (projection < minA)
					minA = projection;
			}

			float minB = INFINITY;
			float maxB = -INFINITY;
			for (auto& vertex : polyB.vertices)
			{
				float projection = glm::dot(separating_axis, vertex);

				// minimum
				if (projection > maxB)
					maxB = projection;
				// maximum
				if (projection < minB)
					minB = projection;
			}

			// check for interesections of the projected values in 1D:
			float projA[2] = { minA, maxA };
			float projB[2] = { minB, maxB };
			
			float overlap;
			if (!internal::interval_intersect(projA, projB, overlap))
				return false;
			
			if (overlap < minOverlap)
			{
				minOverlap = overlap;
				minIndex = index;
			}
			index++;

			// Loop stuff:
			it_normalsA != normalsA.end() ? ++it_normalsA : ++it_normalsB;
		}

		// reference Body has the face of the collision normal, incident is the other body
		if (minIndex < polyA.vertices.size())
		{
			data.referenceBody = const_cast<PolygonData*>(&polyA);
			data.incidentBody = const_cast<PolygonData*>(&polyB);
			data.collisionFaceIndex = minIndex & 3;
		}
		else
		{
			data.referenceBody = const_cast<PolygonData*>(&polyB);
			data.incidentBody = const_cast<PolygonData*>(&polyA);
			data.collisionFaceIndex = minIndex & 3;
		}

		return true;
	}

	
	inline bool pointInPoly(const glm::vec2 &point, const PolygonData &polygon)
	{
		bool isInside = true;
		// traverse all the  edges
		for (int i = 0; i < polygon.vertices.size(); i++)
		{
			auto line_indices = polygon.indices[i];
			auto first = polygon.vertices[line_indices.first];
			auto second = polygon.vertices[line_indices.second];

			glm::vec2 normal = polygon.normals[i];
			// check, if point is in positive normal directions

			float checkVal = glm::dot(normal, point - first);

			isInside = isInside && (checkVal < 0);
		}
		return isInside;
	}

// line - line intersect

// 

}
