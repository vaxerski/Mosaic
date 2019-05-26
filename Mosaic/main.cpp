//-----------------------//
//Entire code by vaxerski.
//Do not distribute.
//-----------------------//

//-----------------------//
//--------WARNING--------//
//starting from 1.5, mosaic
//uses 64-bit due to overflows.
//
//-----------------------//



#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <SFML\Graphics.hpp>
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
#include "INIReader.h"
#include <experimental/filesystem>
#include <Psapi.h>
#include <sysinfoapi.h>

using namespace std::experimental::filesystem::v1;
using namespace std::chrono;

#define MAX_INPUT        512
#define MAX_CLONES       5000000
#define SCALE            0.01
#define BRUTYAS_COUNTRY_OF_ORIGIN Amurica
#define LOADING_X        480
#define LOADING_Y        200
#define BILLION          1000000000
#define MILLION          1000000
#define DIV              1048576

typedef std::vector<sf::Sprite> sprite_vector;
typedef std::vector<sf::Texture> texture_vector;
typedef std::vector<sf::Image> image_vector;
//namespace fs = std::filesystem;

static sf::Color avg[MAX_INPUT];
static float distances[MAX_INPUT];

bool debug = false;
static unsigned int maxclones;
static float scale;
static float IS;
static int loadStage = 0;
static bool loaded = false;
bool iconEnabled = false;
static double alphaLoad = 255;
bool alphaUp = false;
static int viewportX;
static int viewportY;
static bool cursorsucc = false;

image_vector images(MAX_INPUT); //for later averaging
sprite_vector sprites(MAX_INPUT);
texture_vector Textures(MAX_INPUT);


//--------------------//
//BG ANIMATION DEFINES//
//--------------------//

float or1 = 0 * LOADING_X;
float or2 = 1 * LOADING_X;
float or3 = 2 * LOADING_X;
float or4 = 3 * LOADING_X;
sf::Color col1bg = sf::Color(81, 81, 81);
sf::Color col2bg = sf::Color(255, 255, 255);

sf::Vertex bganim[] =
{
	sf::Vertex(sf::Vector2f(or1,LOADING_Y), col1bg),
	sf::Vertex(sf::Vector2f(or1,0), col1bg),
	sf::Vertex(sf::Vector2f(or2,LOADING_Y), col2bg),
	sf::Vertex(sf::Vector2f(or2,0), col2bg)
};
sf::Vertex bganim2[] =
{
	sf::Vertex(sf::Vector2f(or2,LOADING_Y), col2bg),
	sf::Vertex(sf::Vector2f(or2,0), col2bg),
	sf::Vertex(sf::Vector2f(or3,LOADING_Y), col1bg),
	sf::Vertex(sf::Vector2f(or3,0), col1bg)
};
sf::Vertex bganim3[] =
{
	sf::Vertex(sf::Vector2f(or3,LOADING_Y), col1bg),
	sf::Vertex(sf::Vector2f(or3,0), col1bg),
	sf::Vertex(sf::Vector2f(or4,LOADING_Y), col2bg),
	sf::Vertex(sf::Vector2f(or4,0), col2bg)
};

float bgoriginx = -1 * LOADING_X;

high_resolution_clock::time_point timeorigin;
high_resolution_clock::time_point now;
static high_resolution_clock::time_point framestart;
steady_clock::duration delta;

//-------------------------------------------------//



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

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void doBGanim() {
	now = high_resolution_clock::now();
	delta = timeorigin - now;

	bgoriginx = (double)((double)delta.count() / BILLION) * (LOADING_X / 10);

	bganim[0] = sf::Vertex(sf::Vector2f(or1 + bgoriginx, 0), col1bg);
	bganim[1] = sf::Vertex(sf::Vector2f(or1 + bgoriginx, LOADING_Y), col1bg);
	bganim[2] = sf::Vertex(sf::Vector2f(or2 + bgoriginx, LOADING_Y), col2bg);
	bganim[3] = sf::Vertex(sf::Vector2f(or2 + bgoriginx, 0), col2bg);

	bganim2[0] = sf::Vertex(sf::Vector2f(or2 + bgoriginx, 0), col2bg);
	bganim2[1] = sf::Vertex(sf::Vector2f(or2 + bgoriginx, LOADING_Y), col2bg);
	bganim2[2] = sf::Vertex(sf::Vector2f(or3 + bgoriginx, LOADING_Y), col1bg);
	bganim2[3] = sf::Vertex(sf::Vector2f(or3 + bgoriginx, 0), col1bg);

	bganim3[0] = sf::Vertex(sf::Vector2f(or3 + bgoriginx, 0), col1bg);
	bganim3[1] = sf::Vertex(sf::Vector2f(or3 + bgoriginx, LOADING_Y), col1bg);
	bganim3[2] = sf::Vertex(sf::Vector2f(or4 + bgoriginx, LOADING_Y), col2bg);
	bganim3[3] = sf::Vertex(sf::Vector2f(or4 + bgoriginx, 0), col2bg);
}

void textPulse(bool speeded) {
	now = high_resolution_clock::now();
	delta = now - framestart;

	if (speeded) {
		if (alphaUp) {
			if (alphaLoad < 250) {
				alphaLoad += (double)((double)(delta.count() / 100000) / 50);
			}
			else {
				alphaUp = false;
				alphaLoad -= (double)((double)(delta.count() / 100000) / 50);
			}
		}
		else {
			if (alphaLoad > 10) {
				alphaLoad -= (double)((double)(delta.count() / 100000) / 50);
			}
			else {
				alphaUp = true;
				alphaLoad += (double)((double)(delta.count() / 100000) / 50);
			}
		}
	}
	else {
		if (alphaUp) {
			if (alphaLoad < 250) {
				alphaLoad += (double)((double)(delta.count() / MILLION) / 5);
			}
			else {
				alphaUp = false;
				alphaLoad -= (double)((double)(delta.count() / MILLION) / 5);
			}
		}
		else {
			if (alphaLoad > 10) {
				alphaLoad -= (double)((double)(delta.count() / MILLION) / 5);
			}
			else {
				alphaUp = true;
				alphaLoad += (double)((double)(delta.count() / MILLION) / 5);
			}
		}
	}

}

int calculateScreenshot() {
	std::ifstream t("mc.mosc");

	if (!t.good()) {
		MessageBox(nullptr, TEXT("Screenshot config file missing! this may lead to overwriting and errors in the future."), TEXT("Runtime Warning"), MB_OK);
		std::ofstream outfile("mc.mosc");
		outfile << "1";
		outfile.close();
	}

	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	std::string newstring = str + "1";
	std::ofstream out("mc.mosc");
	out << newstring;
	out.close();
	
	return newstring.length();
}

bool parseCFG() {
	INIReader reader("config.ini");
	if (reader.ParseError() < 0) {
		MessageBox(nullptr, TEXT("Couldn't locate config.ini, autogenerating default."), TEXT("Warning"), MB_OK);
		std::ofstream outfile("config.ini");
		outfile << "[Mosaic]\nmax_clones = 50000                  ; max amount of images on screen.Increase if mosaic doesnt produce full image.\nprecision_point = 0.05                       ; smaller precision point = more detail, more clones.";
		outfile.close();
		INIReader reader("config.ini");
	}

	maxclones = reader.GetInteger("Mosaic", "max_clones", 0);
	if (maxclones == 0) {
		MessageBox(nullptr, TEXT("Couldn't initialize config: error at maxclones. Expected: int"), TEXT("Fatal Error"), MB_OK);
		return false;
	}
	scale = reader.GetReal("Mosaic", "precision_point", 0);
	if (scale == 0) {
		MessageBox(nullptr, TEXT("Couldn't initialize config: error at precision_point. Expected: float != 0"), TEXT("Fatal Error"), MB_OK);
		return false;
	}

	debug = reader.GetBoolean("Mosaic", "debug_info", false);

	return true; //success

}




int main() {

	timeorigin = high_resolution_clock::now();



	//cfg

	if (!parseCFG()) {
		return 0;
	}

	try {
		sprite_vector clones(maxclones);
	}
	catch (std::string e) {
		string z = "Error! > Unhandled Exception: " + e;
		LPCTSTR lp = (LPCTSTR)z.c_str();
		MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
		return 0;
	}
	
	sprite_vector clones(maxclones); //init vector for clones

	//STAGE 0


	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("font.ttf"))
	{
		MessageBox(nullptr, TEXT("Couldn't load font.ttf"), TEXT("Fatal Error"), MB_OK);
		return 0;
	}
	text.setFont(font);

	text.setCharacterSize(24);


	sf::Text loadingcapt;
	loadingcapt.setFont(font);
	loadingcapt.setCharacterSize(24);
	loadingcapt.setColor(sf::Color(57, 239, 115, 255));
	loadingcapt.setOutlineThickness(1.f);
	loadingcapt.setOutlineColor(sf::Color(0, 0, 0, 249));

	sf::Text bottomcapt;
	bottomcapt.setFont(font);
	bottomcapt.setCharacterSize(12);
	bottomcapt.setColor(sf::Color(57, 239, 115, 200));
//	bottomcapt.setOutlineThickness(0.5f);
//	bottomcapt.setOutlineColor(sf::Color(0, 0, 0, 249));
	bottomcapt.setString("If the app goes unresponsive, DO NOT close it. It's still running.");
	bottomcapt.setPosition(LOADING_X / 2 - bottomcapt.getLocalBounds().width / 2, LOADING_Y * 0.9f);
	
	sf::RectangleShape loadingbarbg(sf::Vector2f(LOADING_X * 0.75, LOADING_Y * 0.12));
	loadingbarbg.setPosition(sf::Vector2f(LOADING_X * 0.125, LOADING_Y * 0.6));
	loadingbarbg.setFillColor(sf::Color(50, 50, 50, 60));

	sf::RectangleShape loadingbar(sf::Vector2f(LOADING_X * 0.75, LOADING_Y * 0.12));
	loadingbar.setPosition(sf::Vector2f(LOADING_X * 0.125, LOADING_Y * 0.6));
	loadingbar.setFillColor(sf::Color(57, 239, 115, 255));

	//STAGE 1

	std::string files[MAX_INPUT]; //max files

	static int i = 1;
	static int lw, lh;
	static int cw, ch;


	//STAGE 2

	static int j = 1;

	std::string maximus;
	sf::Color avg1, avg2;



	static int ij = 1;

	//STAGE 3

	sf::Image source;
	static unsigned int width, height;
	static unsigned int sw, sh;
	if (!source.loadFromFile("source.png")) {
		MessageBox(nullptr, TEXT("Couldn't load source.png"), TEXT("Fatal Error"), MB_OK);
		return 0;
	}
	std::ifstream in("source.png");
	in.seekg(16);
	in.read((char *)&width, 4);
	in.read((char *)&height, 4);


	//STAGE 4

	static int rowX, rowY;
	rowX = 0;
	rowY = 0;
	bool done = false;
	static unsigned int counter = 0;



	static sf::Color measure;

	//init loading window.
	static sf::RenderWindow window;
	window.create(sf::VideoMode(LOADING_X, LOADING_Y), "Mosaic > Initializing...");
	window.setFramerateLimit(99999);
	sf::Image icon;
	if (icon.loadFromFile("icon.png")) {
		iconEnabled = true;
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	

	while (window.isOpen())
	{
		framestart = high_resolution_clock::now();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && loaded)
			{
				if (event.key.code == sf::Keyboard::F11)
				{

					sf::Image fuckingScreenshot;
					fuckingScreenshot = window.capture();
					std::stringstream filename;
					filename << "screenshots/screenshot_" << calculateScreenshot() << ".png";
					fuckingScreenshot.saveToFile(filename.str());

				}

				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
					return 0;
				}

			}
		}

		std::string Stage = "";

		switch (loadStage) {

		case 0:

			loadStage = 1;
			Stage = "Getting images...";
			loadingcapt.setString(Stage);
			loadingcapt.setPosition(LOADING_X / 2 - loadingcapt.getLocalBounds().width / 2, LOADING_Y / 2 - loadingcapt.getLocalBounds().height);


			loadingbar.setSize(sf::Vector2f(LOADING_X * 0.75 * 1 / 3, loadingbar.getSize().y));
			break;
		case 1:

			//load all files' names from directory

			lw = lh = -1;

			for (const auto & entry : directory_iterator("images")) { //get paths
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
							string z = "All textures must be the same size PNG's. Error at: " + pathh1;
							LPCTSTR lp = (LPCTSTR)z.c_str();
							MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
							return 0;
						}
					}

				}
				i++;
			}

			loadStage = 2;
			Stage = "Parsing textures...";
			loadingcapt.setString(Stage);
			loadingcapt.setPosition(LOADING_X / 2 - loadingcapt.getLocalBounds().width / 2, LOADING_Y / 2 - loadingcapt.getLocalBounds().height);
			loadingbar.setSize(sf::Vector2f(LOADING_X * 0.75 * 1/2, loadingbar.getSize().y));
			break;
		case 2:

			for (j; j < i; j++) {
				framestart = high_resolution_clock::now();
				if (!Textures[j].loadFromFile(files[j], sf::IntRect(0, 0, lw, lh))) {
					std::string yeet = std::string("Error while parsing textures. Error at: " + files[j] + "\nDo you have enough RAM?");
					LPCTSTR lp = (LPCTSTR)yeet.c_str();
					MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
					return 0;
				}
				if (!images[j].loadFromFile(files[j])) {
					std::string yeet = ("Error while creating buffer image. Error at: " + files[j] + "\nDo you have enough RAM?");
					LPCTSTR lp = (LPCTSTR)yeet.c_str();
					MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
					return 0;
				}
				sf::Color c1, c2, c3, c4;

				c1 = images[j].getPixel((int)lw / 2, (int)lh / 2); //center
				c2 = images[j].getPixel((int)lw / 2, (int)lh / 4); //top center
				c3 = images[j].getPixel((int)lw / 4, (int)lh*0.75); //left lower
				c4 = images[j].getPixel((int)lw*0.75, (int)lh*0.75); //right lower

				avg1 = sf::Color((c1.r + c2.r) / 2, (c1.g + c2.g) / 2, (c1.b + c2.b) / 2);
				avg2 = sf::Color((c3.r + c4.r) / 2, (c3.g + c4.g) / 2, (c3.b + c4.b) / 2);

				avg[j] = sf::Color((avg1.r + avg2.r) / 2, (avg1.g + avg2.g) / 2, (avg1.b + avg2.b) / 2);

				Stage = "Parsing textures... " + std::to_string(j + 1) + " of " + std::to_string(i + 1);
				loadingcapt.setString(Stage);
				loadingcapt.setPosition(LOADING_X / 2 - loadingcapt.getLocalBounds().width / 2, LOADING_Y / 2 - loadingcapt.getLocalBounds().height);
				loadingbar.setSize(sf::Vector2f(LOADING_X * 0.75 * 1 / 2, loadingbar.getSize().y));

				doBGanim();

				textPulse(false);

				loadingcapt.setOutlineColor(sf::Color(0, 0, 0, alphaLoad));

				window.clear(sf::Color::White);
				window.draw(bganim, 4, sf::Quads);
				window.draw(bganim2, 4, sf::Quads);
				window.draw(bganim3, 4, sf::Quads);
				window.draw(loadingcapt);
				window.draw(loadingbarbg);
				window.draw(loadingbar);
				window.draw(bottomcapt);
				window.display();

			}


			for (ij; ij < i; ij++) {
				framestart = high_resolution_clock::now();
				sprites[ij].setTexture(Textures[ij]);
				sprites[ij].setScale(scale, scale);

				Stage = "Assigning sprites... " + std::to_string(ij + 1) + " of " + std::to_string(i + 1);
				loadingcapt.setString(Stage);
				loadingcapt.setPosition(LOADING_X / 2 - loadingcapt.getLocalBounds().width / 2, LOADING_Y / 2 - loadingcapt.getLocalBounds().height);
				loadingbar.setSize(sf::Vector2f(LOADING_X * 0.75 * 1 / 1.7f, loadingbar.getSize().y));

				doBGanim();

				textPulse(false);

				loadingcapt.setOutlineColor(sf::Color(0, 0, 0, alphaLoad));

				window.clear(sf::Color::White);
				window.draw(bganim, 4, sf::Quads);
				window.draw(bganim2, 4, sf::Quads);
				window.draw(bganim3, 4, sf::Quads);
				window.draw(loadingcapt);
				window.draw(loadingbarbg);
				window.draw(loadingbar);
				window.draw(bottomcapt);
				window.display();
			}
			loadStage = 3;
			Stage = "Initializing source...";
			loadingcapt.setString(Stage);
			loadingcapt.setPosition(LOADING_X / 2 - loadingcapt.getLocalBounds().width / 2, LOADING_Y / 2 - loadingcapt.getLocalBounds().height);
			loadingbar.setSize(sf::Vector2f(LOADING_X * 0.75 * 1 / 1.6f, loadingbar.getSize().y));
			break;
		case 3:

			//initialize source

			sw = ntohl(width);
			sh = ntohl(height);

			//get viewport info
			HWND hwnd;
			MONITORINFO moninfo;
			HMONITOR monitor;

			hwnd = FindWindow(NULL, TEXT("Mosaic > Initializing..."));

			monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);

			if (monitor != NULL) {   //so we dont blow up our pc by reading some null shit
				moninfo.cbSize = sizeof(moninfo);
				GetMonitorInfoA(monitor, &moninfo);

				viewportX = moninfo.rcMonitor.right - moninfo.rcMonitor.left;
				viewportY = moninfo.rcMonitor.bottom - moninfo.rcMonitor.top;

				if (sw > viewportX || sh > viewportY) {
					std::string z = std::string("The source image is bigger than your current viewport. The mosaic window will *not* fit on the screen. To capture the mosaic, press F11. To exit, press ESC.\n\nDo not panic! Everything will be fine.\n\n(viewport size: " + std::to_string(viewportX) + "x" + std::to_string(viewportY) + ", source size: " + std::to_string(sw) + "x" + std::to_string(sh) + ")");
					LPCTSTR lp = (LPCTSTR)z.c_str();
					MessageBox(nullptr, lp, TEXT("Warning!"), MB_OK);
				}
			}

			loadStage = 4;
			Stage = "Generating the image...";
			loadingcapt.setString(Stage);
			loadingcapt.setPosition(LOADING_X / 2 - loadingcapt.getLocalBounds().width / 2, LOADING_Y / 2 - loadingcapt.getLocalBounds().height);
			loadingbar.setSize(sf::Vector2f(LOADING_X * 0.75 * 1 / 1.4f, loadingbar.getSize().y));
			break;

		case 4:
		{
			int cumter = 0;
			framestart = high_resolution_clock::now();

			for (int x = 0; x < maxclones; x++) {


				if (rowX*scale*lw > sw) {
					rowX = 0;
					rowY++;
				}

						//we have correct row and column here already.

				int boxX = rowX * scale*lw;
				int boxY = rowY * scale*lh;

				if (boxX + 0.5*scale*lw > sw || boxY + 0.5*scale*lh > sh) {
					if (boxX < sw && boxY < sh) {
						measure = source.getPixel((int)(boxX), (int)(boxY));
					}
					else if ((rowY + 1)*scale*lh < sh) {
						measure = source.getPixel((int)(0), (int)((rowY + 1)*scale*lh));
					}
					else {
						break;
					}

				}
				else {
					measure = source.getPixel((int)(boxX + 0.5*scale*lw), (int)(boxY + 0.5*scale*lh));
				}


				clones[counter] = sprites[getNearest3D(measure, i)];
				clones[counter].setPosition(rowX*scale*lw, rowY*scale*lh);

				rowX++;
				counter++;
				cumter++;

				if (cumter > 188) {
					Stage = "Generating the image... " + std::to_string(x + 1) + "/" + std::to_string(maxclones);
					loadingcapt.setString(Stage);
					loadingcapt.setPosition(LOADING_X / 2 - loadingcapt.getLocalBounds().width / 2, LOADING_Y / 2 - loadingcapt.getLocalBounds().height);
					loadingbar.setSize(sf::Vector2f(LOADING_X * 0.75 * 1 / 1.4f, loadingbar.getSize().y));

					doBGanim();

					textPulse(true);

					loadingcapt.setOutlineColor(sf::Color(0, 0, 0, alphaLoad));

					window.clear(sf::Color::White);
					window.draw(bganim, 4, sf::Quads);
					window.draw(bganim2, 4, sf::Quads);
					window.draw(bganim3, 4, sf::Quads);
					window.draw(loadingcapt);
					window.draw(loadingbarbg);
					window.draw(loadingbar);
					window.draw(bottomcapt);
					window.display();

					cumter = 0;
					framestart = high_resolution_clock::now();
				}
			}

			loadStage = 5;
			loaded = true;
			window.close();
			window.create(sf::VideoMode(sw, sh), "Mosaic");
			window.setFramerateLimit(0.5f);
			if(iconEnabled) window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


			Stage = "Rendering...";
			loadingcapt.setString(Stage);
			loadingcapt.setPosition(sw / 2 - loadingcapt.getLocalBounds().width / 2, sh / 2 - loadingcapt.getLocalBounds().height);

			loadingcapt.setOutlineColor(sf::Color(0, 0, 0, 255));

			window.clear(sf::Color::White);
			window.draw(loadingcapt);
			window.draw(bottomcapt);
			window.display();
			break;
		}
		}
		if (!loaded) {

			doBGanim();

			textPulse(false);

			loadingcapt.setOutlineColor(sf::Color(0, 0, 0, alphaLoad));

			window.clear(sf::Color::White);
			window.draw(bganim, 4, sf::Quads);
			window.draw(bganim2, 4, sf::Quads);
			window.draw(bganim3, 4, sf::Quads);
			window.draw(loadingcapt);
			window.draw(loadingbarbg);
			window.draw(loadingbar);
			window.draw(bottomcapt);
			window.display();
		}
		else {
			window.clear();
			std::string maximus;
			if(counter >= maxclones) maximus = "\nInput Count: " + std::to_string(i) + "\nCLONE LIMIT REACHED (" + std::to_string(counter) + ")\nX: " + std::to_string(sw) + "\nY: " + std::to_string(sh);
			else maximus = "\nInput Count: " + std::to_string(i) + "\nClone Count: " + std::to_string(counter) + "\nX: " + std::to_string(sw) + "\nY: " + std::to_string(sh);
			text.setPosition(0, sh / 2);
			text.setString(maximus);
			
			for (int x = 1; x < counter; x++) {
				window.draw(clones[x]);
			}
			
			if (debug) window.draw(text);
		}
		window.display();
	}

	return 0;

}




