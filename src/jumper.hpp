#pragma once

class Jumper {
	sf::RectangleShape rec;
public:
	Jumper(sf::Vector2f pos, sf::Vector2f size);
	void draw(sf::RenderWindow &window);
};