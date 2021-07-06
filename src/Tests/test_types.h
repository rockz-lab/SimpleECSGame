#include <nlohmann/json.hpp>


struct Vec2
{
	float x;
	float y;
	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vec2, x, y)
};