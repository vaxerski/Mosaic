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
#include <experimental/filesystem>
#include <Psapi.h>
#include <sysinfoapi.h>
#include <dwmapi.h>
#define WIN32_LEAN_AND_MEAN
#include <dwmapi.h>
#include <unknwn.h>
#include <gdiplus.h>
#pragma comment( lib, "gdiplus" )

#include "INIReader.h"
#include "parsing.h"
#include "math.h"
#include "helpers.h"
#include "render.h"
#include "controls.h"
#include "Generate.h"

//defines
typedef std::vector<sf::Sprite> sprite_vector;
typedef std::vector<sf::Texture> texture_vector;
typedef std::vector<sf::Image> image_vector;
typedef std::vector<sf::Color> color_vec;

image_vector images(MAX_INPUT);
sprite_vector sprites(MAX_INPUT);
texture_vector Textures(MAX_INPUT);
color_vec dScan_cols(MAX_SAMPLES);
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

CButton saveBut;
CButtonDrawable saveButD;
sf::RenderWindow genWin;

extern sf::Text prevWarn;
extern std::string savepath;
extern sf::Text loadingcapt;
extern sf::Text bottomcapt;
extern sf::RectangleShape loadingbarbg;
extern sf::RectangleShape loadingbar;
extern sf::RenderWindow* pWind;
extern float scale;
extern bool dScan;
extern int aspectr;


int j = 1;

int Generate::generateImage(std::string comp, std::string sourcp, sf::Image* imag) {


	generating = true;
	try {
		sprite_vector clones(maxclones);
	}
	catch (std::string e) {
		string z = "Error! > Unhandled Exception: " + e;
		LPCTSTR lp = (LPCTSTR)z.c_str();
		MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
		return ERROR_VEC_INIT;
	}

	sprite_vector clones(maxclones); //init vector for clones

	

	int i = 1;
	int lw, lh;
	int cw, ch;


	int ij = 1;

	sf::Color avg1, avg2;

	sf::Image source;
	

	if (!source.loadFromFile(sourcp)) {
		return ERROR_SOURCE;
	}

	parsing::GetImageSize(sourcp.c_str(), &sw, &sh);

	if (sw == 0 || sh == 0) {
		return ERROR_SOURCE;
	}

	int rowX, rowY;
	rowX = 0;
	rowY = 0;
	bool done = false;
	unsigned int counter = 0;

	

	sf::Color measure;

	//here starts the work

	lw = lh = -1;

	Stage = "Getting images...";
	loadingcapt.setString(Stage);
	loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
	loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 3, loadingbar.getSize().y));

	Helpers::forceRender(pWind);

	for (const auto& entry : directory_iterator(comp)) { //get paths



		if (i == MAX_INPUT) {
			break;
		}
		else {

			//we dont need lots of that anymore :crabrave:

			if (lw == -1) {

				unsigned int width, height;
				std::string pathh = entry.path().u8string();
				const char* pth = pathh.c_str();


				parsing::GetImageSize(pth, &cw, &ch);

				lw = cw;
				lh = ch;

				files[i] = pathh;

			}
			else {
				unsigned int width, height;
				std::string pathh1 = entry.path().u8string();
				const char* pth1 = pathh1.c_str();

				parsing::GetImageSize(pth1, &cw, &ch);

				files[i] = pathh1;
			}

		}
		i++;
		if (abortE) {
			Stage = "Aborted";
			loadingcapt.setString(Stage);
			loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
			loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
			abortE = false;
			return ERROR_SUCCESS;
		}
	}



	Stage = "Parsing textures...";
	loadingcapt.setString(Stage);
	loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
	loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 2, loadingbar.getSize().y));
	Helpers::forceRender(pWind);

	switch (aspectr) {
	case 0:
		//16:9
		lw = (lh * 16) / 9;
		break;
	case 1:
		//4:3
		lw = (lh * 4) / 3;
		break;
	case 2:
		//1:1
		lw = (lh * 1) / 1;
		break;
	}

	if (!firststt) {
		if (aspectr != lastas || files != lastfiles) {
			for (j; j < i; j++) {
				if (!Textures[j].loadFromFile(files[j], sf::IntRect(0, 0, lw, lh))) {
					std::string yeet = std::string("Error while parsing textures. Error at: " + files[j] + "\nDo you have enough RAM?");
					LPCTSTR lp = (LPCTSTR)yeet.c_str();
					MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
					Stage = "Error";
					loadingcapt.setString(Stage);
					loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
					loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
					return 0;
				}
				if (!images[j].loadFromFile(files[j])) {
					std::string yeet = ("Error while creating buffer image. Error at: " + files[j] + "\nDo you have enough RAM?");
					LPCTSTR lp = (LPCTSTR)yeet.c_str();
					MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
					Stage = "Error";
					loadingcapt.setString(Stage);
					loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
					loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
					return 0;
				}
				sf::Color c1, c2, c3, c4;

				int ix, iy;

				std::string pathh = files[j];
				const char* pth = pathh.c_str();

				parsing::GetImageSize(pth, &ix, &iy);

				c1 = images[j].getPixel((int)ix / 2, (int)iy / 2); //center
				c2 = images[j].getPixel((int)ix / 2, (int)iy / 4); //top center
				c3 = images[j].getPixel((int)ix / 4, (int)iy * 0.75); //left lower
				c4 = images[j].getPixel((int)ix * 0.75, (int)iy * 0.75); //right lower

				if (dScan) {
					int pp;
					double r, g, b = 0;
					float ir, ig, ib;

					for (int p = 0; p < MAX_SAMPLES; p++) {
						if (p >= ix * iy) break;
						dScan_cols[p] = images[j].getPixel((int)p % ix, (int)floor(p / ix));
						r += dScan_cols[p].r;
						g += dScan_cols[p].g;
						b += dScan_cols[p].b;
						pp = p;
					}
					ir = r / pp;
					ig = g / pp;
					ib = b / pp;

					avg[j] = sf::Color(ir, ig, ib);
				}
				else {
					avg1 = sf::Color((c1.r + c2.r) / 2, (c1.g + c2.g) / 2, (c1.b + c2.b) / 2);
					avg2 = sf::Color((c3.r + c4.r) / 2, (c3.g + c4.g) / 2, (c3.b + c4.b) / 2);

					avg[j] = sf::Color((avg1.r + avg2.r) / 2, (avg1.g + avg2.g) / 2, (avg1.b + avg2.b) / 2);
				}



				Stage = "Parsing composite... " + std::to_string(j + 1) + " of " + std::to_string(i + 1);
				loadingcapt.setString(Stage);
				loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
				loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 2, loadingbar.getSize().y));
				Helpers::forceRender(pWind);
				if (abortE) {
					Stage = "Aborted";
					loadingcapt.setString(Stage);
					loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
					loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
					abortE = false;
					return ERROR_SUCCESS;
				}
			}
		}
	}
	else {
		for (j; j < i; j++) {
			if (!Textures[j].loadFromFile(files[j], sf::IntRect(0, 0, lw, lh))) {
				std::string yeet = std::string("Error while parsing textures. Error at: " + files[j] + "\nDo you have enough RAM?");
				LPCTSTR lp = (LPCTSTR)yeet.c_str();
				MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
				Stage = "Error";
				loadingcapt.setString(Stage);
				loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
				loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
				return 0;
			}
			if (!images[j].loadFromFile(files[j])) {
				std::string yeet = ("Error while creating buffer image. Error at: " + files[j] + "\nDo you have enough RAM?");
				LPCTSTR lp = (LPCTSTR)yeet.c_str();
				MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
				Stage = "Error";
				loadingcapt.setString(Stage);
				loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
				loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
				return 0;
			}
			sf::Color c1, c2, c3, c4;

			int ix, iy;

			std::string pathh = files[j];
			const char* pth = pathh.c_str();

			parsing::GetImageSize(pth, &ix, &iy);

			c1 = images[j].getPixel((int)ix / 2, (int)iy / 2); //center
			c2 = images[j].getPixel((int)ix / 2, (int)iy / 4); //top center
			c3 = images[j].getPixel((int)ix / 4, (int)iy * 0.75); //left lower
			c4 = images[j].getPixel((int)ix * 0.75, (int)iy * 0.75); //right lower

			if (dScan) {
				int pp;
				double r, g, b = 0;
				float ir, ig, ib;

				for (int p = 0; p < MAX_SAMPLES; p++) {
					if (p >= ix * iy) break;
					dScan_cols[p] = images[j].getPixel((int)p % ix, (int)floor(p / ix));
					r += dScan_cols[p].r;
					g += dScan_cols[p].g;
					b += dScan_cols[p].b;
					pp = p;
				}
				ir = r / pp;
				ig = g / pp;
				ib = b / pp;

				avg[j] = sf::Color(ir, ig, ib);
			}
			else {
				avg1 = sf::Color((c1.r + c2.r) / 2, (c1.g + c2.g) / 2, (c1.b + c2.b) / 2);
				avg2 = sf::Color((c3.r + c4.r) / 2, (c3.g + c4.g) / 2, (c3.b + c4.b) / 2);

				avg[j] = sf::Color((avg1.r + avg2.r) / 2, (avg1.g + avg2.g) / 2, (avg1.b + avg2.b) / 2);
			}



			Stage = "Parsing composite... " + std::to_string(j + 1) + " of " + std::to_string(i + 1);
			loadingcapt.setString(Stage);
			loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
			loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 2, loadingbar.getSize().y));
			Helpers::forceRender(pWind);
			if (abortE) {
				Stage = "Aborted";
				loadingcapt.setString(Stage);
				loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
				loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
				abortE = false;
				return ERROR_SUCCESS;
			}
		}
		firststt = false;

		for(int g = 0; g < MAX_INPUT; g++)
			lastfiles[g] = files[g];
	}

	

	for (ij; ij < i; ij++) {
		sprites[ij].setTexture(Textures[ij]);
		sprites[ij].setScale(scale, scale);

		Stage = "Assigning sprites... " + std::to_string(ij + 1) + " of " + std::to_string(i + 1);
		loadingcapt.setString(Stage);
		loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
		loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 1.7f, loadingbar.getSize().y));
		Helpers::forceRender(pWind);
		if (abortE) {
			Stage = "Aborted";
			loadingcapt.setString(Stage);
			loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
			loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
			abortE = false;
			return ERROR_SUCCESS;
		}
	}

	Stage = "Initializing source...";
	loadingcapt.setString(Stage);
	loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
	loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 1.6f, loadingbar.getSize().y));
	Helpers::forceRender(pWind);

	//get viewport info
	HWND hwnd;
	MONITORINFO moninfo;
	HMONITOR monitor;

	hwnd = FindWindow(NULL, TEXT("Mosaic 2"));

	monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);

	if (monitor != NULL) {
		moninfo.cbSize = sizeof(moninfo);
		GetMonitorInfoA(monitor, &moninfo);

		viewportX = moninfo.rcMonitor.right - moninfo.rcMonitor.left;
		viewportY = moninfo.rcMonitor.bottom - moninfo.rcMonitor.top;

		if (sw > viewportX || sh > viewportY) {
			std::string z = std::string("The source image is bigger than your current viewport. The full view will *not* fit on the screen. To exit, press ESC.\n\nDo not panic! Everything will be fine.\n\n(viewport size: " + std::to_string(viewportX) + "x" + std::to_string(viewportY) + ", source size: " + std::to_string(sw) + "x" + std::to_string(sh) + ")");
			LPCTSTR lp = (LPCTSTR)z.c_str();
			MessageBox(nullptr, lp, TEXT("Warning!"), MB_OK);
		}
	}
	Stage = "Generating the image...";
	loadingcapt.setString(Stage);
	loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
	loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 1.4f, loadingbar.getSize().y));
	Helpers::forceRender(pWind);

	int cumter = 0;
	bool fixSkip = true;

	for (int x = 0; x < maxclones; x++) {

		//20.09.2019 - Jesus fucking christ thanks younger me for not hardcoding a thing in here, holy shit saved me a lot of time
		//           - Doing clone free aspect btw

		if (!fixSkip) {
			if (rowX * scale * lw > sw) {
				rowX = 0;
				rowY++;
			}

			//we have correct row and column here already.

			int boxX = rowX * scale * lw;
			int boxY = rowY * scale * lh;

			if (boxX + 0.5 * scale * lw > sw || boxY + 0.5 * scale * lh > sh) {
				if (boxX < sw && boxY < sh) {
					measure = source.getPixel((int)(boxX), (int)(boxY));
				}
				else if ((rowY + 1) * scale * lh < sh) {
					measure = source.getPixel((int)(0), (int)((rowY + 1) * scale * lh));
				}
				else {
					break;
				}

			}
			else {
				measure = source.getPixel((int)(boxX + 0.5 * scale * lw), (int)(boxY + 0.5 * scale * lh));
			}

			if (noise > 0) {
				clones[counter] = sprites[Helpers::getNearest3D(sf::Color(measure.r + rand() % (int)noise - noise / 2, measure.g + rand() % (int)noise - noise / 2, measure.b + rand() % (int)noise - noise / 2), i)];
			}
			else {
				clones[counter] = sprites[Helpers::getNearest3D(measure, i)];
			}

			clones[counter].setPosition(rowX * scale * lw, rowY * scale * lh);

			rowX++;
			counter++;
			cumter++;

			if (cumter > 586) {
				Stage = "Generating the image... " + std::to_string(x + 1) + "/" + std::to_string(maxclones);
				loadingcapt.setString(Stage);
				loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
				loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 1.4f, loadingbar.getSize().y));
				Helpers::forceRender(pWind);

				cumter = 0;
			}
			if (abortE) {
				Stage = "Aborted";
				loadingcapt.setString(Stage);
				loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
				loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
				abortE = false;
				return ERROR_SUCCESS;
			}
		}
		else {
			measure = source.getPixel((int)(1), (int)(1));
			clones[counter] = sprites[Helpers::getNearest3D(measure, i)];
			clones[counter].setPosition(0, 0);
			fixSkip = false;
			counter++;
			//yay another shit fix
		}


	}

	genWin.create(sf::VideoMode(sw, sh), "Generating", sf::Style::None);
	genWin.setPosition(sf::Vector2i(10000, 10000));

	int render = 0;
	int countere = 0;

	Stage = "Generated!";
	loadingcapt.setString(Stage);
	loadingcapt.setPosition(BAR_X / 2 - loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - loadingcapt.getLocalBounds().height / 2 + 598);
	loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
	Helpers::forceRender(pWind);

	notenough.setFillColor(sf::Color(200, 200, 200, 200));
	notenough.setCharacterSize(50);
	notenough.setString("There was not enough clones to show this image.");
	notenough.setPosition(sw / 2 - notenough.getLocalBounds().width / 2, sh / 2 - notenough.getLocalBounds().height / 2);

	while (genWin.isOpen()) {
		if (render == 3) { //SFML and double-buffering, so 3
			sf::Image capture;
			capture = genWin.capture();
			*imag = capture;
			result = capture;
			genWin.close();
		}
		genWin.clear();
		for (int x = 1; x < counter; x++) {
			genWin.draw(clones[x]);
		}
		if (counter > maxclones - 2) {
			genWin.draw(notenough);
		}
		genWin.display();
		render++;
	}

	generated = true;
	generating = false;

}

void setupSave(sf::Font* font) {
	//get viewport info
	HWND hwnd;
	MONITORINFO moninfo;
	HMONITOR monitor;

	hwnd = FindWindow(NULL, TEXT("Preview"));

	monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);

	if (monitor != NULL) {
		moninfo.cbSize = sizeof(moninfo);
		GetMonitorInfoA(monitor, &moninfo);

		viewportX = moninfo.rcMonitor.right - moninfo.rcMonitor.left;
		viewportY = moninfo.rcMonitor.bottom - moninfo.rcMonitor.top;
	}


	saveBut.w = 100;
	saveBut.h = 30;
	saveBut.x = sw / 2 - 50;
	if (sh > viewportY) {
		saveBut.y = viewportY - 70 + (sh - viewportY)/2;
	}
	else {
		saveBut.y = sh - 70;

		
	}
	saveBut.callbackID = NONE;
	saveBut.caption = "Save as...";

	saveButD.bg.setSize(sf::Vector2f(saveBut.w, saveBut.h));
	saveButD.bg.setPosition(sf::Vector2f(saveBut.x, saveBut.y));
	saveButD.bg.setFillColor(sf::Color(74, 74, 74));
	saveButD.fg.setSize(sf::Vector2f(saveBut.w - 4, saveBut.h - 4));
	saveButD.fg.setPosition(sf::Vector2f(saveBut.x + 2, saveBut.y + 2));
	saveButD.fg.setFillColor(sf::Color(85, 85, 85));
	saveButD.text.setFont(*font);
	saveButD.text.setString("Save as...");
	saveButD.text.setCharacterSize(16);
	saveButD.text.setFillColor(sf::Color::White);
	saveButD.text.setPosition(saveBut.w / 2 - saveButD.text.getLocalBounds().width / 2 + saveBut.x, saveBut.h / 2 - saveButD.text.getLocalBounds().height / 2 + saveBut.y - 2);
	

	saveBut.drawable = &saveBut;
}

void rendSave() {
	int mx, my;
	Helpers::GetCursorToWindow(&mx, &my, &genWin);

	int x, y, w, h;
	x = saveBut.x;
	y = saveBut.y;
	w = saveBut.w;
	h = saveBut.h;

	if (x < mx && mx < x + w && y < my && my < y + h) {  //hover
		genWin.draw(saveButD.bg);
		saveButD.fg.setFillColor(sf::Color(149, 149, 149));
		genWin.draw(saveButD.fg);
		genWin.draw(saveButD.text);
	}
	else {
		genWin.draw(saveButD.bg);
		saveButD.fg.setFillColor(sf::Color(85, 85, 85));
		genWin.draw(saveButD.fg);
		genWin.draw(saveButD.text);
	}
}

void SaveAs() {
	butoncallbacks(OpenFileDialogC, nullptr, nullptr, nullptr, nullptr);
}

sf::Sprite prevS2;
sf::Texture prevT2;

void Generate::doView(sf::Font* font) {

	genWin.create(sf::VideoMode(sw, sh), "Preview", sf::Style::None);

	prevT2.loadFromImage(result);
	prevS2.setColor(sf::Color(255, 255, 255, 200));
	prevS2.setTexture(prevT2);
	prevS2.setTextureRect(sf::IntRect(0, 0, sw, sh));

	setupSave(font);

	sf::Image icon;
	if (icon.loadFromFile("resource/icon.png")) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}

	while (genWin.isOpen()) {

		sf::Event event;
		while (genWin.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				genWin.close();
			if (event.type == sf::Event::LostFocus)
				genWin.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					genWin.close();
				}
			}
			if (event.type == event.MouseButtonPressed) {
				int mx, my;
				Helpers::GetCursorToWindow(&mx, &my, &genWin);

				int x, y, w, h;
				x = saveBut.x;
				y = saveBut.y;
				w = saveBut.w;
				h = saveBut.h;

				if (x < mx && mx < x + w && y < my && my < y + h) {  //hover
					SaveAs();
					if (savepath != "ERROR") {
						std::stringstream filename;
						filename << savepath;
						result.saveToFile(filename.str());
					}
				}
			}
		}
		genWin.clear();
		genWin.draw(prevS2);
		genWin.draw(prevWarn);
		rendSave();
		genWin.display();
	}
}

void Generate::abort() {
	abortE = true;
	return;
}