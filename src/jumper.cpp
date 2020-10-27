#include "jumper.hpp"
#include <fstream>

using namespace std;

int gene::getSteps() {
	return STEPS;
}

bool gene::getCodon(int i) {
	return coding[i];
}

std::string gene::print() {
	for (int i = 0; i < STEPS - 1; i++) {
		std::cout << coding[i] << ",";
	}
	std::cout << coding[STEPS - 1];
	return ""; // return for ease with cout
}

void gene::setRandom() {
	for (int i = 0; i < STEPS; i++) {
		coding[i] = rand()%2;
	}
}

chromosome::chromosome() {
	leftGene.setRandom();
	rightGene.setRandom();
	jumpingGene.setRandom();
}

void chromosome::print() {
	std::cout << "left gene    : [" << leftGene.print() << "]" << std::endl;
	std::cout << "right gene   : [" << rightGene.print() << "]" << std::endl;
	std::cout << "jumping gene : [" << jumpingGene.print() << "]" << std::endl;
}

Jumper::Jumper(Physics& phys) {
	physics = &phys;
	currentStep = 0;
}

void Jumper::tick() {
	if (chromo.leftGene.getCodon(currentStep)) physics->leftPress(); else physics->leftRelease();
	if (chromo.rightGene.getCodon(currentStep)) physics->rightPress(); else physics->rightRelease();
	if (chromo.jumpingGene.getCodon(currentStep)) physics->jump();
	currentStep = (currentStep + 1) % STEPS;
}

void Jumper::reset() {
	currentStep = 0;
}

Population::Population(Physics& phys) {
	physics = &phys;
	currentJumper = 0;
}

int Population::size() {
	return jumpers.size();
}

void Population::addRandJumper(int size) {
	for (int i = 0; i < size; i++) {
		jumpers.push_back(Jumper(*physics));
	}
}

void Population::tick() {
	jumpers[currentJumper].tick();
}

void Population::setCurrentScore(float score) {
	jumpers[currentJumper].score = score;
}

bool Population::nextJumper() {
	if (currentJumper < jumpers.size() - 1) {
		currentJumper++;
		return true;
	}
	return false;
}

/* Sets currentJumper index var to zero */
void Population::resetCurrent() {
	currentJumper = 0;
}

void Population::save(string filename) {
	ofstream file(filename);
	file << "PS" << endl;
	file <<	jumpers.size() << endl;
	file << "GS" << endl;
	file << STEPS << endl;

	for (int i = 0; i < jumpers.size(); i++) {
		// Can only save jumpers with scores better than or equal to 0
		if (jumpers[i].score >= 0) {
			file << "J" << endl;
			for (int j = 0; j < STEPS - 1; j++) {
				file << jumpers[i].chromo.leftGene.getCodon(j) << ",";
			}
			file << jumpers[i].chromo.leftGene.getCodon(STEPS - 1) << endl;
			for (int j = 0; j < STEPS - 1; j++) {
				file << jumpers[i].chromo.rightGene.getCodon(j) << ",";
			}
			file << jumpers[i].chromo.rightGene.getCodon(STEPS - 1) << endl;

			for (int j = 0; j < STEPS - 1; j++) {
				file << jumpers[i].chromo.jumpingGene.getCodon(j) << ",";
			}
			file << jumpers[i].chromo.jumpingGene.getCodon(STEPS - 1) << endl;
			file << "S" << endl;
			file << jumpers[i].score << endl;
			file << "J-END" << endl;
		}
	}
	file << "END";
	file.close();
}

