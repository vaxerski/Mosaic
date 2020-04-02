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
	exit_thread,
};

struct cloneinfo {
	int count;
	sf::Color color;
	int leader;
	float posX, posY;
};

struct mfColor {
	float r, g, b;
	int alpha = 255;

	mfColor(sf::Color c) {
		r = c.r;
		g = c.g;
		b = c.b;
		alpha = c.a;
	}

	mfColor(int ra, int ga, int ba, float a = 255) {
		r = ra;
		g = ga;
		b = ba;
		alpha = a;
	}

	operator sf::Color() const {
		return sf::Color(r, g, b, alpha);
	}
};
