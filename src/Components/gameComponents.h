#pragma once


#include "ecs/ecs.h"

#include <glm/glm.hpp>


struct Transform2D
{
	glm::vec2 pos;
	glm::mat2 R;
};

struct MovementState
{
	glm::vec2 vel;
	glm::vec2 accel;
};

struct Gravity
{
	float g = 9.81f;
};

struct DrawableCircle
{
	float radius;
};

// Should your do vertex buffers, etc as a component?? 
// For large objects, maybe just use heap dynamic array ..
template <size_t S>
struct TestBuffer
{
	std::array<float, S> vertices;
};

struct Color
{
	uint32_t r;
	uint32_t g;
	uint32_t b;
};