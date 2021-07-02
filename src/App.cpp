#include "App.h"
#include "Factories.h"


#include <type_traits>
#include <chrono>

extern ECSManager manager;

// initialize the counter
CompType BaseComponent::m_counter = 0;

using namespace std::string_literals;

void OnWindowPosChanged(GLFWwindow* win, int x, int y)
{
    void* ptr = glfwGetWindowUserPointer(win);
    App* application = reinterpret_cast<App*>(ptr);

    application->OnRedraw();
}

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
    m_window->SetUserPointer(this);
    m_window->SetWindowPosCallback(OnWindowPosChanged);

    std::string atlas = SOURCE_DIRECTORY + "/resource/textures/spritesheet.png"s;
    std::string metaData = SOURCE_DIRECTORY + "/resource/textures/spritesheet.json"s;
    texManager = std::make_shared<TextureManager>(atlas, metaData);

    spriteSystem->Init(m_window, texManager);


    rendersystem->Init(m_window);

    //eventLoopThread = std::thread([&]() {this->m_window->EventLoopThread(); });
}

void App::Run()
{
    auto t0 = std::chrono::system_clock::now();

    while (*m_window)
    {
		physicssystem->Update(dT);
        spriteSystem->Update(dT);
        
        rendersystem->Update();
        m_window->Update();

		auto t1 = std::chrono::system_clock::now();
		dT = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()) * 1e-6f;
		t0 = t1;
    }
    
}
