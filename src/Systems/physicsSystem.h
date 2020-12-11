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

		for (auto const& source_entity : m_entities)
			for (auto const& target_entity : m_entities)
			{
				
				if (target_entity != source_entity)
				{
					CollideCirles(source_entity, target_entity);
				}
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

		for (auto const& source_entity : m_entities)
			for (auto const& target_entity : m_entities)
			{
				if (target_entity != source_entity)
				{

					bool isSourceALine = manager.CheckEntityType<Line>(source_entity);
					bool isTargetALine = manager.CheckEntityType<Line>(target_entity);
					if (isSourceALine && !isTargetALine)
					{
						// do Circle Line check
						if (manager.CheckEntityType<Circle>(target_entity))
						{
							auto& circ = manager.GetComponent<Circle>(target_entity);
							auto& circle_pos = manager.GetComponent<Transform2D>(target_entity).pos;
							auto& line = manager.GetComponent<Line>(source_entity);
							auto& circle_mov = manager.GetComponent<MovementState>(target_entity);

							float penetration = 0;
							if (sec::circle_line(circ.radius, circle_pos, line.p1, line.p2, &penetration))
							{
								// resolve collision
								glm::vec2 linePoint = sec::projectPointOntoLine(circle_pos, line.p1, line.p2);

								glm::vec2 dir = line.p2 - line.p1;
								glm::vec2 normal = glm::normalize( glm::vec2(dir.y, -dir.x) );

								glm::vec2 projAlongNormal = glm::dot(circle_mov.vel, normal) * normal;	// pointer towards the line

								circle_mov.vel = -2.0f * projAlongNormal + circle_mov.vel;

								// move circle away from the line proportional to the amount of penetration
								circle_pos += penetration * glm::normalize(circle_pos - linePoint);

							}

						}
					}
				}
			}
	}
private:

};
