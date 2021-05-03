#include "SpriteExampleSystem.h"

void SpriteExampleSystem::Init(std::shared_ptr<Window> window, std::shared_ptr<TextureManager> texManager)
{
	m_spriteCreator = std::make_shared<SpriteCreator>(texManager);
	m_source = std::make_unique<RandomSpriteSource>(m_spriteCreator);
	m_source->Frequency(60).Speed(4).LifeTime(1000);
}

void SpriteExampleSystem::Update(float dT)
{
	m_source->TrySpawn(vec2(500, 500));

	auto iter = m_entitiyLists[0].begin();

	while (iter != m_entitiyLists[0].end())
	{
		auto& entity = *iter;

		auto& timeToDie = manager.GetComponent<LifeTimer>(entity).timeToDie;

		timeToDie -= dT;
		if (timeToDie < 0)
		{
			eID entityToDel = *(iter++);	// we first need to increment the iterator 
			manager.DestroyEntity(entityToDel);	
		} else
			++iter;
	}
}
