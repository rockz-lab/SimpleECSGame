#include "constraints.h"

void ConstraintSolver::solve()
{
	// create the matrices
	int c = constraints.size();
	
}


void ConstraintSolver::AddConstraint(const ConstraintData& cData)
{
	constraints.push_back(cData);
}

std::vector<ConstraintData> makeContactConstraint(const ContactManifold& manifold, float dT, float beta)
{
	std::vector<ConstraintData> constraints;
	auto& n = manifold.contactNormal;
	auto& c_a = manifold.centerPosA;
	auto& c_b = manifold.centerPosB;

	for (auto& cp : manifold.contactPts)
	{
		auto r_a = cp - c_a;
		auto r_b = cp - c_b;
		auto n_cross = glm::vec2{ -n.y, n.x };
		ConstraintData cData;
		cData.j_c = {
			-n.x, -n.y, -glm::dot(r_a, n_cross),
			n.x, n.y, glm::dot(r_b, n_cross)
		};

		cData.c_max = INFINITY;
		cData.c_min = 0;
		cData.bodyA = manifold.bodyA;
		cData.bodyB = manifold.bodyB;

		constraints.push_back(cData);
	}
	return constraints;
};