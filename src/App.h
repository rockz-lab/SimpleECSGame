#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/ECS.h"

#include "Components/gameComponents.h"
#include "Systems/physicsSystem.h"
#include "Systems/renderSystem.h"

#include "Systems/ApplyForces.h"

class App
{
public:
	App();

	void Run();

private:
	std::shared_ptr<sf::RenderWindow> m_window;

	std::shared_ptr<RenderSystem> rendersystem;
	std::shared_ptr<PhysicsSystem> physicssystem;
	std::shared_ptr<CollisionSystem> collisionsystem;
	std::shared_ptr<ApplyForces> applyforceSystem;
};

