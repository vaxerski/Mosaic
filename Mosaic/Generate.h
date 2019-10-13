#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "controls.h"
#include "render.h"
#include "helpers.h"

//defines
typedef std::vector<sf::Sprite> sprite_vector;
typedef std::vector<sf::Texture> texture_vector;
typedef std::vector<sf::Image> image_vector;
typedef std::vector<sf::Color> color_vec;

namespace Generate {
	int generateImage(std::string comp, std::string sourcp, sf::Image* imag);
	void doView(sf::Font*);
	void abort();
	void Thread(sf::Font*);
	void makeRequest(int);
}

struct preciseMeasure {
	sf::Color c1, c2, c3, c4; //maybe in a future update do multipoint? hmm
};

enum requests {
	empty = 0,
	generate_image = 1,
	show_window,
	abort_render,
};

