#pragma once
#include <glm/glm.hpp>

#include <iostream>
// Serializable interface

//class ISerializable
//{
//
//};

struct vec2
{
	glm::vec2 m_vec{};

	friend std::ostream& operator<<(std::ostream& os, const vec2& vec)
	{
		os << vec.m_vec.x << ", " << vec.m_vec.y << "; ";
		return os;
	}
	
	vec2 operator-(const vec2& other)
	{
		vec2 result;
		result.m_vec = ( this->m_vec - other.m_vec);
		return result;
	}

	vec2 operator+(const vec2& other)
	{
		vec2 result;
		result.m_vec = (this->m_vec + other.m_vec);
		return result;
	}

	vec2 operator*(const vec2& other)
	{
		vec2 result;
		result.m_vec = (this->m_vec * other.m_vec);
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
};