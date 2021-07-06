#include "ECS/Component.h"
#include "test_types.h"

struct Transform2D : public Component<Transform2D>
{
	Vec2 pos{};
	float rotation{};
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Transform2D, pos, rotation)
};

struct Circle : public Component<Circle>
{
	float radius = 0.0f;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Circle, radius)
};

struct Gravity : public Component<Gravity>
{
	float g = 9.81f;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Gravity, g)
};

