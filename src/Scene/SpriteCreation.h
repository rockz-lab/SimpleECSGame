#pragma once

#include "Renderer/TextureManager.h"

#include <memory>
#include <chrono>

#include "Factories.h"

class SpriteCreator
{
public:
	SpriteCreator(std::shared_ptr<TextureManager> textureManager);

	eID SpawnSprite(const std::string& name, const Factory::InitialPos& pos, const Factory::InitialMov& mov);

private:
	std::shared_ptr<TextureManager> m_textureManager;
};

class RandomSpriteSource
{
public:
	RandomSpriteSource(std::shared_ptr<SpriteCreator> spriteCreator);
	//RandomSpriteSource(std::shared_ptr<SpriteCreator> spriteCreator, float frequency, float initialSpeed);
	RandomSpriteSource& Frequency(float frequency) { this->m_frequency = frequency; return *this; }
	RandomSpriteSource& Speed(float speed);
	RandomSpriteSource& LifeTime(float time) { this->m_lifeTime = time; return *this; }

	void TrySpawn(const vec2& position);
private:
	std::random_device rd;
	std::mt19937 gen;

	std::uniform_real_distribution<float> x_dist;
	std::uniform_real_distribution<float> y_dist;

	std::shared_ptr<SpriteCreator> m_spriteCreator;
	float m_frequency = 5;
	float m_initialSpeed = 100;
	float m_lifeTime = 10;
	std::chrono::steady_clock::time_point t_last;
	float m_elapsedTime = 0.0f;
};