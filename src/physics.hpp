#pragma once
#include "world.hpp"

class Physics {
	float G = 600.0f;
	sf::RectangleShape* jumper;
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
	float speed = 2.0;
	Physics(sf::RectangleShape &f_jumper, Solids &f_solids);
	void newTick();
	void tick();
	void jump();
	void resetMotion();
	void leftPress();
	void leftRelease();
	void rightPress();
	void rightRelease();
};