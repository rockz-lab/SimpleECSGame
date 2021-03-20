#pragma once


#include "ecs/ecs.h"
#include "Utils/collision.h"

#include <glm/glm.hpp>
#include "Utils/math.h"

using namespace YAML;	

struct Transform2D : public Component<Transform2D>
{

	//glm::vec2 pos{};
	vec3 pos;
	float rotation{};

	void serialize_impl(std::ostream& os)
	{
		os << "Pos:" << pos << "Rotation" << rotation;
	}
};

struct RigidBodyState : public Component<RigidBodyState>
{
	float mass;
	float angMass;
	glm::vec2 centerPos{};
	glm::vec2 momentum{};
	float rotation{};
	float angMomentum{};

	glm::vec2 centerPos_o;
	glm::vec2 momentum_o;
	float rotation_o;
	float angMomentum_o;

	glm::vec2 force = {};
	float torque = 0;
};

struct Gravity : public Component<Gravity>
{
	float g = 9.81f;
};

struct Circle : public Component<Circle>
{
	float radius = 20.0f;
};

// Should your do vertex buffers, etc as a component?? 
// For large objects, maybe just use heap dynamic array ..
//template <size_t S>
//struct TestBuffer
//{
//	std::array<float, S> vertices;
//};

struct Color : public Component<Color>
{
	uint8_t r = 128;
	uint8_t g = 128;
	uint8_t b = 128;
};

struct Line : public Component<Line>
{
	glm::vec2 p1;
	glm::vec2 p2;
};




struct Polygon : public Component<Polygon>
{
	int numVerts{};
	coll::PolygonData poly;
};



struct CollisionState : public Component<CollisionState>
{
	bool status = false;
};


