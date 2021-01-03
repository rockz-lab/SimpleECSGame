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
		for (auto& entity : m_entitiyLists[0])
		{
			auto& transform = manager.GetComponent<Transform2D>(entity);
			auto& color = manager.GetComponent<Color>(entity);
			//auto& circleComp = manager.GetComponent<Circle>(entity);

			// For testing purposes, we just create the vertex buffer on the fly
			//sf::CircleShape circle(circleComp.radius);

			if (manager.CheckCompType<Polygon>(entity))
			{
				auto& triData = manager.GetComponent<Polygon>(entity).poly;

				auto triangleColor = sf::Color(color.r, color.g, color.b);
				sf::VertexArray vertArr(sf::Triangles, 3);

				vertArr[0].position = {triData.vertices[0].x, triData.vertices[0].y};
				vertArr[1].position = {triData.vertices[1].x, triData.vertices[1].y};
				vertArr[2].position = {triData.vertices[2].x, triData.vertices[2].y};

				vertArr[0].color = triangleColor;
				vertArr[1].color = triangleColor;
				vertArr[2].color = triangleColor;

				sf::Transform transformSFML;
				transformSFML.translate({ transform.pos.x, transform.pos.y });
				transformSFML.rotate(transform.rotation * 180 / glm::pi<float>());

				win->draw(vertArr, transformSFML);
			}

		}

		for (auto& entity : m_entitiyLists[1])
		{
			auto& line = manager.GetComponent<Line>(entity);

			sf::VertexArray vertArr(sf::Lines, 2);
			vertArr[0].position = { line.p1.x, line.p1.y };
			vertArr[1].position = { line.p2.x, line.p2.y };

			win->draw(vertArr);
		}
	}
	
private:
	
	std::shared_ptr<sf::RenderWindow> win;
};