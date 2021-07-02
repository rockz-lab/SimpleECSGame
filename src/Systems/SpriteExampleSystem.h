#pragma once

#include "ecs/ecs.h"
#include "Components/gameComponents.h"


#include "Renderer/Window.h"
#include "Scene/SpriteCreation.h"

#include <glm/gtx/rotate_vector.hpp>

extern ECSManager manager;

class SpriteExampleSystem : public System
{
public:
	void Init(std::shared_ptr<Window> window, std::shared_ptr<TextureManager> texManager);

	void Update(float dT);
private:
	std::shared_ptr<Window> win;
	std::shared_ptr<SpriteCreator> m_spriteCreator;

	std::unique_ptr<RandomSpriteSource> m_source;
};