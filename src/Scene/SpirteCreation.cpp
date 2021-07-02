#include "SpriteCreation.h"

#include <random>

extern ECSManager manager;

SpriteCreator::SpriteCreator(std::shared_ptr<TextureManager> textureManager)
{
	using namespace std::string_literals;

	auto atlas = SOURCE_DIRECTORY + "/resource/textures/spritesheet.png"s;
	auto metadata = SOURCE_DIRECTORY + "/resource/textures/spritesheet.json"s;
	m_textureManager = textureManager;
}

eID SpriteCreator::SpawnSprite(const std::string& name, const Factory::InitialPos& pos, const Factory::InitialMov& mov)
{
	// control the size/aspect ratio of the sprite
	eID entity = manager.CreateEntity();

	std::array<vec2, 4> coords = m_textureManager->GetTexCoords(name);
	
	const int width = 50;
	std::array<vec2, 4> verts = { vec2(-width/2, width/2), vec2(width/2, width/2), vec2(width/2, -width/2), vec2(-width/2, -width/2)};
	Factory::makeSprite(entity, verts, coords);
	
	Factory::addSpritePhysicsICs(entity, pos, mov);

	Gravity g;
	manager.AddComponent(entity, g);

	return entity;
}

RandomSpriteSource::RandomSpriteSource(std::shared_ptr<SpriteCreator> spriteCreator)
{
	m_spriteCreator = spriteCreator;
	gen = std::mt19937(rd());
}

RandomSpriteSource& RandomSpriteSource::Speed(float speed)
{ 
	this->m_initialSpeed = speed;

	x_dist = std::uniform_real_distribution(-m_initialSpeed, m_initialSpeed);
	y_dist = std::uniform_real_distribution(-m_initialSpeed, m_initialSpeed);
	return *this; 
}


void RandomSpriteSource::TrySpawn(const vec2& position)
{
	// synchronously try to spawn sprites based on the absolute texture Coords
	
	if (m_elapsedTime > 1 / m_frequency)
	{
		Factory::InitialPos pos = { position, 0.0f };
		
		Factory::InitialMov mov = { vec2(x_dist(gen), y_dist(gen)), 0.0f };

		eID entity;
		if (std::rand() > RAND_MAX/2)
		{
			entity = m_spriteCreator->SpawnSprite("two", pos, mov);
		}
		else
		{
			entity = m_spriteCreator->SpawnSprite("one", pos, mov);
		}

		LifeTimer timer;
		timer.timeToDie = m_lifeTime;
		manager.AddComponent<LifeTimer>(entity, timer);

		t_last = std::chrono::high_resolution_clock::now();
	}

	auto now = std::chrono::high_resolution_clock::now();
	m_elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - t_last).count();
}
