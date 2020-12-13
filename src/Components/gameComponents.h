#pragma once


#include "ecs/ecs.h"

#include <glm/glm.hpp>


struct Transform2D : public Component<Transform2D>
{
	glm::vec2 pos;
	glm::mat2 R;
};

struct MovementState : public Component<MovementState>
{
	glm::vec2 vel;
	glm::vec2 accel;
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