#pragma once
#include "jumper.hpp"
#include "world.hpp"

class Physics {
	float G = 9.8f;
	Jumper* jumper;
	bool falling;
	sf::Vector2f jVel;

	Solids* solids;
	sf::Clock clock;
	sf::Time lastTime;
	bool isRightPressed = false;
	bool isLeftPressed = false;
	sf::Time rightTime;
	sf::Time leftTime;
	sf::Vector2f getOverlap(sf::FloatRect target, sf::FloatRect rec);
	float parabolicForce(float extent, float magnitude, float vel);
public:
	Physics(Jumper &f_jumper, Solids &f_solids);
	void tick();
	void jump();
	void leftPress();
	void leftRelease();
	void rightPress();
	void rightRelease();
};