#include "physics.hpp"
#include "math.h"

Physics::Physics(sf::RectangleShape &f_jumper, Solids &f_solids) {
	jumper = &f_jumper;
	solids = &f_solids;
	falling = true;
}

void Physics::tick() {
	float dt = clock.getElapsedTime().asSeconds() - lastTime.asSeconds();
	dt = dt * speed;

	if (isRightPressed) {
		jVel.x = 100;
	}
	if (isLeftPressed) {
		jVel.x = -100;
		if (isRightPressed && !falling) {
			jVel.x = 0;
		}
	}

	sf::Vector2f ppos = jumper->getPosition();
	sf::Vector2f jpos = ppos;
	jVel.y += G * dt;
	jpos.x += jVel.x * dt;
	jpos.y += jVel.y * dt;
	if (!falling && !(isLeftPressed || isRightPressed)) {
		jVel.x *= 0.9;
	}
	std::vector<sf::FloatRect> intra = solids->intersects(sf::FloatRect(jpos.x, jpos.y, jumper->getSize().x, jumper->getSize().y));
	if (intra.size() != 0) {
		for (int i = 0; i < intra.size(); i++) {
			sf::Vector2f overlap = getOverlap(sf::FloatRect(jpos.x, jpos.y, jumper->getSize().x, jumper->getSize().y), intra[i]);
			if (overlap.y < 0) {
				falling = false;
			}
			if(abs(overlap.y) > abs(overlap.x)) {
				jVel.x = 0.0;
			}
			if(abs(overlap.y) < abs(overlap.x) && jVel.y > 0) {
				jpos.y += overlap.y;
				jVel.y = 0.0;
			}
			else
				jpos.x += overlap.x;
		}
	}
	jumper->setPosition(jpos);

	lastTime = clock.getElapsedTime();
}

void Physics::newTick() {
	lastTime = clock.getElapsedTime();
}

void Physics::resetMotion() {
	jVel.x = 0.0;
	jVel.y = 0.0;
	isRightPressed = false;
	isLeftPressed = false;
}


void Physics::jump() {
	if (jVel.y < 1 && jVel.y > -1) { // if basicly not moving verticaly
		falling = true;
		jVel.y = -230;
	}
}

void Physics::leftPress() {
	if (!isLeftPressed && !falling)
		leftTime = clock.getElapsedTime();
	isLeftPressed = true;
}

void Physics::leftRelease() {
	isLeftPressed = false;
}

void Physics::rightPress() {
	if (!isRightPressed && !falling)
		rightTime = clock.getElapsedTime();
	isRightPressed = true;
}

void Physics::rightRelease() {
	isRightPressed = false;
}

float Physics::parabolicForce(float extent, float magnitude, float vel) {
	vel = abs(vel);
	if (vel < 0 || vel > extent) return 0;
	return ((4*magnitude*vel) / extent) - ((4*magnitude*vel*vel) / (extent*extent));
}

sf::Vector2f Physics::getOverlap(sf::FloatRect cur, sf::FloatRect rec) {
	sf::Vector2f offset;
	if (cur.left + cur.width > rec.left &&
		cur.left < rec.left) // if CUR overlaps REC to the left of REC
	{
		offset.x = rec.left - (cur.left + cur.width); // (left edge of CUR) - (right edge of REC) = negitive overlap
	}
	else if (cur.left < rec.left + rec.width &&
			 cur.left > rec.left) // if CUR overlaps REC to the right of REC
	{
		offset.x = (rec.left + rec.width) - cur.left; // (right edge of REC) - (left edge of CUR) = positive overlap
	}
	else offset.x = cur.width; // total size of CUR along y
// ______________________________________________________________________

	if (cur.top + cur.height > rec.top &&
		cur.top < rec.top) // if CUR overlaps REC above REC
	{
		offset.y = rec.top - (cur.top + cur.height); // (top edge of CUR) - (right edge of REC) = negitive overlap
	}
	else if (cur.top < rec.top + rec.height &&
			 cur.top > rec.top) // if CUR overlaps REC below REC
	{
		offset.y = (rec.top + rec.height) - cur.top; // (right edge of REC) - (top edge of CUR) = positive overlap
	}
	else offset.y = cur.height; // total size of CUR along y

	return offset;
}