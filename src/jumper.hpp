#pragma once
#include "physics.hpp"

#define STEPS 16
class gene {
	bool coding[STEPS];
public:
	int getSteps();
	bool getCodon(int i);
	std::string print();
	void setRandom();
};

class chromosome {
public:
	gene leftGene;
	gene rightGene;
	gene jumpingGene;
	chromosome();
	void print();
};

class Jumper {
	Physics* physics;
	int currentStep;
public:
	chromosome chromo;
	float score;
	Jumper(Physics& phys);
	void tick();
	void reset();
	void printChromosome();
};

class Population {
	std::vector<Jumper> jumpers;
	Physics* physics;
public:
	int currentJumper;
	Population(Physics& phys);
	void tick();
	int size();
	void setCurrentScore(float score);
	bool nextJumper();
	void resetCurrent();
	void save(std::string filename);
	void addRandJumper(int size);
	void load(std::string filename);
	void clear();
};