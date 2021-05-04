#include "App.h"
#include "Factories.h"


#include <type_traits>
#include <chrono>

// initialize the counter
CompType BaseComponent::m_counter = 0;

using namespace std::string_literals;

App::App()
{

    manager.Init();
    
    manager.RegisterComponents<Gravity, Transform2D, RigidBodyState, Quad, TexCoords<4>, LifeTimer>();
   
    physicssystem = manager.RegisterSystem<PhysicsSystem>();
    rendersystem = manager.RegisterSystem<RenderSystem>();
    spriteSystem = manager.RegisterSystem<SpriteExampleSystem>();


    manager.SetSystemSignature<RenderSystem,
        Gravity, Transform2D, RigidBodyState, Quad, TexCoords<4>>();

    manager.SetSystemSignature<PhysicsSystem,
        Gravity, Transform2D, RigidBodyState, Quad, TexCoords<4>>();

    manager.SetSystemSignature<SpriteExampleSystem,
        Gravity, Transform2D, RigidBodyState, Quad, TexCoords<4>, LifeTimer>();

        m_window = std::make_unique<Window>(1000, 1000, "Game");

    std::string atlas = SOURCE_DIRECTORY + "/resource/textures/spritesheet.png"s;
    std::string metaData = SOURCE_DIRECTORY + "/resource/textures/spritesheet.json"s;
    texManager = std::make_shared<TextureManager>(atlas, metaData);

    spriteSystem->Init(m_window, texManager);


    rendersystem->Init(m_window);
}

void App::Run()
{
    auto t0 = std::chrono::system_clock::now();

    while (*m_window)
    {
		physicssystem->Update(dT);
        spriteSystem->Update(dT);
		rendersystem->Update(dT);
        
        m_window->Update();
		auto t1 = std::chrono::system_clock::now();
		dT = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()) * 1e-6;
		t0 = t1;
    }
    
}
