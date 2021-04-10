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

float MakePolys::GetAngularMoment(const Triangle& tri)
{
	const auto& points = tri.vertexData.vertices;

	vec2 baseDir = points[1] - points[0];
	vec2 normal = normalize(vec2(baseDir.y, -baseDir.x));

	float base = glm::distance(points[1].to_glm(), points[0].to_glm());
	float height = dot(normal, points[2] - points[1]);

	//vec2 centroid = 1 / 3.0f * (points[0] + points[1] + points[2]);

	float a = -dot(normalize(baseDir), (points[2] - points[1]));
	//return (height*pow(base, 3) - height*pow(base, 2)*a + height*pow(a, 2)*base + base*pow(height, 3)) / 36.0f;
	return (base * base - base * a + a * a + height * height) / 18.0f;
}

float MakePolys::GetMass(const Triangle& tri, float density)
{
	const auto& points = tri.vertexData.vertices;

	vec2 baseDir = points[1] - points[0];
	vec2 normal = normalize(vec2(baseDir.y, -baseDir.x));

	float base = distance(points[1].to_glm(), points[0].to_glm());
	float height = dot(normal, points[2] - points[1]);
	return base * height / 2.0f * density;
}
eID MakePolys::MakeTriangle(std::array<vec2, 3> const& points, vec2 centerPos)
{
	using std::pair;
	eID entity = m_manager->CreateEntity();

	Triangle triangle;

	triangle.vertexData.vertices = points;

	// translation of the triangle center
	vec2 center = (points[0] + points[1] + points[2]) * 1.0f / 3.0f;
	for (auto& v : triangle.vertexData.vertices)
		v -= center;

	auto indices = { pair<int, int>(0, 1), pair<int, int>(1, 2), pair<int, int>(2, 0) };

	std::array<vec2, 3> n;
	int i = 0;
	for (auto const& lineIndices : indices)
	{
		vec2& lineDir = points[lineIndices.second] - points[lineIndices.first];
		vec2 normal = normalize(glm::vec2{ -lineDir.y, lineDir.x });
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
	float density = 0.001f;
	physics.mass = GetMass(triangle, density);
	physics.angMass = GetAngularMoment(triangle)*physics.mass;
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
