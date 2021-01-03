#pragma once

#include "ecs/ecs.h"
#include "Components/gameComponents.h"

extern ECSManager manager;

#include <SFML/Graphics.hpp>
#include <glm/gtx/rotate_vector.hpp>

class ApplyForces : public System
{
public:
	void Init(std::shared_ptr<sf::RenderWindow> window);

	void Update(float dT);
private:
	float strength = 50.0f;
	float d = 2.0f;
	eID selectedEntity{};


	bool isDragging = false;
	glm::vec2 grabPointLocal{};			// point on the shape, that is dragged
	glm::vec2 elongation_o{};

	std::shared_ptr<sf::RenderWindow> win;
};