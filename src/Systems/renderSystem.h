#pragma once

#include <SFML/Graphics.hpp>

#include "ecs/ecs.h"
#include "Components/gameComponents.h"

extern ECSManager manager;
// 2D rendering with SFML
class RenderSystem : public System
{
public:
	void Init(std::shared_ptr<sf::RenderWindow> window)
	{
		win = window;
	}
	template <typename T>
	void Update(T dT)
	{

		for (auto& entity : m_entities)
		{
			auto& transform = manager.GetComponent<Transform2D>(entity);
			auto& color = manager.GetComponent<Color>(entity);
			auto& circleComp = manager.GetComponent<Circle>(entity);

			// For testing purposes, we just create the vertex buffer on the fly
			sf::CircleShape circle(circleComp.radius);
		
			auto centerX = transform.pos.x - circleComp.radius;
			auto centerY = transform.pos.y - circleComp.radius;

			circle.setPosition(sf::Vector2f(centerX, centerY));
			circle.setFillColor(sf::Color(color.r, color.g, color.b));
			win->draw(circle);
		}
	}
	
private:
	std::shared_ptr<sf::RenderWindow> win;
};