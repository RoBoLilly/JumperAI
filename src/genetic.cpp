#include "genetic.hpp"
#include "world.hpp"
#include "physics.hpp"
#include "jumper.hpp"
#include <math.h>
#include <vector>

void Jumper_GeneticAlgorithm::exec()
{
	sf::Clock clock;
	sf::Time tickTimer = clock.getElapsedTime();
	sf::Time gameTimer = clock.getElapsedTime();
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
	pop.addRandJumper(10);

	bool genEnded = false;

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
				if (event.key.code == sf::Keyboard::Space) {
					physics.jump();
				}
				if (event.key.code == sf::Keyboard::A) {
					physics.leftPress();
				}
				if (event.key.code == sf::Keyboard::D) {
					physics.rightPress();
				}
			}
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::A) {
					physics.leftRelease();
				}
				if (event.key.code == sf::Keyboard::D) {
					physics.rightRelease();
				}
			}
		}


		if (clock.getElapsedTime().asSeconds() - runnerTimer.asSeconds() > 0.45 && !genEnded)
		{
			runnerTimer = clock.getElapsedTime();
			pop.tick();
		}
		if (clock.getElapsedTime().asSeconds() - tickTimer.asSeconds() > 0.016 && !genEnded)
		{
			tickTimer = clock.getElapsedTime();
			physics.tick();
			float gameTime = (clock.getElapsedTime().asSeconds() - gameTimer.asSeconds());
			float score = jumperRec.getPosition().x - (gameTime * 10) + 20;

			if (jumperRec.getPosition().y > 600 || jumperRec.getPosition().x > 560 || score < 0) {
				gameTimer = clock.getElapsedTime();
				runnerTimer = clock.getElapsedTime();
				if (jumperRec.getPosition().x > 560) { // Checks if in goal area
					score *= 1.2;
					std::cout << pop.currentJumper << " -> Score: " << score << "   GOAL!" << std::endl;
				} else {
					std::cout << pop.currentJumper << " -> Score: " << score << std::endl;
				}
				pop.setCurrentScore(score);
				jumperRec.setPosition(sf::Vector2f(600 * (1.0 / 8.0), 600 * (3.0 / 8.0))); // takes rec out of catch region
				if (!pop.nextJumper()) {
					genEnded = true;
					std::cout << "Saving Generation..." << std::endl;
					pop.save("testPop.pop");
					window.close();
				}
			}
		}

		window.clear(sf::Color(23, 30, 20));
		solids.draw(window);
		window.draw(jumperRec);
		window.display();
	}
}