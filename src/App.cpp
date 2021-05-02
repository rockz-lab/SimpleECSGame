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
    //collisionsystem = manager.RegisterSystem<CollisionSystem>();
    spriteSystem = manager.RegisterSystem<SpriteExampleSystem>();

    //applyforceSystem = manager.RegisterSystem<ApplyForces>();

    manager.SetSystemSignature<RenderSystem,
        Gravity, Transform2D, RigidBodyState, Quad, TexCoords<4>>();

    manager.SetSystemSignature<PhysicsSystem,
        Gravity, Transform2D, RigidBodyState, Quad, TexCoords<4>>();

    manager.SetSystemSignature<SpriteExampleSystem,
        Gravity, Transform2D, RigidBodyState, Quad, TexCoords<4>, LifeTimer>();

    //manager.SetSystemSignature<CollisionSystem, Transform2D>();
    //manager.SetSystemSignature<ApplyForces, Transform2D, RigidBodyState, Triangle>();

    /*const int numCircles = 4;
    const int screenHeight = 1080;
    const int screenWidth = 1920;*/

   

    //MakeCircles circleFactory(&manager);
    //circleFactory.g = 10;

    //std::vector<eID> allEntities;
    //for (int i = 0; i < numCircles; i++)
    //{
    //    auto newEntity = circleFactory.MakeRandomCircle();

    //    allEntities.push_back(newEntity);
    //}

    //MakePolys triangleFactory(&manager);
    //std::array<vec2, 3> triVertices = { vec2{0.0, 0.0}, vec2{ -100.0, 0.0 }, vec2{ -50.0, 200.0} };
    //auto triangle1 = triangleFactory.MakeTriangle(triVertices, { 1000, 100});
    //auto triangle2 = triangleFactory.MakeTriangle(triVertices, { 950, 100 });

    //manager.RemoveComponent<Gravity>(triangle2);
    //
    ////manager.AddComponent<Triangle<3>>(triangle, testTriangle);

    //// create line at the Edges of the screen
    //eID aLine = manager.CreateEntity();

    //
    //Transform2D defaultTform;
    //defaultTform.pos = {}; defaultTform.rotation = 0;
    //manager.AddComponent<Transform2D>(aLine, defaultTform);

    //Line lineComp;
    //lineComp.p1 = { 0.0f, 1000.0f};
    //lineComp.p2 = { 1920.0f, 1000.0f };
    //manager.AddComponent<Line>(aLine, lineComp);
    //Color color;
    //color.r = 255; color.g = 255; color.b = 255;
    //manager.AddComponent<Color>(aLine, color);
    m_window = std::make_unique<Window>(1000, 1000, "Game");

    std::string atlas = SOURCE_DIRECTORY + "/resource/textures/spritesheet.png"s;
    std::string metaData = SOURCE_DIRECTORY + "/resource/textures/spritesheet.json"s;
    texManager = std::make_shared<TextureManager>(atlas, metaData);

    spriteSystem->Init(m_window, texManager);


    rendersystem->Init(m_window);
    //applyforceSystem->Init(m_window);
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
