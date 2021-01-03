#include "App.h"
#include "Factories.h"


#include <chrono>

// initialize the counter
CompType BaseComponent::m_counter = 0;


App::App()
{
    manager.Init();
    
    manager.RegisterComponent<Gravity>();
    manager.RegisterComponent<Transform2D>();
    manager.RegisterComponent<RigidBodyState>();
    manager.RegisterComponent<Line>();
    manager.RegisterComponent<Color>();
    manager.RegisterComponent<CollisionState>();

    manager.RegisterComponent<Circle>();
    manager.RegisterComponent<Polygon>();

    physicssystem = manager.RegisterSystem<PhysicsSystem>();
    rendersystem = manager.RegisterSystem<RenderSystem>();
    collisionsystem = manager.RegisterSystem<CollisionSystem>();
    applyforceSystem = manager.RegisterSystem<ApplyForces>();

    manager.SetSystemSignature<PhysicsSystem, Gravity, Transform2D, RigidBodyState, Polygon>();
    manager.SetSystemSignature<RenderSystem, Transform2D, Polygon, Color>();
    manager.SetSystemSignature<RenderSystem, Line>();

    manager.SetSystemSignature<CollisionSystem, Transform2D>();
    manager.SetSystemSignature<ApplyForces, Transform2D, RigidBodyState, Polygon>();

    const int numCircles = 4;
    const int screenHeight = 1080;
    const int screenWidth = 1920;
    
    MakeCircles circleFactory(&manager);
    circleFactory.g = 10;

    std::vector<eID> allEntities;
    for (int i = 0; i < numCircles; i++)
    {
        auto newEntity = circleFactory.MakeRandomCircle();

        allEntities.push_back(newEntity);
    }

    MakePolys triangleFactory(&manager);
    std::array<glm::vec2, 3> triVertices = { glm::vec2{0.0, 0.0}, glm::vec2{ -100.0, 0.0 }, glm::vec2{ -50.0, 200.0} };
    auto triangle1 = triangleFactory.MakeTriangle(triVertices, { 1000, 100});
    auto triangle2 = triangleFactory.MakeTriangle(triVertices, { 950, 100 });

    manager.RemoveComponent<Gravity>(triangle2);
    
    //manager.AddComponent<Polygon<3>>(triangle, testTriangle);

    // create line at the Edges of the screen
    eID aLine = manager.CreateEntity();

    Transform2D defaultTform;
    defaultTform.pos = {}; defaultTform.rotation = 0;
    manager.AddComponent<Transform2D>(aLine, defaultTform);

    Line lineComp;
    lineComp.p1 = { 0.0f, 1000.0f};
    lineComp.p2 = { 1920.0f, 1000.0f };
    manager.AddComponent<Line>(aLine, lineComp);
    Color color;
    color.r = 255; color.g = 255; color.b = 255;
    manager.AddComponent<Color>(aLine, color);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode((unsigned int)screenWidth, (unsigned int)screenHeight),
        "CausalGame",
        sf::Style::Fullscreen, settings);

    m_window->setVerticalSyncEnabled(true);

    rendersystem->Init(m_window);
    applyforceSystem->Init(m_window);
}

void App::Run()
{
    auto t0 = std::chrono::system_clock::now();

    while (m_window->isOpen())
    {
        m_window->clear(sf::Color(0, 0, 0));

        sf::Event event;
        while (m_window->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                m_window->close();
            if (event.key.code == sf::Keyboard::Escape)
            {
                m_window->close();
            }

        }

        auto t1 = std::chrono::system_clock::now();
        double dT = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()) * 1e-6;
        t0 = t1;


        applyforceSystem->Update(dT);
        physicssystem->Update(dT);
        
       collisionsystem->Update(dT);

        rendersystem->Update(dT);
        m_window->display();
        printf("FPS: %f\n", 1.0f / dT);
    }
}
