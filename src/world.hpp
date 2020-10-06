#pragma once
#include <vector>

struct node {
	sf::RectangleShape rec;
	node* next;
};

class Solids {
private:
	node* head;
public:
	Solids();
	~Solids();
	void addRec(sf::RectangleShape rec);
	void draw(sf::RenderWindow& window);
	std::vector<sf::FloatRect> intersects(sf::FloatRect rec);
	//int side(sf::FloatRect jumper, sf::FloatRect rec);
};