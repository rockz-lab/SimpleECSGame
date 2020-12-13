#include <iostream>

#include "ecs/ecs.h"
ECSManager manager; // needs to be defined
// initialize the counter
CompType BaseComponent::m_counter = 0;

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

    Gravity grav;
    grav.g = 9.81f;

    Circle circle;
    //Line testLine{glm::vec2(20, 20), glm::vec2(120, 20)};


    // add randomly colored and distributed circles
    const int numCircles = 90;

    //std::random_device rd;
    std::mt19937 gen(12);

    std::uniform_int_distribution<> randColor(0, 255);
    std::uniform_real_distribution<float> initPos(-250, 250);
    std::normal_distribution<float> initVel(0, 100);
    std::uniform_real_distribution<float> circleRadius(3, 30);

    std::vector<eID> allEntities;
    for (int i = 0; i < numCircles; i++)
    {
        auto newEntity = manager.CreateEntity();
        Color newColor;
        MovementState initMov;
        Transform2D initTForm;
        initTForm.pos = glm::vec2{ initPos(gen) + screenWidth / 2, initPos(gen) + screenHeight / 2 };

        circle.radius = circleRadius(gen);
        manager.AddComponent<Circle>(newEntity, circle);

        newColor.r = uint8_t(randColor(gen));
        newColor.g = uint8_t(randColor(gen));
        newColor.b = uint8_t(randColor(gen));
        manager.AddComponent<Color>(newEntity, newColor);
        manager.AddComponent<Gravity>(newEntity, grav);
        initMov.vel = glm::vec2{ initVel(gen), initVel(gen) };

        manager.AddComponent<MovementState>(newEntity, initMov);
        manager.AddComponent<Transform2D>(newEntity, initTForm);

        allEntities.push_back(newEntity);
    }

    // create line at the Edges of the screen
    eID bottomLine = manager.CreateEntity();
    eID topLine = manager.CreateEntity();
    eID leftLine = manager.CreateEntity();
    eID rightLine = manager.CreateEntity();

    Transform2D defaultTform;
    defaultTform.pos = {}; defaultTform.R = {};
    manager.AddComponent<Transform2D>(bottomLine, defaultTform);
    manager.AddComponent<Transform2D>(topLine, defaultTform);
    manager.AddComponent<Transform2D>(leftLine, defaultTform);
    manager.AddComponent<Transform2D>(rightLine, defaultTform);

    Line topLineComp, botLineComp, leftLineComp, rightLineComp;
    botLineComp.p1 = { 0.0f, screenHeight };
    botLineComp.p2 = {screenWidth, screenHeight};

    topLineComp.p1 = { 0.0f, 0.0f };
    topLineComp.p2 = { screenWidth, 0.0f };

    leftLineComp.p1 = { 0.0f, 0.0f };
    leftLineComp.p2 = { 0.0f, screenHeight };

    rightLineComp.p1 = { screenWidth, 0.0f };
    rightLineComp.p2 = { screenWidth, screenHeight };

    manager.AddComponent<Line>(bottomLine, botLineComp);
    manager.AddComponent<Line>(topLine, topLineComp);
    manager.AddComponent<Line>(leftLine, rightLineComp);
    manager.AddComponent<Line>(rightLine, leftLineComp);

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
                break;
            }

            if (event.key.code == sf::Keyboard::Space)
            {
                // add a New Circle
                auto newEntity = manager.CreateEntity();
                Circle newCircle;
                Transform2D newTForm;
                Color newColor;
                MovementState newMov;

                newCircle.radius = 10;
                newColor.r = randColor(gen);
                newColor.g = randColor(gen);
                newColor.b = randColor(gen);
                newTForm.pos = { screenWidth / 2, screenHeight / 2 };
                newMov.vel = {};

                manager.AddComponent(newEntity, newCircle);
                manager.AddComponent(newEntity, newColor);
                manager.AddComponent(newEntity, newTForm);
                manager.AddComponent(newEntity, grav);
                manager.AddComponent(newEntity, newMov);

                allEntities.push_back(newEntity);
                break;
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
