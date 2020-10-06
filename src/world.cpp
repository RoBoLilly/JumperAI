#include "world.hpp"

Solids::Solids() {
	head = nullptr;
}

Solids::~Solids() {
	if (head != nullptr) { // if list1 is empty just move on
        node* prev = head;
        node* curr = head->next;
        while (curr != nullptr) {
            delete prev;
            prev = curr;
            curr = curr->next;
        }
        delete prev;
    }
}

void Solids::addRec(sf::RectangleShape rec) {
	if (head == nullptr) {
		node* temp = new node;
		temp->rec = rec;
		temp->next = nullptr;
		head = temp;
		return;
	}
	node* initHead = head;
	head = new node();
	head->rec = rec;
	head->next = initHead;
}

void Solids::draw(sf::RenderWindow& window) {
	for (node* i = head; i != nullptr; i = i->next) {
		window.draw(i->rec);
	}
}

std::vector<sf::FloatRect> Solids::intersects(sf::FloatRect rec) {
	std::vector<sf::FloatRect> intersections;
	for (node* i = head; i != nullptr; i = i->next) {
		if (rec.intersects(i->rec.getGlobalBounds()))
			intersections.push_back(i->rec.getGlobalBounds());
	}
	return intersections;
}



// int Solids::side(sf::FloatRect jumper, sf::FloatRect rec) {
// 	if (jumper.left > (rec.left + rec.width)) return 0; // left
// 	if (jumper.top > (rec.top + rec.height)) return 1; // down
// 	if ((jumper.left + jumper.width) < rec.left) return 2; // right
// 	if ((jumper.top + jumper.height) < rec.top) return 3; // up
// 	return -1;
// }