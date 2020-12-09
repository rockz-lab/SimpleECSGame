#include <iostream>

#include "Components/gameComponents.h"
#include "Systems/physicsSystem.h"
#include "Systems/renderSystem.h"

#include <chrono>
#include <thread>
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
    manager.RegisterComponent<DrawableCircle>();
    manager.RegisterComponent<Color>();

    //manager.RegisterComponent<TestBuffer<1000>>();

    auto physicssystem = manager.RegisterSystem<PhysicsSystem>();
    auto rendersystem = manager.RegisterSystem<RenderSystem>();

    manager.SetSystemSignature<PhysicsSystem, Gravity, Transform2D, MovementState>();
    manager.SetSystemSignature<RenderSystem, Transform2D, DrawableCircle, Color>();
     
    // create some Entites
    std::vector<eID> allEntities;

    auto redCircle = manager.CreateEntity();
    auto greenCircle = manager.CreateEntity();
    auto blueCircle = manager.CreateEntity();

    allEntities.push_back(redCircle);
    allEntities.push_back(greenCircle);
    allEntities.push_back(blueCircle);

    Color red(  { 255, 0, 0 });
    Color green({ 0, 255, 0 });
    Color blue( { 0, 0, 255 });

    Gravity grav({ 9.81f });

    DrawableCircle circle{ 10.0 };

    manager.AddComponent(redCircle, red);
    manager.AddComponent(blueCircle, blue);
    manager.AddComponent(greenCircle, green);

    manager.AddComponent(redCircle,
        Transform2D{
            glm::vec2(100, 100),
            makeRot(0)
        });
    manager.AddComponent(blueCircle,
        Transform2D{
            glm::vec2(110, 150),
            makeRot(0)
        });
    manager.AddComponent(greenCircle,
        Transform2D{
            glm::vec2(300, 200),
            makeRot(0)
        });

    int it = -10;
    for (auto const& e : allEntities)
    {
        manager.AddComponent(e, grav);
        manager.AddComponent(e, circle);
        manager.AddComponent(e, MovementState{glm::vec2(it, 0.0f), glm::vec2(0.0f)});
        it += 10;
    }

    using namespace std::chrono;
    using namespace std::chrono_literals;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1920, 1080), "CausalGame", sf::Style::Fullscreen, settings);
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
        }

        auto t1 = system_clock::now(); 
        double dT = static_cast<double>(duration_cast<microseconds>(t1 - t0).count()) * 1e-6;
        t0 = t1;

        physicssystem->Update(dT);
        rendersystem->Update(dT);

        window->display();

    }
   
}