#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>



namespace Generate {
	int generateImage(std::string comp, std::string sourcp, sf::Image* imag);
	void doView(sf::Font*);
	void abort();
}

struct preciseMeasure {
	sf::Color c1, c2, c3, c4; //maybe in a future update do multipoint? hmm
};