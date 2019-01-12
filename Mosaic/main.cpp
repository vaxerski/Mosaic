#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include <string>
#undef main
#include <tchar.h>
#include <chrono>
#include <ctime>
#include <ratio>
#include <random>
#include <sstream>
#include <fstream>  
#include <winsock.h>
#include <math.h>
#include <filesystem>
#include <cmath>

#define MAX_INPUT        512
#define MAX_CLONES       8192
#define SCALE            0.01
#define BRUTYAS_COUNTRY_OF_ORIGIN Not_belarus

typedef std::vector<sf::Sprite> sprite_vector;
namespace fs = std::filesystem;

sf::Color avg[MAX_INPUT];
float distances[MAX_INPUT];


int getNearest3D(sf::Color source, int scanRange) {
	int leader;
	float distance = 1337;
	float dis;

	int r, g, b;
	r = source.r;
	g = source.g;
	b = source.b;
	int r1, g1, b1;

	for (int x = 1; x < scanRange; x++) {
		r1 = avg[x].r;
		g1 = avg[x].g;
		b1 = avg[x].b;

		dis = sqrt(pow((r - r1), 2) + pow((g - g1), 2) + pow((b - b1), 2));
		distances[x] = dis;

		if (dis < distance) {
			leader = x;
			distance = dis;
		}
	}

	return leader;

}



int main() {

	//load all files' names from directory

	std::string files[MAX_INPUT]; //max files
	sf::Image images[MAX_INPUT]; //for later averaging
	
	int i = 1;
	int lw, lh;
	int cw, ch;


	lw = lh = -1;

	for (const auto & entry : fs::directory_iterator("images")) { //get paths
		if (i == MAX_INPUT) {
			break;
		}
		else {

			if (lw == -1) {

				unsigned int width, height;
				std::string pathh = entry.path().u8string();
				std::size_t pos = pathh.find("images\\");
				std::string path_ = pathh.substr(pos);

				std::ifstream in(path_);

				in.seekg(16);
				in.read((char *)&width, 4);
				in.read((char *)&height, 4);

				lw = ntohl(width);
				lh = ntohl(height);

				files[i] = path_;




			}
			else {
				unsigned int width, height;
				std::string pathh1 = entry.path().u8string();
				std::size_t pos1 = pathh1.find("images\\");
				std::string path_1 = pathh1.substr(pos1);

				std::ifstream in(path_1);

				in.seekg(16);
				in.read((char *)&width, 4);
				in.read((char *)&height, 4);

				cw = ntohl(width);
				ch = ntohl(height);
				
				if (cw == lw && ch == lh) {

					files[i] = path_1;
				}
				else {
					MessageBox(nullptr, TEXT("All Textures must have the same dimensions and be PNG format."), TEXT("Fatal Error"), MB_OK);
					return 0;
				}
			}
			
		}
		i++;
	}
	int j = 1;
	sf::Texture Textures[MAX_INPUT];
	std::string maximus;
	sf::Color avg1, avg2;

	for (j; j < i; j++) {
		if (!Textures[j].loadFromFile(files[j], sf::IntRect(0, 0, lw, lh))) {
			MessageBox(nullptr, TEXT("Error while parsing textures."), TEXT("Fatal Error"), MB_OK);
			return 0;
		}
		if (!images[j].loadFromFile(files[j])) {
			MessageBox(nullptr, TEXT("Error while creating buffer image."), TEXT("Fatal Error"), MB_OK);
			return 0;
		}
		sf::Color c1, c2, c3, c4;

		c1 = images[j].getPixel((int)lw / 2, (int)lh / 2); //center
		c2 = images[j].getPixel((int)lw / 2, (int)lh / 4); //top center
		c3 = images[j].getPixel((int)lw / 4, (int)lh*0.75); //left lower
		c4 = images[j].getPixel((int)lw*0.75, (int)lh*0.75); //right lower

		avg1 = sf::Color((c1.r + c2.r)/2, (c1.g + c2.g) / 2, (c1.b + c2.b) / 2);
		avg2 = sf::Color((c3.r + c4.r)/2, (c3.g + c4.g) / 2, (c3.b + c4.b) / 2);

		avg[j] = sf::Color((avg1.r+avg2.r)/2, (avg1.g + avg2.g) / 2, (avg1.b + avg2.b) / 2);
		
	}

	sf::Sprite sprites[MAX_INPUT];
	int ij = 1;

	for (ij; ij < i; ij++) {
		sprites[ij].setTexture(Textures[ij]);
		sprites[ij].setScale(SCALE, SCALE);
	}

//	int bestr = getNearest3D(sf::Color::Red, i);

	//initialize source

	sf::Image source;
	if (!source.loadFromFile("source.png")) {
		MessageBox(nullptr, TEXT("Couldn't load source.png"), TEXT("Fatal Error"), MB_OK);
		return 0;
	}
	unsigned int width, height;
	unsigned int sw, sh;

	std::ifstream in("source.png");

	in.seekg(16);
	in.read((char *)&width, 4);
	in.read((char *)&height, 4);

	sw = ntohl(width);
	sh = ntohl(height);




	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("font.ttf"))
	{
		return 0;
	}
	text.setFont(font);

	text.setCharacterSize(24);


	
	int rowX, rowY;
	rowX = 0;
	rowY = 0;
	bool done = false;
	unsigned int counter = 0;



	sprite_vector clones(MAX_CLONES);

//	DON'T DO THAT   >>>>  sf::Sprite clones[MAX_CLONES];
//  your pc wont handle that
//  It will throw stack overflow error, use dynamic allocation.
//  sprite_vector typedef is just std::vector<sf::sprite> btw


	sf::Color measure;

	for (int x = 0; x < MAX_CLONES; x++) {
		
		
		if (rowX*SCALE*lw > 1280) {
			rowX = 0;
			rowY++;
		}

		if (rowX*SCALE*lw > 1280 && rowY*SCALE*lh > 720) {
			done = true;
			break;
		}

		//we have correct row and column here already.

		int boxX = rowX * SCALE*lw;
		int boxY = rowY * SCALE*lh;

		if (boxX + 0.5*SCALE*lw >= sw || boxY + 0.5*SCALE*lh >= sh) {
			if (boxX >= sw && boxY + 1 >= sh) {
				measure = source.getPixel((int)(boxX), (int)(boxY));
			}
			else {
				break;
			}
			
		}
		else {
			measure = source.getPixel((int)(boxX + 0.5*SCALE*lw), (int)(boxY + 0.5*SCALE*lh));
		}

		

		clones[counter] = sprites[getNearest3D(measure, i)];
		clones[counter].setPosition(rowX*SCALE*lw, rowY*SCALE*lh);
		
        
//		sprites[x].setPosition(rowX*0.1*lw, rowY*0.1*lh);
		rowX++;
		counter++;
	}
	sf::RenderWindow window(sf::VideoMode(sw, sh), "Mosaic");   //render window

	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		std::string maximus = "\n3:" + std::to_string(distances[3]) + "\n2:" + std::to_string(distances[2]) + "\n1:" + std::to_string(distances[1]);
		text.setString(maximus);
		
		window.clear();
	
//		window.draw(sprites[bestr]);
		for (int x = 1; x < counter; x++) {
			window.draw(clones[x]);
        }
//		window.draw(text);
		window.display();
	}

	return 0;

}




