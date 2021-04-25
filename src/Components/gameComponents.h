#pragma once


#include "ECS/Component.h"
#include "Utils/collision.h"

#include <glm/glm.hpp>
#include "Utils/math.h"

//#include <nlohmann/json.hpp>
//using json = nlohmann::json;

//#include "Utils/Serialization.h"

struct Transform2D : public Component<Transform2D>
{
	//glm::vec2 pos{};
	vec2 pos{};
	float rotation{};
};

struct Gravity : public Component<Gravity>
{
	float g = 9.81f;
};

struct Circle : public Component<Circle>
{
	float radius = 0.0f;
};

struct RigidBodyState : public Component<RigidBodyState>
{
	float mass;
	float angMass;
	vec2 centerPos{};
	vec2 momentum{};
	float rotation{};
	float angMomentum{};

	vec2 centerPos_o;
	vec2 momentum_o;
	float rotation_o;
	float angMomentum_o;

	vec2 force = {};
	float torque = 0;
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
	vec2 p1;
	vec2 p2;
};

template <int N>
struct Polygon : public Component<Polygon<N>>
{
	Static_Poly<N> vertexData;
};

using Triangle = Polygon<3>;

struct CollisionState : public Component<CollisionState>
{
	bool status = false;
};

template <int N>
struct TexCoords : public Component<TexCoords<N>>
{
	std::array<vec2, N> coords;
};

using Quad = Polygon<4>;

#define STR_LEN 20
struct SpriteName : public Component<SpriteName>
{
	char name[STR_LEN];
};
