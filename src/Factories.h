#pragma once
#include <random>

#include "ecs/ecs.h"
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

	eID MakeTriangle(std::array<glm::vec2, 3> const& points, glm::vec2 centerPos);
private:
	
	ECSManager* m_manager;
};

