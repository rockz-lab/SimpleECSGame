#include <iostream>

#include "ecs/ecs.h"
ECSManager manager; // needs to be defined

#include "Components/gameComponents.h"
#include "Systems/physicsSystem.h"
#include "Systems/renderSystem.h"

#include <chrono>
#include <thread>
#include <random>
#include <SFML/Graphics.hpp>

glm::mat2 makeRot(float alpha)
{
    float c = cos(alpha);
    float s = sin(alpha);

    glm::mat2 m(0);
    m[0][0] = c;
    m[1][0] = s;
    m[0][1] = -s;
    m[1][1] = c;
    return m;
}
int main()
{

    manager.Init();

    manager.RegisterComponent<Gravity>();
    manager.RegisterComponent<Transform2D>();
    manager.RegisterComponent<MovementState>();
    manager.RegisterComponent<Circle>();
    manager.RegisterComponent<Color>();
    manager.RegisterComponent<Line>();

    //manager.RegisterComponent<TestBuffer<1000>>();

    auto physicssystem = manager.RegisterSystem<PhysicsSystem>();
    auto rendersystem = manager.RegisterSystem<RenderSystem>();
    auto collisionsystem = manager.RegisterSystem<CollisionSystem>();

    manager.SetSystemSignature<PhysicsSystem, Gravity, Transform2D, MovementState, Circle>();
    manager.SetSystemSignature<RenderSystem, Transform2D, Circle, Color>();
    manager.SetSystemSignature<CollisionSystem, Transform2D>();

    const float screenWidth = 1920;
    const float screenHeight = 1080;

    Gravity grav({ 9.81f*10 });
    Circle circle{ 10.0 };
    Line testLine{glm::vec2(20, 20), glm::vec2(120, 20)};
    
    // add randomly colored and distributed circles
    const int numCircles = 800;

    //std::random_device rd;
    std::mt19937 gen(12);

    std::uniform_int_distribution<> randColor(0, 255);
    std::uniform_real_distribution<float> initPos(-250, 250);
    std::normal_distribution<float> initVel(0, 100);
    std::uniform_real_distribution<float> circleRadius(1, 10);

    std::vector<eID> allEntities;
    for (int i = 0; i < numCircles; i++)
    {
        auto newEntity = manager.CreateEntity();

        circle.radius = circleRadius(gen);
        manager.AddComponent<Circle>(newEntity, circle);
        manager.AddComponent<Color>(newEntity, Color { uint8_t(randColor(gen)), uint8_t(randColor(gen)) , uint8_t(randColor(gen)) });
        manager.AddComponent<Gravity>(newEntity, grav);
        manager.AddComponent<MovementState>(newEntity, MovementState
            {
                glm::vec2{initVel(gen), initVel(gen)},
                {},
            });
        manager.AddComponent<Transform2D>(newEntity, Transform2D
            {
                glm::vec2{initPos(gen) + screenWidth/2, initPos(gen) + screenHeight/2} , {}
            });

        allEntities.push_back(newEntity);
    }

    // create line at the Edges of the screen
    eID bottomLine = manager.CreateEntity();
    eID topLine = manager.CreateEntity();
    eID leftLine = manager.CreateEntity();
    eID rightLine = manager.CreateEntity();


    manager.AddComponent<Transform2D>(bottomLine, { {}, {} });
    manager.AddComponent<Transform2D>(topLine, { {}, {} });
    manager.AddComponent<Transform2D>(leftLine, { {}, {} });
    manager.AddComponent<Transform2D>(rightLine, { {}, {} });

    manager.AddComponent<Line>(bottomLine,      Line{ glm::vec2{0.0f, screenHeight}, glm::vec2{screenWidth, screenHeight} });
    manager.AddComponent<Line>(topLine,         Line{ glm::vec2{0.0f, 0.0f}, glm::vec2{screenWidth, 0.0f} });
    manager.AddComponent<Line>(leftLine,        Line{ glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, screenHeight} });
    manager.AddComponent<Line>(rightLine,       Line{ glm::vec2{screenWidth, 0.0f}, glm::vec2{screenWidth, screenHeight} });

    using namespace std::chrono;
    using namespace std::chrono_literals;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode((unsigned int)screenWidth, (unsigned int)screenHeight), "CausalGame", sf::Style::Fullscreen, settings);
    window->setVerticalSyncEnabled(true);

    rendersystem->Init(window);

    auto t0 = system_clock::now();
    while (window->isOpen())
    {
        window->clear(sf::Color(0, 0, 0));

        sf::Event event;
        while (window->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.key.code == sf::Keyboard::Escape)
            {
                window->close(); 
            }

            if (event.key.code == sf::Keyboard::BackSpace)
            {
                manager.DestroyEntity(allEntities.back());
                allEntities.pop_back();
            }
        }

        auto t1 = system_clock::now(); 
        double dT = static_cast<double>(duration_cast<microseconds>(t1 - t0).count()) * 1e-6;
        t0 = t1;


        collisionsystem->Update(dT);
        physicssystem->Update(dT);
        rendersystem->Update(dT);
        window->display();
        printf("FPS: %f\n", 1.0f / dT);
    }
   
}
