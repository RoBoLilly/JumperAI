#include "genetic.hpp"
#include "world.hpp"
#include "physics.hpp"
#include "jumper.hpp"
#include <math.h>
#include <vector>
#include <string>

struct mostRight {
	int x;
	void reset();

};

void Jumper_GeneticAlgorithm::exec()
{
	sf::Clock clock;
	sf::Time tickTimer = clock.getElapsedTime();
	sf::Time gameTimer = clock.getElapsedTime();
	sf::Time launchTimer = clock.getElapsedTime();
	sf::Time runnerTimer = clock.getElapsedTime();
	sf::Vector2i windowSize = sf::Vector2i(600, 600);
	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Jumper - Genetic Algorithm");

	Solids solids;
	sf::RectangleShape brush;
	brush.setFillColor(sf::Color(215, 215, 210));
	brush.setPosition(0, 600 * (1.0/2.0));
	brush.setSize(sf::Vector2f(600 * (1.0/4.0), 600.0 * (5.0/8.0)));
	solids.addRec(brush);
	brush.setPosition(600 * (3.0/4.0), 600 * (1.0/2.0));
	brush.setSize(sf::Vector2f(600 * (1.0/4.0), 600.0 * (5.0/8.0)));
	solids.addRec(brush);
	brush.setPosition(600 * (7.0/16.0), 600 * (5.0/8.0));
	brush.setSize(sf::Vector2f(600 * (1.0/8.0), 600.0 * (3.0/8.0)));
	solids.addRec(brush);
	brush.setPosition(600 * (10.0/16.0), 600 * (9.0/16.0));
	brush.setSize(sf::Vector2f(600 * (1.0/16.0), 600.0 * (7.0/16.0)));
	solids.addRec(brush);
	brush.setPosition(-100, 0);
	brush.setSize(sf::Vector2f(100, 600));
	solids.addRec(brush);
	brush.setPosition(600, 0);
	solids.addRec(brush);

	sf::RectangleShape jumperRec;
	jumperRec.setPosition(600 * (1.0 / 8.0), 600 * (3.0 / 8.0));
	jumperRec.setSize(sf::Vector2f(600 * (1.0 / 16.0), 600 * (1.0 / 18.0)));
	jumperRec.setFillColor(sf::Color::Blue);

	Physics physics(jumperRec, solids);

	Population pop(physics);
	//pop.addRandJumper(10);
	pop.fill(pop.load("Saves/SlowGen/inPop.pop"));
	bool genEnded = false;
	int genNum = 1;
	int mx = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) window.close();
				// if (event.key.code == sf::Keyboard::Space) {
				// 	physics.jump();
				// }
				// if (event.key.code == sf::Keyboard::A) {
				// 	physics.leftPress();
				// }
				// if (event.key.code == sf::Keyboard::D) {
				// 	physics.rightPress();
				// }
			}
			// if (event.type == sf::Event::KeyReleased)
			// {
			// 	if (event.key.code == sf::Keyboard::A) {
			// 		physics.leftRelease();
			// 	}
			// 	if (event.key.code == sf::Keyboard::D) {
			// 		physics.rightRelease();
			// 	}
			// }
		}

		if (clock.getElapsedTime().asSeconds() - launchTimer.asSeconds() < 0.3 || clock.getElapsedTime().asSeconds() < 2) {
			genEnded = true;
			runnerTimer = clock.getElapsedTime();
			gameTimer = clock.getElapsedTime();
			physics.newTick();
		} else {
			genEnded = false;
		}

		if (clock.getElapsedTime().asSeconds() - runnerTimer.asSeconds() > 0.25 / physics.speed && !genEnded)
		{
			runnerTimer = clock.getElapsedTime();
			pop.tick();
		}
		if (clock.getElapsedTime().asSeconds() - tickTimer.asSeconds() > 0.016 / physics.speed && !genEnded)
		{
			tickTimer = clock.getElapsedTime();
			physics.tick();
			float runTime = (clock.getElapsedTime().asSeconds() - gameTimer.asSeconds());
			if (mx < jumperRec.getPosition().x) mx = jumperRec.getPosition().x;
			float score = jumperRec.getPosition().x - (runTime * 10 * physics.speed) + 20;

			if (jumperRec.getPosition().y > 600 || jumperRec.getPosition().x > 560 || score < 0) {
				score = score + mx;
				if (jumperRec.getPosition().x > 560) { // Checks if in goal area
					score *= 2;
					std::cout << pop.currentID() << " -> Score: " << score << "   GOAL!" << std::endl;
				} else {
					std::cout << pop.currentID() << " -> Score: " << score << std::endl;
				}
				pop.setCurrentScore(score);
				jumperRec.setPosition(600 * (1.0 / 8.0), 600 * (3.0 / 8.0)); // takes rec out of catch region
				physics.resetMotion();

				if (!pop.nextJumper()) {
					std::cout << "Generation mutation rate: 1 / " << pop.mutationRate << std::endl;
					std::cout << "Average Generation Score: " << pop.averageScore() << std::endl;
					std::cout << "Saving Generation (" << genNum << ")" << std::endl;
					pop.save("Saves/SlowGen/gen" + std::to_string(genNum) + ".pop");
					//pop.load("Saves/SlowGen/gen" + std::to_string(genNum) + ".pop");
					genNum++;
					pop.resetCurrent();
					pop.generation();
				}
				pop.resetRun();
				launchTimer = clock.getElapsedTime();
				mx = 0;
			}
		}

		window.clear(sf::Color(23, 30, 20));
		solids.draw(window);
		window.draw(jumperRec);
		window.display();
	}
}