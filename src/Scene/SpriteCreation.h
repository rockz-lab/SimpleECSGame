#pragma once

#include "Renderer/TextureManager.h"

#include <memory>
#include <chrono>

#include "Factories.h"

class SpriteCreator
{
public:
	SpriteCreator(std::shared_ptr<TextureManager> textureManager);

	void SpawnSprite(const std::string& name, const Factory::InitialPos& pos, const Factory::InitialMov& mov);

private:
	std::shared_ptr<TextureManager> m_textureManager;
};

class RandomSpriteSource
{
public:
	RandomSpriteSource(std::shared_ptr<SpriteCreator> spriteCreator);
	RandomSpriteSource(std::shared_ptr<SpriteCreator> spriteCreator, float frequency, float initialSpeed);

	void TrySpawn(const vec2& position);
private:
	std::shared_ptr<SpriteCreator> m_spriteCreator;
	float m_frequency = 5;
	float m_initialSpeed = 100;
	std::chrono::steady_clock::time_point m_t_now;
	float m_elapsedTime = 0.0f;
};