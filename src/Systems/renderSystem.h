#pragma once

//#include <SFML/Graphics.hpp>

#include "ecs/ecs.h"
#include "Components/gameComponents.h"

#include "Renderer/Renderer2D.h"
#include "Renderer/Window.h"

#include "Scene/Camera.h"

extern ECSManager manager;
// 2D rendering with SFML
class RenderSystem : public System
{
public:
	void Init(std::shared_ptr<Window> window)
	{
		win = window;
		
		renderer = std::make_unique<Renderer2D>();
		cam = std::make_unique<Camera2D>(1000, 1);
	}

	template <typename T>
	void Update(T dT)
	{

		FrameVertexBuffer vertexBuffer;

		vertexBuffer.Bind();
		for (auto& entity : m_entitiyLists[0])
		{
			vertexBuffer.AddDrawable(entity);
		}

		renderer->Draw(vertexBuffer, cam->transform);

		vertexBuffer.Unbind();
		/*for (auto& entity : m_entitiyLists[1])
		{
			auto& line = manager.GetComponent<Line>(entity);

			sf::VertexArray vertArr(sf::Lines, 2);
			vertArr[0].position = { line.p1.x, line.p1.y };
			vertArr[1].position = { line.p2.x, line.p2.y };

			win->draw(vertArr);
		}*/
	}
	
private:
	std::unique_ptr<Renderer2D> renderer;
	std::shared_ptr<Window> win;
	std::unique_ptr<Camera2D> cam;
};