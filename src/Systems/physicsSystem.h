#pragma once

#include "ecs/ecs.h"
#include "Components/gameComponents.h"

#include "Utils/collision.h"

extern ECSManager manager;

class PhysicsSystem : public System
{
public:
	template<typename T>
	void Update(const T dT)
	{
		// test collisions

		eID sourceIt = 0;
		eID targetIt = 0;
		for (auto const& e_source : m_entities)
		{
			for (auto const& e_target : m_entities)
			{
				if (e_source != e_target)
				{
					CollideCirles(e_source, e_target);
				}
				targetIt++;
			}
			sourceIt++;
		}
		for (auto const& entity : m_entities)
		{
			auto& transform = manager.GetComponent<Transform2D>(entity);
			auto& grav = manager.GetComponent<Gravity>(entity);
			auto& mov = manager.GetComponent<MovementState>(entity);

			//mov.accel = grav.g;
			mov.vel += grav.g * static_cast<float>(dT) * glm::vec2(0.0f, 1.0f);
			transform.pos += mov.vel * static_cast<float>(dT);


			//printf("Position: (%f, %f)\t", transform.pos.x, transform.pos.y);

			//printf("Geschwindigkeit: %f\t dT: %f\n", mov.vel.x, static_cast<float>(dT));
		}
	}
private:
    void CollideCirles(eID source, eID target);
};

class CollisionSystem : public System
{
public:
	void Update(const float dT)
	{
		eID sourceIt = 0;
		eID targetIt = 0;
		for (auto const& e_source : m_entities)
		{
			for (auto const& e_target : m_entities)
			{
				if (sourceIt < targetIt)
				{
					bool isSourceALine = manager.CheckEntityType<Line>(e_source);
					bool isTargetALine = manager.CheckEntityType<Line>(e_target);
					if ((isSourceALine ^ isTargetALine))
					{
						// do Circle Line check
						if (manager.CheckEntityType<Circle>(e_target))
						{
							auto& circ = manager.GetComponent<Circle>(e_target);
							auto& circle_pos = manager.GetComponent<Transform2D>(e_target).pos;
							auto& line = manager.GetComponent<Line>(e_source);
							auto& circle_mov = manager.GetComponent<MovementState>(e_target);
							collideCircleLine(circle_pos, line, circ, circle_mov);
						}
						else
						{
							auto& circ = manager.GetComponent<Circle>(e_source);
							auto& circle_pos = manager.GetComponent<Transform2D>(e_source).pos;
							auto& line = manager.GetComponent<Line>(e_target);
							auto& circle_mov = manager.GetComponent<MovementState>(e_source);
							collideCircleLine(circle_pos, line, circ, circle_mov);
						}
					}
				}
				targetIt++;
			}
			sourceIt++;
		}
	}
private:

	void collideCircleLine(glm::vec2& circle_pos, Line& line, Circle& circ, MovementState& circle_mov)
	{
		float penetration;
		if (sec::circle_line(circ.radius, circle_pos, line.p1, line.p2, &penetration))
		{
			// resolve collision
			glm::vec2 linePoint = sec::projectPointOntoLine(circle_pos, line.p1, line.p2);

			glm::vec2 dir = line.p2 - line.p1;
			glm::vec2 normal = glm::normalize(glm::vec2(dir.y, -dir.x));

			glm::vec2 projAlongNormal = glm::dot(circle_mov.vel, normal) * normal;	// pointer towards the line

			// detect if the line was crossed: if velocity vector shows in the same dir 
			if (glm::dot(circle_mov.vel, circle_pos - linePoint) > 0)
			{
				circle_pos -= (2*circ.radius - penetration + 0.01f) * glm::normalize(circle_pos - linePoint);
			}
			else
			{
				// move circle away from the line proportional to the amount of penetration
				circle_pos += (penetration + 0.01f) * glm::normalize(circle_pos - linePoint);
			}

			//circle_pos - linePoint
			circle_mov.vel = -2.0f * projAlongNormal + circle_mov.vel;


		}
	}

};
