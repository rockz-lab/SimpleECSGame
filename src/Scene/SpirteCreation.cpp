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

void SpriteCreator::SpawnSprite(const std::string& name, const Factory::InitialPos& pos, const Factory::InitialMov& mov)
{
	eID entity = manager.CreateEntity();

	std::array<vec2, 4> coords = m_textureManager->GetTexCoords(name);
	
	const int width = 50;
	std::array<vec2, 4> verts = { vec2(-width/2, -width/2), vec2(-width/2, width/2), vec2(width/2, width/2), vec2(width/2, -width/2)};
	Factory::makeSprite(entity, verts, coords);
	
	Factory::setSpriteICs(entity, pos, mov);

	Gravity g;
	manager.AddComponent(entity, g);


}

RandomSpriteSource::RandomSpriteSource(std::shared_ptr<SpriteCreator> spriteCreator)
{
	m_spriteCreator = spriteCreator;
}

RandomSpriteSource::RandomSpriteSource(std::shared_ptr<SpriteCreator> spriteCreator, float frequency, float initialSpeed) : RandomSpriteSource(spriteCreator)
{
	m_frequency = frequency;
	m_initialSpeed = initialSpeed;
}

void RandomSpriteSource::TrySpawn(const vec2& position)
{
	// synchronously try to spawn sprites
	
	if (m_elapsedTime > 1 / m_frequency)
	{
		Factory::InitialPos pos = { position, 0.0f };

		std::mt19937 gen(12);
		std::uniform_real_distribution x_dist(-m_initialSpeed, m_initialSpeed);
		std::uniform_real_distribution y_dist(-2*m_initialSpeed, 0.0f);

		Factory::InitialMov mov = { vec2(x_dist(gen), y_dist(gen)), 0.0f };

		m_spriteCreator->SpawnSprite("one", pos, mov);
	}

	auto t_old = m_t_now;
	m_t_now = std::chrono::high_resolution_clock::now();
	m_elapsedTime = (m_t_now - t_old).count();
}
