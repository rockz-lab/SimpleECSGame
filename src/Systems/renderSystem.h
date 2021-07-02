#pragma once

//#include <SFML/Graphics.hpp>

#include "ecs/ecs.h"
#include "Components/gameComponents.h"

#include "Renderer/Renderer2D.h"
#include "Renderer/Window.h"

#include "Scene/Camera.h"

// 2D rendering with SFML
class RenderSystem : public System
{
public:
	void Init(std::shared_ptr<Window> window)
	{
		win = window;
		
		renderer = std::make_unique<Renderer2D>();
		cam = std::make_unique<Camera2D>(1000.0f, 1.0f);
	}

	void Update()
	{

		FrameVertexBuffer vertexBuffer;

		vertexBuffer.Bind();
		for (auto& entity : m_entitiyLists[0])
		{
			vertexBuffer.AddDrawable(entity);
		}

		renderer->Draw(vertexBuffer, cam->transform);

		vertexBuffer.Unbind();
		
	}
	
private:
	std::unique_ptr<Renderer2D> renderer;
	std::shared_ptr<Window> win;
	std::unique_ptr<Camera2D> cam;
};