#include "physicsSystem.h"

#include "Utils/math.h"

#include <iostream>
#include <cassert>

//using namespace glm;

//namespace glm
//{
//	vec2& dot(vec2 a, vec2 b)
//	{
//		return glm::dot(a, b);
//	}
//}


bool timeOfImpact_circle_circle(float& t, vec2 const& p1, vec2 const& p2, vec2 const& v1, vec2 const& v2, float r1, float r2)
{
	// calculate the impact time of two circles assuming linear trajectories
	// given the previous positions and current velocities calculate the incremental timestep to get to the actual time of impact

	float p = 2.0f * dot((p1 - p2), (v1 - v2)) / dot((v1 - v2), (v1 - v2));
	float q = ( dot((p1 - p2), (p1 - p2)) - r1*r1 - r2*r2 ) / dot((v1 - v2), (v1 - v2));

	// solve quadratic equation
	float discriminant = (p / 2.0f) * (p / 2.0f) - q;

	if (discriminant < 0)
		return false;

	t = - p / 2 - glm::sqrt(discriminant);
	return true;
}

bool timeOfImpact_circle_line(float& t, vec2& p1, vec2& p2, vec2& l1, vec2& l2, float r)
{
	vec2 dir = l2 - l1;
	vec2 n = normalize(vec2( dir.y, -dir.x ));

	float t1 = (r - dot(n, p1)) / dot(n, (p2 - p1));
	float t2 = (r - dot(-n, p1)) / dot(-n, (p2 - p1));
	float t_out = min(t1, t2);
	printf("Time of impact: %f\n", t_out);
	printf("p2 -p1: %f\n", normalize(p2 - p1));
	printf("p2: %f, %f\n", p1.x, p1.y);
	printf("p2: %f, %f\n", p2.x, p2.y);
	if (t_out > 0)
	{
		return true;
		t = t_out;
	}
	return false;
}

//void CollisionSystem::collideCircleLine(glm::vec2& circle_pos, Line& line, Circle& circ, RigidBodyState& circle_mov, float dT, float g)
//{
//	float penetration;
//	if (coll::circle_line(circ.radius, circle_pos, line.p1, line.p2, &penetration))
//	{
//		// resolve collision
//		float t = 0;
//		glm::vec2 pos_old = circle_mov.pos_old;
//		float dT_remaining = 0;
//
//		while (true)
//		{
//			bool isOK = timeOfImpact_circle_line(t, circle_mov.pos_old, circle_pos, line.p1, line.p2, circ.radius);
//			float dT_impact = t * dT;
//			dT_remaining = dT - dT_impact;
//
//			circle_pos = pos_old + dT_impact * circle_mov.vel_old;
//			circle_mov.vel = circle_mov.vel_old + glm::vec2(0.0f, g) * dT_impact;
//			//if (isOK)
//				break;
//		}
//
//		glm::vec2 dir = line.p2 - line.p1;
//		glm::vec2 normal = glm::normalize(glm::vec2( dir.y, -dir.x ));
//
//		//circle_pos - linePoint
//		auto tangent = glm::vec2{ normal.y, -normal.x };
//		float normal_vel = glm::dot(circle_mov.vel, normal);
//		float tangent_vel = glm::dot(circle_mov.vel, tangent);
//		circle_mov.vel = tangent_vel * tangent - normal_vel * elatsicity * normal;
//
//		circle_pos = circle_pos + dT_remaining * circle_mov.vel;
//		circle_mov.vel = circle_mov.vel + glm::vec2(0.0f, g) * dT_remaining;
//	}
//}
//
//void CollisionSystem::CollideCirles(eID source, eID target, float dT)
//{
//	assert(source != target && "error: comparing the same entities for collisions");
//	auto& tform1 = manager.GetComponent<Transform2D>(source);
//	auto& circle1 = manager.GetComponent<Circle>(source);
//
//	auto& tform2 = manager.GetComponent<Transform2D>(target);
//	auto& circle2 = manager.GetComponent<Circle>(target);
//
//	auto const& r_s = circle1.radius;
//	auto const& r_t = circle2.radius;
//
//	glm::vec2& center_s = tform1.pos;
//	glm::vec2& center_t = tform2.pos;
//
//	// Test for collisions.
//	float penetration;
//	if (coll::circle_circle(r_s, r_t, center_s, center_t, &penetration))
//	{
//		auto& source_mov = manager.GetComponent<RigidBodyState>(source);
//		auto& target_mov = manager.GetComponent<RigidBodyState>(target);
//		// calculate the time of impact assuming a linear trajectory
//		auto& source_vel = source_mov.pos_old - center_s;
//		auto& target_vel = source_mov.pos_old - center_t;
//
//		float g = manager.GetComponent<Gravity>(source).g;
//		float dT_remaining = 0;
//		float dT_imp;
//		if (timeOfImpact_circle_circle(dT_imp, source_mov.pos_old,
//			target_mov.pos_old,
//			source_vel,
//			target_vel, r_s, r_t))
//		{
//			dT_imp *= dT;
//			assert(dT_imp > 0 && dT_imp < dT && "the collision did not happen within the last timestep");
//			
//			// advance physics integration by the delta time of impact
//			center_s = source_mov.pos_old + dT_imp * source_mov.vel_old;
//			center_t = target_mov.pos_old + dT_imp * target_mov.vel_old;
//			
//			source_mov.vel = source_mov.vel_old + glm::vec2(0.0f, g) * dT_imp;
//			target_mov.vel = target_mov.vel_old + glm::vec2(0.0f, g) * dT_imp;
//			dT_remaining = dT - dT_imp;
//		}
//
//		// calculate normal and tangent vectors at collision point
//		glm::vec2 n = glm::normalize(center_s - center_t);		// (source -> target)
//		glm::vec2 t = {n.y, -n.x};
//
//
//
//		// Dynamic response. Here only the velocities change
//
//		auto& vel_s = source_mov.vel;
//		auto& vel_t = target_mov.vel;
//
//		// Project vel onto the normal and tangent directions (using dotproduct)
//		float vel_n_s = glm::dot(n, vel_s);
//		float vel_t_s = glm::dot(t, vel_s);
//
//		float vel_n_t = glm::dot(n, vel_t);
//		float vel_t_t = glm::dot(t, vel_t);
//
//		// masses 
//		float m_s = r_s * r_s * r_s;
//		float m_t = r_t * r_t * r_t;
//		float massTerm_n = m_s * vel_n_s + m_t * vel_n_t;
//		float massTerm_t = m_s * vel_t_s + m_t * vel_t_t;
//
//		float plasticTerm_n_s = m_t * (vel_n_s - vel_n_t) * elatsicity;
//		//float plasticTerm_t_s = m_t * (vel_t_s - vel_t_t) * elatsicity;
//
//		float plasticTerm_n_t = m_s * (vel_n_t - vel_n_s) * elatsicity;
//		//float plasticTerm_t_t = m_s * (vel_t_t - vel_t_s) * elatsicity;
//
//		float vel_n_s_update = (massTerm_n - plasticTerm_n_s) / (m_s + m_t);
//		//float vel_t_s_update = (massTerm_t - plasticTerm_t_s) / (m_s + m_t);
//								
//		float vel_n_t_update = (massTerm_n - plasticTerm_n_t) / (m_s + m_t);
//		//float vel_t_t_update = (massTerm_t - plasticTerm_t_t) / (m_s + m_t);
//
//		vel_s = vel_t_s * t + vel_n_s_update * n;
//		vel_t = vel_t_t * t + vel_n_t_update * n;
//
//		// finally, finish the timestep
//		center_s += dT_remaining * vel_s;
//		center_t += dT_remaining * vel_t;
//
//		source_mov.vel += dT_remaining * glm::vec2(0.0f, g);
//		target_mov.vel += dT_remaining * glm::vec2(0.0f, g);
//	}
//}
