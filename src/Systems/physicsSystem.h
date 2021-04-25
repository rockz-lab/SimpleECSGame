#pragma once

#include "ECS/ECS.h"
#include "Components/gameComponents.h"

#include "Utils/Collision.h"
//#include "Collision/constraints.h"

extern ECSManager manager;

class PhysicsSystem : public System
{
public:
	
	void Update(const float dT)
	{
		// needed: Mechanism to select the right list of entities


		for (auto const& entity : m_entitiyLists[0])
		{
			auto& transform = manager.GetComponent<Transform2D>(entity);
			auto& grav = manager.GetComponent<Gravity>(entity);
			auto& state = manager.GetComponent<RigidBodyState>(entity);

			// apply Gravity and other constant forces/torques
			state.force += glm::vec2(0, -grav.g * state.mass);

			state.centerPos_o =		state.centerPos;
			state.angMomentum_o =	state.angMomentum;
			state.momentum_o =		state.momentum;
			state.rotation_o =		state.rotation;
			state.centerPos_o =		state.centerPos;

			//state.accel = grav.g;
			state.momentum +=		state.force * dT;
			state.angMomentum +=	state.torque * dT;
			state.centerPos +=		state.momentum / state.mass * dT;
			state.rotation +=		state.angMomentum / state.angMass * dT;

			transform.pos += state.momentum / state.mass * dT;
			transform.rotation = state.rotation;

			// reset force and torque
			state.force = {};
			state.torque = {};
		}
	}
private:
};

class CollisionSystem : public System
{
public:
	void Update(const float dT)
	{
		eID sourceIt = 0;
		eID targetIt = 0;
		auto& collisionEntities = m_entitiyLists[0];

		for (eID const& e_source : collisionEntities)
		{
			for (eID const& e_target : collisionEntities)
			{
				if (e_source != e_target)
				{
					bool isTriangle_s = manager.CheckCompType<Triangle>(e_source);
					bool isTriangle_t = manager.CheckCompType<Triangle>(e_target);

					if (isTriangle_s && isTriangle_t)
					{
						auto &polyBase_s = manager.GetComponent<Triangle>(e_source);
						
						auto poly_t = manager.GetComponent<Triangle>(e_target);
						auto poly_s = manager.GetComponent<Triangle>(e_source);

						auto& color_s = manager.GetComponent<Color>(e_source);
						auto& color_t = manager.GetComponent<Color>(e_target);

						auto& transform_s = manager.GetComponent<Transform2D>(e_source);
						auto& transform_t = manager.GetComponent<Transform2D>(e_target);

						transformPolygon(transform_s.rotation, transform_s.pos, poly_s.vertexData);
						transformPolygon(transform_t.rotation, transform_t.pos, poly_t.vertexData);

						//coll::collisionData data{};
						if (convexPolyCollision(poly_s.vertexData, poly_t.vertexData))
						{
							// detect change of collision state:
							if ((manager.GetComponent<CollisionState>(e_target).status == false) ||
								(manager.GetComponent<CollisionState>(e_source).status = false) )
							{
								color_t.r = 200;
								color_s.r = 200;
								eID line = manager.CreateEntity();
								Line markNormal;

								
								//auto manifold = makeContactManifold(*data.referenceBody, *data.incidentBody, data.collisionFaceIndex);
								/*glm::vec2 centerPt = 0.5f * (data.v1 + data.v2);

								markNormal.p1 = centerPt;
								markNormal.p2 = centerPt + data.normal *100.0f;
								manager.AddComponent<Line>(line, markNormal);*/
							}

							manager.GetComponent<CollisionState>(e_target).status = true;
							manager.GetComponent<CollisionState>(e_source).status = true;
						}
						else
						{
							color_s.r = 128;
							color_t.r = 128;
							manager.GetComponent<CollisionState>(e_target).status = false;
							manager.GetComponent<CollisionState>(e_source).status = false;
						}

					}

				}

				//if (sourceIt < targetIt)
				//{
				//	bool isSourceALine = manager.CheckEntityType<Line>(e_source);
				//	bool isTargetALine = manager.CheckEntityType<Line>(e_target);
				//	if ((isSourceALine ^ isTargetALine))
				//	{
				//		// do Circle Line check
				//		if (manager.CheckEntityType<Circle>(e_target))
				//		{
				//			auto& circ = manager.GetComponent<Circle>(e_target);
				//			auto& circle_pos = manager.GetComponent<Transform2D>(e_target).pos;
				//			auto& line = manager.GetComponent<Line>(e_source);
				//			auto& circle_mov = manager.GetComponent<RigidBodyState>(e_target);
				//			float g = manager.GetComponent<Gravity>(e_target).g;
				//			collideCircleLine(circle_pos, line, circ, circle_mov, dT, g);
				//		}
				//		else
				//		{
				//			auto& circ = manager.GetComponent<Circle>(e_source);
				//			auto& circle_pos = manager.GetComponent<Transform2D>(e_source).pos;
				//			auto& line = manager.GetComponent<Line>(e_target);
				//			auto& circle_mov = manager.GetComponent<RigidBodyState>(e_source);
				//			float g = manager.GetComponent<Gravity>(e_source).g;
				//			collideCircleLine(circle_pos, line, circ, circle_mov, dT, g);
				//		}
				//	}
				//	else if ((e_source != e_target) && manager.CheckEntityType<Circle>(e_source) && manager.CheckEntityType<Circle>(e_target))
				//	{
				//		CollideCirles(e_source, e_target, dT);
				//	}
				//}
				//targetIt++;
			}
			sourceIt++;
		}
	}
private:
	float elatsicity = 0.4f;
	void CollideCirles(eID source, eID target, float dT);

	void collideCircleLine(glm::vec2& circle_pos, Line& line, Circle& circ, RigidBodyState& circle_mov, float dT, float g);

};
