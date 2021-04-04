#include "Factories.h"

MakeCircles::MakeCircles(ECSManager* manager)
{

	m_manager = manager;

	using uniform_int = std::uniform_int_distribution<>;
	using uniform_real = std::uniform_real_distribution<float>;

	// Random generation
	std::random_device r;
	m_rng = std::make_shared<std::mt19937>(r());
	randColor = std::make_unique<uniform_int>(0, 255);
	initPos = std::make_unique<uniform_real>(-250, 250);
	initVel = std::make_unique<std::normal_distribution<float>>(0, 100);
	circleRadius = std::make_unique<uniform_real>(3, 30);
}

eID MakeCircles::MakeCircle(float radius, glm::vec<3, uint8_t> color, vec2 startPos)
{
	auto entity = m_manager->CreateEntity();

	// Circle data
	Circle c;
	c.radius = radius;
	m_manager->AddComponent(entity, c);

	// Transform
	Transform2D tForm;
	tForm.pos = startPos;
	// Color
	Color col;
	col.r = color.r;
	col.g = color.g;
	col.b = color.b;
	// Movement State
	RigidBodyState movState;
	movState.centerPos = { { 0.0f, 0.0f } };
	// gravity
	Gravity grav;
	grav.g = 9.81f;
	return entity;
}

eID MakeCircles::MakeRandomCircle()
{
	auto entity = m_manager->CreateEntity();

	// Circle data
	Circle c;
	c.radius = (*circleRadius)(*m_rng);
	m_manager->AddComponent(entity, c);

	// Transform
	Transform2D tForm;
	tForm.pos = { (*initPos)(*m_rng) + 500.0f, (*initPos)(*m_rng) + 500.0f };
	m_manager->AddComponent(entity, tForm);

	// Color
	Color col;
	col.r = (*randColor)(*m_rng);
	col.g = (*randColor)(*m_rng);
	col.b = (*randColor)(*m_rng);
	m_manager->AddComponent(entity, col);

	// Movement State
	RigidBodyState movState;
	movState.centerPos = tForm.pos;
	m_manager->AddComponent(entity, movState);

	// gravity
	Gravity grav;
	grav.g = g;
	m_manager->AddComponent(entity, grav);
	return entity;
}


MakePolys::MakePolys(ECSManager* manager)
{
	m_manager = manager;
}

eID MakePolys::MakeTriangle(std::array<glm::vec2, 3> const& points, glm::vec2 centerPos)
{
	using std::pair;
	eID entity = m_manager->CreateEntity();

	Triangle triangle;

	triangle.vertexData.vertices = points;

	// translation of the triangle center
	glm::vec2 center = 1.0f / 3.0f * (points[0] + points[1] + points[2]);
	for (auto& v : triangle.vertexData.vertices)
		v -= center;

	auto indices = { pair<int, int>(0, 1), pair<int, int>(1, 2), pair<int, int>(2, 0) };

	std::array<glm::vec2, 3> n;
	int i = 0;
	for (auto const& lineIndices : indices)
	{
		glm::vec2& lineDir = points[lineIndices.second] - points[lineIndices.first];
		glm::vec2 normal = glm::normalize(glm::vec2{ -lineDir.y, lineDir.x });
		n[i] = normal;
		i++;
	}
	triangle.vertexData.normals = n;

	m_manager->AddComponent(entity, triangle);

	Color defaultColor;
	defaultColor.r = 128;
	defaultColor.g = 128;
	defaultColor.b = 128;

	m_manager->AddComponent(entity, defaultColor);

	Transform2D transform;

	transform.pos = centerPos;
	m_manager->AddComponent(entity, transform);


	// add rigid body Physics:

	RigidBodyState physics;
	physics.angMass = 20000.0f;
	physics.mass = 10;
	physics.angMomentum = 0;
	physics.centerPos = center;
	physics.torque = 0;

	m_manager->AddComponent(entity, physics);
	Gravity grav;
	grav.g = 200.0;
	m_manager->AddComponent(entity, grav);

	CollisionState collision;
	collision.status = false;
	m_manager->AddComponent(entity, collision);

	return entity;
}
