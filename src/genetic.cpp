#include "genetic.hpp"
#include "world.hpp"
#include "jumper.hpp"
#include "physics.hpp"

void Jumper_GeneticAlgorithm::exec()
{
	sf::Clock clock;
	sf::Time tickTimer = clock.getElapsedTime();
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
	brush.setPosition(600 * (2.0/16.0), 600 * (2.0/8.0));
	brush.setSize(sf::Vector2f(600 * (1.0/16.0), 600.0 * (1.0/16.0)));
	solids.addRec(brush);

	Jumper jumper(
		sf::Vector2f(600 * (1.0 / 8.0), 600 * (3.0 / 8.0)),
		sf::Vector2f(600 * (1.0 / 16.0), 600 * (1.0 / 18.0))
	);

	Physics physics(jumper, solids);

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
					physics.left();
				}
				if (event.key.code == sf::Keyboard::D) {
					physics.right();
				}
			}
		}


		if (clock.getElapsedTime().asSeconds() - tickTimer.asSeconds() > 0.016)
		{
			tickTimer = clock.getElapsedTime();
			physics.tick();
			if (jumper.rec.getPosition().y > 600) {
				jumper.rec.setPosition(sf::Vector2f(600 * (1.0 / 8.0), 600 * (3.0 / 8.0)));
			}
		}

		window.clear(sf::Color(23, 30, 20));
		solids.draw(window);
		jumper.draw(window);
		window.display();
	}
}