#pragma once

class Jumper {
public:
	sf::RectangleShape rec;
	sf::RectangleShape bodyBox;
	Jumper(sf::Vector2f pos, sf::Vector2f size);
	//sf::RectangleShape getHibox(sf::Vector2f cutin);
	void draw(sf::RenderWindow &window);
};