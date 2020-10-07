#include "jumper.hpp"

Jumper::Jumper(sf::Vector2f pos, sf::Vector2f size){
	rec.setPosition(pos);
	rec.setSize(size);
	//rec.setOrigin(sf::Vector2f(rec.getSize().x / 2, rec.getSize().y));
	rec.setFillColor(sf::Color::Blue);
	bodyBox.setFillColor(sf::Color::Green);
}

void Jumper::draw(sf::RenderWindow &window) {
	window.draw(rec);
}

// sf::RectangleShape Jumper::getHibox(sf::Vector2f cutin) {
// 	bodyBox.setPosition(rec.getPosition().x + cutin.x - rec.getSize().x / 2, rec.getPosition().y + cutin.y - rec.getSize().y);
// 	bodyBox.setSize(sf::Vector2f(rec.getSize().x - 2 * cutin.x, rec.getSize().y - 2 * cutin.y));
// 	return bodyBox;
// }
