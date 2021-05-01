#include "SpriteExampleSystem.h"

void SpriteExampleSystem::Init(std::shared_ptr<Window> window, std::shared_ptr<TextureManager> texManager)
{
	m_spriteCreator = std::make_shared<SpriteCreator>(texManager);
	m_source = std::make_unique<RandomSpriteSource>(m_spriteCreator, 300, 1);
}

void SpriteExampleSystem::Update(float dT)
{
	m_source->TrySpawn(vec2(500, 500));
}
