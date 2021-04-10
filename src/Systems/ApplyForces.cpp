#include "ApplyForces.h"

void ApplyForces::Init(std::shared_ptr<sf::RenderWindow> window)
{
	win = window;
}

void ApplyForces::Update(float dT)
{
	bool startedDrag = false;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for (auto& entity : m_entitiyLists[0])
		{

			auto& state = manager.GetComponent<RigidBodyState>(entity);
			glm::vec2 mousePos{};

			auto& transform = manager.GetComponent<Transform2D>(entity);
			if (!isDragging)
			{
				mousePos = { sf::Mouse::getPosition().x, sf::Mouse::getPosition().y };

				// check, if our triangle containes the current mouse positions
				const auto& poly = manager.GetComponent<Triangle>(entity);

				auto poly_world = poly.vertexData;

				for (auto& v : poly_world.vertices)
					v = vec2(glm::rotate(v.to_glm(), transform.rotation)) + transform.pos;
				for (auto& n : poly_world.normals)
					n = vec2(glm::rotate(n.to_glm(), transform.rotation));

				if (pointInPoly(mousePos, poly_world))
				{
					// get the local position on the triangle by undoing the transform
					grabPointLocal = glm::rotate((mousePos - transform.pos).to_glm(), -transform.rotation);
					isDragging = true;
					startedDrag = true;
					selectedEntity = entity; 
				}
			}
		}

		if (isDragging)
		{
			auto& state = manager.GetComponent<RigidBodyState>(selectedEntity);
			auto& transform = manager.GetComponent<Transform2D>(selectedEntity);

			// apply forces and torques in a rubberband style
			glm::vec2 mousePos = { sf::Mouse::getPosition().x, sf::Mouse::getPosition().y };


			glm::vec2 grabPointWorld = glm::rotate(grabPointLocal, transform.rotation) + transform.pos.to_glm();
			glm::vec2 elongation = (mousePos - grabPointWorld);

			if (startedDrag)
				elongation_o = elongation;
			
			glm::vec2 force;
			if (glm::length(elongation) > base_elongation)
			{
				glm::vec2 velocity = (elongation - elongation_o) / dT;
				glm::vec2 effective_elongation = elongation - glm::normalize(elongation) * base_elongation;
				force = effective_elongation * strength + d * velocity;
				if (glm::dot(force, velocity) < 0)
					force = {};
			} else
				force = {};

			glm::vec2 centerPos = transform.pos.to_glm();
			glm::vec2 lever = grabPointWorld - centerPos;
			float torque = glm::dot({ -lever.y, lever.x }, force);

			state.force += force;	// assumes the forces are cleared every iteration
			state.torque += torque;

			elongation_o = elongation;

			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(mousePos.x, mousePos.y)),
				sf::Vertex(sf::Vector2f(grabPointWorld.x, grabPointWorld.y))
			};

			win->draw(line, 2, sf::Lines);
		}
	} else
	{
		isDragging = false;
	}
}
