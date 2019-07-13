#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>



namespace Generate {
	int generateImage(std::string comp, std::string sourcp, sf::Image* imag);
	void doView(sf::Font*);
	void abort();
}

