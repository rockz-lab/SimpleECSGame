#pragma once
#include <random>

#include "ecs/ecs.h"

#include "Utils/math.h"
#include "Components/gameComponents.h"
#include "glm/glm.hpp"


// Factory classes create different types of entities and their components
class MakeCircles
{
public:
	MakeCircles(ECSManager* manager);
	
	eID MakeCircle(float radius, glm::vec<3, uint8_t> color, vec2 startPos);

	eID MakeRandomCircle();
	glm::vec<3, uint8_t> defaultColor;
	float g = 9.81;
private:
	ECSManager* m_manager;

	std::shared_ptr<std::mt19937> m_rng;
	
	std::unique_ptr<std::uniform_int_distribution<>> randColor;
	std::unique_ptr<std::uniform_real_distribution<float>> initPos;
	std::unique_ptr<std::normal_distribution<float>> initVel;
	std::unique_ptr<std::uniform_real_distribution<float>> circleRadius;
};

class MakePolys
{
public:
	MakePolys(ECSManager* manager);

	eID MakeTriangle(std::array<vec2, 3> const& points, vec2 centerPos);
private:
	float GetMass(const Triangle& tri, float density);
	float GetAngularMoment(const Triangle& tri);
	
	ECSManager* m_manager;
};


// new style of factories 
namespace Factory
{
	struct InitialMov
	{
		vec2 velocity;
		float angularSpeed;
	};

	struct InitialPos
	{
		vec2 pos;
		float rotation;
	};

	void makeSprite(eID entity, const std::array<vec2, 4>& quadVerts, const std::array<vec2, 4>& texCoords);

	void addSpritePhysicsICs(eID entity, const InitialPos& pos, const InitialMov& movement);

}
