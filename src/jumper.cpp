#include "jumper.hpp"
#include <fstream>
#include <string>

using namespace std;

int gene::getSteps() {
	return STEPS;
}

bool gene::getCodon(int i) {
	return coding[i];
}

void gene::setCodon(int i, bool v) {
	if (i > -1 && i < STEPS) {
		coding[i] = v;
	}
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

void chromosome::crossover(chromosome &partner) {
	bool flop = true;
	crossed = true;
	for (int i = 0; i < STEPS; i++) {
		if (i % 6) flop = !flop;
		leftGene.setCodon(i, (flop)? leftGene.getCodon(i) : partner.leftGene.getCodon(i));
		leftGene.setCodon(i, (!flop)? leftGene.getCodon(i) : partner.leftGene.getCodon(i));
		partner.rightGene.setCodon(i, (flop)? rightGene.getCodon(i) : partner.rightGene.getCodon(i));
		partner.rightGene.setCodon(i, (!flop)? rightGene.getCodon(i) : partner.rightGene.getCodon(i));
	}
}

int chromosome::getID() {
	int sum = 0;
	for (int i = 0; i < STEPS; i++) {
		sum += leftGene.getCodon(i) * pow(1.1, i);
		// sum += rightGene.getCodon(i) * pow(2, i) * 2;
		// sum += jumpingGene.getCodon(i) * pow(2, i) * 3;
	}
	return sum;
}

/* one in a <chance> probability to change a codon [rand() % <chance> == 0] */
void mutate(gene &g, int chance) {
	for (int i = 0; i < STEPS; i++) {
		if ((rand()%chance) == 0) g.setCodon(i, !g.getCodon(i));
	}
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

/* Sets currentJumper index var to zero and resets jumpers for next gen */
void Population::resetCurrent() {
	for (int i = 0; i < jumpers.size(); i++) {
		jumpers[i].chromo.crossed = false;
	}
	currentJumper = 0;
	//mutationRate *= 1.002;
}

void Population::resetRun() {
	//physics->velZero( );
	for (int i = 0; i < jumpers.size(); i++) {
		jumpers[i].reset();
	}
}

void Population::save(string filename) {
	ofstream file(filename);
	if (file.is_open()) {
		file << "PS" << endl;
		file <<	jumpers.size() << endl;
		// file << "GS" << endl;
		// file << STEPS << endl;

		for (int i = 0; i < jumpers.size(); i++) {
			// Can only save jumpers with scores better than or equal to 0
			if (jumpers[i].score >= 0 || true) {
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
				if (jumpers[i].chromo.crossed) file << "CROSSED" << endl;
				file << "J-END" << endl;
			}
		}
		file << "END";
		file.close();
	} else
		cout << "Failed to open " << filename << endl;
}


int Population::load(std::string filename) {
	ifstream file(filename);
	if (file.is_open()) {
		jumpers.clear();
		int popSize = -1;
		int geneSize = -1;
		string line = "START";
		getline(file, line);
		while (line != "END" && file) {
			if (line == "PS") {
				getline(file, line);
				popSize = stoi(line);
			}
			// else if (line == "GS") {
			// 	getline(file, line);
			// 	geneSize = stoi(line);}
			else if (line == "J") {
				Jumper j(*physics);
				getline(file, line);
				for (int i = 0; i < STEPS; i++) {
					j.chromo.leftGene.setCodon(i, (line[i * 2] == '1'));
				}
				getline(file, line);
				for (int i = 0; i < STEPS; i++) {
					j.chromo.rightGene.setCodon(i, (line[i * 2] == '1'));
				}
				getline(file, line);
				for (int i = 0; i < STEPS; i++) {
					j.chromo.jumpingGene.setCodon(i, (line[i * 2] == '1'));
				}
				getline(file, line);
				while (line != "J-END") {
					if (line == "S") {
						getline(file, line);
						j.score = stoi(line);
					}
					getline(file, line);
				}
				jumpers.push_back(j);
			}
			getline(file, line);
		}
		file.close();
		return popSize;
	} else
		cout << "Failed to open " << filename << endl;
}


Jumper Population::getRouletteSelection() {
	float scoreSum = 0;
	for (int i = 0; i < jumpers.size(); i++) {
		scoreSum += jumpers[i].score;
	}
	if (scoreSum > 1) {
		float selSum = 0;
		// cout << "[" << scoreSum << "]: " << sel;
		int sel = rand() % (int)scoreSum;
		for (int i = 0; i < jumpers.size(); i++) {
			selSum += jumpers[i].score;
			if (selSum > sel) {
				cout << " Selected Jumper " << i << endl;
				return jumpers[i];
			}
		}
	} else {
		if (jumpers.size() > 0) return jumpers[rand() % jumpers.size()]; // retrun random jumper
		return Jumper(*physics);
	}
}

void Population::fill(int size) {
	int needed = size - jumpers.size();
	vector<Jumper> list;
	for (int i = 0; i < needed; i++) {
		list.push_back(getRouletteSelection());
	}
	for (int i = 0; i < list.size(); i++) {
		jumpers.push_back(list[i]);
	}
}

void Population::generation() {
	int minScore = averageScore();
	for (int i = 0; i < jumpers.size(); i++) {
		if (jumpers[i].score < minScore) {
			jumpers.erase(jumpers.begin() + i);
		}
	}
	fill(10);
	for (int i = 0; i < jumpers.size(); i++) {
		for (int j = 0; j < jumpers.size(); j++) {
			if ((rand()%15) == 1 && i != j)
				jumpers[i].chromo.crossover(jumpers[j].chromo);
		}
		mutate(jumpers[i].chromo.leftGene, mutationRate);
		mutate(jumpers[i].chromo.rightGene, mutationRate);
		mutate(jumpers[i].chromo.jumpingGene, mutationRate);
	}
}

int Population::averageScore() {
	int sum = 0;
	for (Jumper j : jumpers) {
		sum += j.score;
	}
	return sum / jumpers.size();
}

int Population::currentID() {
	return jumpers[currentJumper].chromo.getID();
}
