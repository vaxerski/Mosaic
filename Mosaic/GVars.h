#pragma once
#include <SFML/Graphics.hpp>
#include <future>
#include "controls.h"
#include "Generate.h"

class GVars {
public:
	int request = 0;
	image_vector images = std::vector<sf::Image>(MAX_INPUT);
	sprite_vector sprites = std::vector<sf::Sprite>(MAX_INPUT);
	texture_vector Textures = std::vector<sf::Texture>(MAX_INPUT);
	color_vec dScan_cols = std::vector<sf::Color>(MAX_SAMPLES);
	int textureScales[MAX_INPUT];
	std::string files[MAX_INPUT]; //max files
	std::string lastfiles[MAX_INPUT]; //max files
	sf::Color avg[MAX_INPUT];
	float distances[MAX_INPUT];
	bool firststt = true;
	int lastas = 0;
	bool generated = false;
	bool generating = false;
	int viewportX;
	int viewportY;
	sf::Image result;
	std::string Stage = "";
	bool abortE;
	int sw, sh;
	long long maxclones = 5000000;
	sf::Text notenough;
	CExit ExitB2;
	float noise = 0.f;
	preciseMeasure precMes[MAX_INPUT];
	sf::Image renderResult;
	CButton saveBut;
	CButtonDrawable saveButD;
	sf::RenderWindow genWin;
	sf::Sprite prevS2;
	sf::Texture prevT2;
	sf::Texture t;
	sf::Text loadingcapt;
	sf::Text bottomcapt;
	sf::Texture prevT;
	float laspectr;
};