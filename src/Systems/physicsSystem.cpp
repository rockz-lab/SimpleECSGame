#include "physicsSystem.h"

#include <iostream>

void PhysicsSystem::CollideCirles(eID source, eID target)
{
	auto& tform1 = manager.GetComponent<Transform2D>(source);
	auto& circle1 = manager.GetComponent<Circle>(source);

	auto& tform2 = manager.GetComponent<Transform2D>(target);
	auto& circle2 = manager.GetComponent<Circle>(target);

	auto& r_s = circle1.radius;
	auto& r_t = circle2.radius;

	glm::vec2& center_s = tform1.pos;
	glm::vec2& center_t = tform2.pos;

	// Test for collisions.
	float penetration;
	if (sec::circle_circle(r_s, r_t, center_s, center_t, &penetration))
	{
		// calculate normal and tangent vectors at collision point
		glm::vec2 n = glm::normalize(center_s - center_t);		// (source -> target)
		glm::vec2 t = {n.y, -n.x};

		// static resolution
        center_s += 0.5f * glm::sqrt(penetration) * n;
        center_t -= 0.5f * glm::sqrt(penetration) * n;

		// Dynamic response. Here only the velocities change

		auto& vel_s = manager.GetComponent<MovementState>(source).vel;
		auto& vel_t = manager.GetComponent<MovementState>(target).vel;

		// Project vel onto the normal and tangent directions (using dotproduct)
		float vel_n_s = glm::dot(n, vel_s);
		float vel_t_s = glm::dot(t, vel_s);

		float vel_n_t = glm::dot(n, vel_t);
		float vel_t_t = glm::dot(t, vel_t);

		float vel_n_s_update = 2 * (r_s * vel_n_s + r_t * vel_n_t) / (r_s + r_t) - vel_n_s;
		float vel_t_s_update = 2 * (r_s * vel_t_s + r_t * vel_t_t) / (r_s + r_t) - vel_t_s;
		
		float vel_n_t_update = 2 * (r_s * vel_n_s + r_t * vel_n_t) / (r_s + r_t) - vel_n_t;
		float vel_t_t_update = 2 * (r_s * vel_t_s + r_t * vel_t_t) / (r_s + r_t) - vel_t_t;

		vel_s = vel_t_s_update * t + vel_n_s_update * n;
		vel_t = vel_t_t_update * t + vel_n_t_update * n;

	}
}
