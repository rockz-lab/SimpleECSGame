#pragma once
#include <glm/glm.hpp>

#include <iostream>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

// Serializable interface

//class ISerializable
//{
//
//};
struct vec2;
float dot(const vec2& v1, const vec2& v2);
vec2 normalize(const vec2& v);

struct vec2
{
	vec2()
	{
		m_vec = {};
	}

	vec2(float x, float y)
	{
		m_vec = { x, y };
	}

	vec2(const glm::vec2& vec) : m_vec(vec) {};

	float& x = m_vec.x;
	float& y = m_vec.y;

	friend std::ostream& operator<<(std::ostream& os, const vec2& vec)
	{
		os << vec.m_vec.x << ", " << vec.m_vec.y << "; ";
		return os;
	}
	
	vec2& operator-=(const vec2& other)
	{
		this->m_vec -= other.m_vec;
		return *this;
	}
	friend vec2 operator-(vec2 one, const vec2& other)
	{
		vec2 result;
		result.m_vec = ( one.m_vec - other.m_vec);
		return result;
	}

	vec2 operator-()
	{
		return vec2(-this->m_vec);
	}

	friend vec2 operator+(vec2 one, const vec2& other)
	{
		vec2 result;
		result.m_vec = (one.m_vec + other.m_vec);
		return result;
	}

	vec2& operator+=(const vec2& other)
	{
		this->m_vec += other.m_vec;
		return *this;
	}

	vec2 operator*(const vec2& other)
	{
		vec2 result;
		result.m_vec = (this->m_vec * other.m_vec);
		return result;
	}

	vec2 operator*(float scalar)
	{
		vec2 result;
		result.m_vec = (this->m_vec * scalar);
		return result;
	}

	template<typename Scalar>
	vec2 operator/(const Scalar& other)
	{
		vec2 result;
		result.m_vec = this->m_vec / other;
		return result;
	}

	vec2& operator=(const vec2& other)
	{
		this->m_vec = other.m_vec;
		return *this;
	}
	
	glm::vec2& to_glm()
	{
		return this->m_vec;
	}

	const glm::vec2& to_glm() const
	{
		return this->m_vec;
	}
	friend float dot(const vec2& v1, const vec2& v2)
	{
		float out = glm::dot(v1.m_vec, v2.m_vec);
		return out;
	}

	friend vec2 normalize(const vec2& v)
	{
		 return vec2(glm::normalize(v.m_vec));
	}
private:
	glm::vec2 m_vec{};
};

