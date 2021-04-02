#pragma once


#include "ECS/ECS.h"
#include "Utils/collision.h"

#include <glm/glm.hpp>
#include "Utils/math.h"

#include "Utils/Serialization.h"

struct Transform2D : public Component<Transform2D>
{
	//glm::vec2 pos{};
	vec2 pos{};
	float rotation{};

	void serialize_impl(std::ostream& os)
	{
		os << "Type: " << ID << "; " << "pos: " << pos << "rotation: " << rotation << "; ";
	}
	void deserialize_impl(Reader& reader, CompType compID)
	{
		// We just try out all ComponentTypes and serialize when they match
		if (compID == ID)
		{
			auto pos_entries = reader.GetProperty("pos");
			auto rotation_entries = reader.GetProperty("rotation");

			pos.m_vec.x = std::stof(pos_entries[0]);
			pos.m_vec.y = std::stof(pos_entries[1]);

			rotation = std::stof(rotation_entries[0]);
		}
	}
};

struct Gravity : public Component<Gravity>
{
	float g = 9.81f;

	void serialize_impl(std::ostream& os)
	{
		os << "Type: " << ID << "; " << "g: " << g << "; ";
	}

	void deserialize_impl(Reader& reader, CompType compID)
	{
		if (compID == ID)
		{
			auto g_entries = reader.GetProperty("g");

			g = std::stof(g_entries[0]);
		}
	}
};

struct Circle : public Component<Circle>
{
	float radius = 20.0f;

	void serialize_impl(std::ostream& os)
	{
		os << "Type: " << ID << "; " << "radius: " << radius << "; ";
	}

	void deserialize_impl(Reader& reader, CompType compID)
	{
		if (compID == ID)
		{
			auto radius_entries = reader.GetProperty("radius");

			radius = std::stof(radius_entries[0]);
		}
	}
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




struct Polygon : public Component<Polygon>
{
	int numVerts{};
	coll::PolygonData poly;
};



struct CollisionState : public Component<CollisionState>
{
	bool status = false;
};


