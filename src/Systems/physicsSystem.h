#pragma once

#include "ecs/ecs.h"

#include "Components/gameComponents.h"

ECSManager manager;

class PhysicsSystem : public System
{
public:
	template<typename T>
	void Update(T dT)
	{
		for (auto const& entity : m_entities)
		{
			auto& transform = manager.GetComponent<Transform2D>(entity);
			auto& grav = manager.GetComponent<Gravity>(entity);
			auto& mov = manager.GetComponent<MovementState>(entity);
			
			//mov.accel = grav.g;
			mov.vel += grav.g * static_cast<float>(dT) * glm::vec2(0.0f, 1.0f);
			transform.pos += mov.vel * static_cast<float>(dT);


			printf("Position: (%f, %f)\t", transform.pos.x, transform.pos.y);

			printf("Geschwindigkeit: %f\t dT: %f\n", mov.vel.x, static_cast<float>(dT));
		}
	}

};


//class renderSystem : public System
//{
//	void Update(float dt)
//	{
//		for (auto const& entity : m_entities)
//		{
//			auto& transform = manager.GetComponent<Transform2D>(entity);
//			auto& grav = manager.GetComponent<Gravity>(entity);
//
//			double vel = grav.g * dt;
//			transform.pos += vel * dt;
//
//		}
//	}
//};


