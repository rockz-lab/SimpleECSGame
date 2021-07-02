#pragma once

#include "ECS/ECS.h"

#include "Renderer/Window.h"

#include "Systems/physicsSystem.h"
#include "Systems/renderSystem.h"
#include "Systems/SpriteExampleSystem.h"

#include <thread>

class App
{
public:
	App();

	void Run();

	void OnRedraw()
	{
		m_window->Clear();
		rendersystem->Update();
		m_window->Draw();
	}
private:
	float dT = 0;
	bool debug_not_called = true;

	std::shared_ptr<Window> m_window;
	//std::thread eventLoopThread;

	std::shared_ptr<RenderSystem> rendersystem;
	std::shared_ptr<PhysicsSystem> physicssystem;
	//std::shared_ptr<CollisionSystem> collisionsystem;
	std::shared_ptr<SpriteExampleSystem> spriteSystem;

	std::shared_ptr<TextureManager> texManager;


};

