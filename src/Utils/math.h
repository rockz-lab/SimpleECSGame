#include <glm/glm.hpp>

#include <iostream>
// Serializable interface

class ISerializable
{

};

struct vec3
{
	glm::vec3 m_vec;

	friend std::ostream& operator<<(std::ostream& os, const vec3& vec)
	{
		os << vec.m_vec.x << vec.m_vec.y << vec.m_vec.z;
		return os;
	}
};