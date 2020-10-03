#include "jumper.hpp"

Jumper::Jumper(sf::Vector2f pos, sf::Vector2f size){
	rec.setPosition(pos);
	rec.setSize(size);
	rec.setOrigin(sf::Vector2f(rec.getSize().x / 2, 0));
}

void Jumper::draw(sf::RenderWindow &window) {
	window.draw(rec);
}