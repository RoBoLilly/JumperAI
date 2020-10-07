#include "physics.hpp"
#include "math.h"

Physics::Physics(Jumper &f_jumper, Solids &f_solids) {
	jumper = &f_jumper;
	solids = &f_solids;
	falling = true;
}

void Physics::tick() {
	float dt = clock.getElapsedTime().asSeconds() - lastTime.asSeconds();

	if (isRightPressed) {
		jVel.x = jVel.x + parabolicForce(0.1, 20, clock.getElapsedTime().asSeconds() - rightTime.asSeconds());
	}
	if (isLeftPressed) {
		jVel.x = jVel.x - parabolicForce(0.1, 20, clock.getElapsedTime().asSeconds() - leftTime.asSeconds());
	}

	sf::Vector2f ppos = jumper->rec.getPosition();
	sf::Vector2f jpos = ppos;
		jVel.y += G;
		jpos.x += jVel.x * dt;
		jpos.y += jVel.y * dt;
		if (!falling && !(isLeftPressed || isRightPressed)) {
			jVel.x *= 0.95;
		}
		std::vector<sf::FloatRect> intra = solids->intersects(sf::FloatRect(jpos.x, jpos.y, jumper->rec.getSize().x, jumper->rec.getSize().y));
		if (intra.size() != 0) {
			for (int i = 0; i < intra.size(); i++) {
				sf::Vector2f overlap = getOverlap(sf::FloatRect(jpos.x, jpos.y, jumper->rec.getSize().x, jumper->rec.getSize().y), intra[i]);
				if (overlap.y < 0) {
					falling = false;
				}
				if(abs(overlap.y) > abs(overlap.x)) {
					jVel.x = 0.0;
				}
				if(abs(overlap.y) < abs(overlap.x)) {
					jpos.y += overlap.y;
					jVel.y = 0.0;
				}
				else
					jpos.x += overlap.x;
			}
		}
		jumper->rec.setPosition(jpos);

	lastTime = clock.getElapsedTime();
}

void Physics::jump() {
	if (jVel.y < 1 && jVel.y > -1) { // if basicly not moving verticaly
		falling = true;
		jVel.y = -230;
	}
}

void Physics::leftPress() {
	if (!isLeftPressed)
		leftTime = clock.getElapsedTime();
	isLeftPressed = true;
}

void Physics::leftRelease() {
	isLeftPressed = false;
}

void Physics::rightPress() {
	if (!isRightPressed)
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