#pragma once

#include <vector>
#include <array>

#include <glm/glm.hpp>
#include <Eigen/Core>

#include "Components/gameComponents.h""
#include "Utils/Collision.h"
//#include "ecs/ecsTypes.h"
#include <ECS/ecs.h>

// The solver implented is based on the tutorial given here: http://www.mft-spirit.nl/files/MTamis_ConstraintBasedPhysicsSolver.pdf

// each constraint can be described by a constraint, a so called push factor and limits on the forces
// different types differ in how those vectors are constructed. The constraint axis is constructed for

// the constraint axis dot the velocity and rotation vector needs to be zero to fullfill the constraint.
// because the constaint in the current timestep is generally not fullfilled. Thus, push factor is introduced
// and we solve jc v = push_factor, where v contains the velocities vectors and rotational speeds of bodies a and b:
// [v_a, omega_a, v_b, omega_b]. 

// J: c x 3n

struct ContactManifold
{
	glm::vec2 contactNormal;
	std::vector<glm::vec2> contactPts;
	glm::vec2 centerPosA;
	glm::vec2 centerPosB;
	float overlap;
	eID bodyA;
	eID bodyB;
};

struct ConstraintData
{
	std::array<float, 6> j_c;
	float c_min;
	float c_max;
	float push_factor;
	eID bodyA;
	eID bodyB;
};



inline ContactManifold makeContactManifold(const coll::PolygonData &ref, const coll::PolygonData &incident, int collisionFaceIndex)
{
	// identify the collision faces
	extern ECSManager manager;

	ContactManifold manifold;
	
	// go along the normal and identify the closest point to the collision normal
	glm::vec2 normal = ref.normals[collisionFaceIndex];

	float maxOverlap = 0;
	glm::vec2 incidentPoint1;
	for (auto& p : incident.vertices)
	{
		float overlap = glm::dot(normal, p);
		if (overlap > maxOverlap)
		{
			maxOverlap = overlap;
			incidentPoint1 = p;
		}
	}
	manifold.contactPts.push_back(incidentPoint1);
	//collisionFaceIndex
	
	return manifold;
}

std::vector<ConstraintData> makeContactConstraint(const ContactManifold& manifold, float dT, float beta);


class ConstraintSolver
{
public:
	void solve();
	//void addBody(eID entity);
	void AddConstraint(const ConstraintData &cData);
private:
	
	std::vector<ConstraintData> constraints;
};

