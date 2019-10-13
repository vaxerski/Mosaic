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
#include <filesystem>
#include <Psapi.h>
#include <sysinfoapi.h>
#include <dwmapi.h>
#define WIN32_LEAN_AND_MEAN
#include <dwmapi.h>
#include <unknwn.h>
#include <gdiplus.h>
#include <future>
#pragma comment( lib, "gdiplus" )

#include "INIReader.h"
#include "parsing.h"
#include "math.h"
#include "helpers.h"
#include "render.h"
#include "controls.h"
#include "Generate.h"
#include "GVars.h"

extern std::mutex mu;


extern std::unique_ptr<GVars> G;
extern sf::Text prevWarn;
extern std::string savepath;
extern sf::RectangleShape loadingbarbg;
extern sf::RectangleShape loadingbar;
extern sf::RenderWindow* pWind;
extern float scale;
extern bool dScan;
extern int aspectr;

int j = 1;




int Generate::generateImage(std::string comp, std::string sourcp, sf::Image* imag) {


	G->generating = true;

	G->Stage = "Starting...";
	loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 15, loadingbar.getSize().y));

	try {
		sprite_vector clones(G->maxclones);
	}
	catch (std::string e) {
		string z = "Error! > Unhandled Exception: " + e;
		LPCTSTR lp = (LPCTSTR)z.c_str();
		MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
		return ERROR_VEC_INIT;
	}

	sprite_vector clones(G->maxclones); //init vector for clones

	

	int i = 1;
	int lw, lh;
	int cw, ch;


	int ij = 1;

	sf::Color avg1, avg2;

	sf::Image source;
	

	if (!source.loadFromFile(sourcp)) {
		return ERROR_SOURCE;
	}

	parsing::GetImageSize(sourcp.c_str(), &G->sw, &G->sh);

	if (G->sw == 0 || G->sh == 0) {
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

	G->Stage = "Getting images...";
	loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 3, loadingbar.getSize().y));
	
	
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

				G->files[i] = pathh;

			}
			else {
				unsigned int width, height;
				std::string pathh1 = entry.path().u8string();
				const char* pth1 = pathh1.c_str();

				parsing::GetImageSize(pth1, &cw, &ch);

				G->files[i] = pathh1;
				
				G->Stage = "Getting images... " + std::to_string(i);
				loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 3 + 0.17 * (i / MAX_INPUT), loadingbar.getSize().y));
			}

		}
		i++;
		if (G->abortE) {
			G->Stage = "Aborted";
			loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
			G->abortE = false;
			return ERROR_SUCCESS;
		}
	}


	
	G->Stage = "Parsing textures...";
	loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 2, loadingbar.getSize().y));
	

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

	bool isDif = false;

	for (int g = 0; g < MAX_INPUT; g++)
	{ if (G->lastfiles[g] != G->files[g]) isDif = true; break; }

	if (!G->firststt) {
		if (isDif || G->laspectr != aspectr) {
			for (j; j < i; j++) {
				
				if (!G->Textures[j].loadFromFile(G->files[j], sf::IntRect(0, 0, lw, lh))) {
					std::string yeet = std::string("Error while parsing textures. Error at: " + G->files[j] + "\nDo you have enough RAM?");
					LPCTSTR lp = (LPCTSTR)yeet.c_str();
					MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
					G->Stage = "Error";
					loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
					
					return 0;
				}
				if (!G->images[j].loadFromFile(G->files[j])) {
					std::string yeet = ("Error while creating buffer image. Error at: " + G->files[j] + "\nDo you have enough RAM?");
					LPCTSTR lp = (LPCTSTR)yeet.c_str();
					MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
					G->Stage = "Error";
					loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
					
					return 0;
				}
				
				sf::Color c1, c2, c3, c4;

				int ix, iy;

				std::string pathh = G->files[j];
				const char* pth = pathh.c_str();

				parsing::GetImageSize(pth, &ix, &iy);

				c1 = G->images[j].getPixel((int)ix / 2, (int)iy / 2); //center
				c2 = G->images[j].getPixel((int)ix / 2, (int)iy / 4); //top center
				c3 = G->images[j].getPixel((int)ix / 4, (int)iy * 0.75); //left lower
				c4 = G->images[j].getPixel((int)ix * 0.75, (int)iy * 0.75); //right lower

				if (dScan) {
					int pp;
					double r, g, b = 0;
					float ir, ig, ib;

					for (int p = 0; p < MAX_SAMPLES; p++) {
						if (p >= ix * iy) break;
						G->dScan_cols[p] = G->images[j].getPixel((int)p % ix, (int)floor(p / ix));
						r += G->dScan_cols[p].r;
						g += G->dScan_cols[p].g;
						b += G->dScan_cols[p].b;
						pp = p;
					}
					ir = r / pp;
					ig = g / pp;
					ib = b / pp;

					G->avg[j] = sf::Color(ir, ig, ib);
				}
				else {
					avg1 = sf::Color((c1.r + c2.r) / 2, (c1.g + c2.g) / 2, (c1.b + c2.b) / 2);
					avg2 = sf::Color((c3.r + c4.r) / 2, (c3.g + c4.g) / 2, (c3.b + c4.b) / 2);

					G->avg[j] = sf::Color((avg1.r + avg2.r) / 2, (avg1.g + avg2.g) / 2, (avg1.b + avg2.b) / 2);
				}


				
				G->Stage = "Parsing composite... " + std::to_string(j + 1) + " of " + std::to_string(i + 1);
				loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 2 + 0.08 * (j / i), loadingbar.getSize().y));

				if (G->abortE) {
					G->Stage = "Aborted";
					loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
					G->abortE = false;
					
					return ERROR_SUCCESS;
				}
				
			}
			for (int g = 0; g < MAX_INPUT; g++)
				G->lastfiles[g] = G->files[g];

			G->laspectr = aspectr;
		}
	}
	else {
		for (j; j < i; j++) {
			if (!G->Textures[j].loadFromFile(G->files[j], sf::IntRect(0, 0, lw, lh))) {
				std::string yeet = std::string("Error while parsing textures. Error at: " + G->files[j] + "\nDo you have enough RAM?");
				LPCTSTR lp = (LPCTSTR)yeet.c_str();
				MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
				G->Stage = "Error";
				loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
				return 0;
			}
			if (!G->images[j].loadFromFile(G->files[j])) {
				std::string yeet = ("Error while creating buffer image. Error at: " + G->files[j] + "\nDo you have enough RAM?");
				LPCTSTR lp = (LPCTSTR)yeet.c_str();
				MessageBox(nullptr, lp, TEXT("Fatal Error"), MB_OK);
				G->Stage = "Error";
				loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
				return 0;
			}
			sf::Color c1, c2, c3, c4;

			int ix, iy;

			std::string pathh = G->files[j];
			const char* pth = pathh.c_str();

			parsing::GetImageSize(pth, &ix, &iy);

			c1 = G->images[j].getPixel((int)ix / 2, (int)iy / 2); //center
			c2 = G->images[j].getPixel((int)ix / 2, (int)iy / 4); //top center
			c3 = G->images[j].getPixel((int)ix / 4, (int)iy * 0.75); //left lower
			c4 = G->images[j].getPixel((int)ix * 0.75, (int)iy * 0.75); //right lower

			if (dScan) {
				int pp;
				double r, g, b = 0;
				float ir, ig, ib;

				for (int p = 0; p < MAX_SAMPLES; p++) {
					if (p >= ix * iy) break;
					G->dScan_cols[p] = G->images[j].getPixel((int)p % ix, (int)floor(p / ix));
					r += G->dScan_cols[p].r;
					g += G->dScan_cols[p].g;
					b += G->dScan_cols[p].b;
					pp = p;
				}
				ir = r / pp;
				ig = g / pp;
				ib = b / pp;

				G->avg[j] = sf::Color(ir, ig, ib);
			}
			else {
				avg1 = sf::Color((c1.r + c2.r) / 2, (c1.g + c2.g) / 2, (c1.b + c2.b) / 2);
				avg2 = sf::Color((c3.r + c4.r) / 2, (c3.g + c4.g) / 2, (c3.b + c4.b) / 2);

				G->avg[j] = sf::Color((avg1.r + avg2.r) / 2, (avg1.g + avg2.g) / 2, (avg1.b + avg2.b) / 2);
			}


			
			G->Stage = "Parsing composite... " + std::to_string(j + 1) + " of " + std::to_string(i + 1);
			loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 2 + 0.08 * (j / i), loadingbar.getSize().y)); //something does not work :thonk:
			if (G->abortE) {
				G->Stage = "Aborted";
				loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
				G->abortE = false;
				
				return ERROR_SUCCESS;
			}
			
		}
		G->firststt = false;

		for(int g = 0; g < MAX_INPUT; g++)
			G->lastfiles[g] = G->files[g];

		G->laspectr = aspectr;
	}

	

	for (ij; ij < i; ij++) {
		G->sprites[ij].setTexture(G->Textures[ij]);
		G->sprites[ij].setScale(scale, scale);

		
		G->Stage = "Assigning sprites... " + std::to_string(ij + 1) + " of " + std::to_string(i + 1);
		loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 1.7f, loadingbar.getSize().y));
		if (G->abortE) {
			G->Stage = "Aborted";
			loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
			G->abortE = false;
			
			return ERROR_SUCCESS;
		}
		
	}
	
	G->Stage = "Initializing source...";
	loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 1.6f, loadingbar.getSize().y));
	

	//get viewport info
	HWND hwnd;
	MONITORINFO moninfo;
	HMONITOR monitor;

	hwnd = FindWindow(NULL, TEXT("Mosaic 2"));

	monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);

	if (monitor != NULL) {
		moninfo.cbSize = sizeof(moninfo);
		GetMonitorInfoA(monitor, &moninfo);

		G->viewportX = moninfo.rcMonitor.right - moninfo.rcMonitor.left;
		G->viewportY = moninfo.rcMonitor.bottom - moninfo.rcMonitor.top;

		if (G->sw > G->viewportX || G->sh > G->viewportY) {
			std::string z = std::string("The source image is bigger than your current viewport. The full view will *not* fit on the screen. To exit, press ESC.\n\nDo not panic! Everything will be fine.\n\n(viewport size: " + std::to_string(G->viewportX) + "x" + std::to_string(G->viewportY) + ", source size: " + std::to_string(G->sw) + "x" + std::to_string(G->sh) + ")");
			LPCTSTR lp = (LPCTSTR)z.c_str();
			MessageBox(nullptr, lp, TEXT("Warning!"), MB_OK);
		}
	}
	
	G->Stage = "Generating the image...";
	loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 1.4f, loadingbar.getSize().y));
	

	int cumter = 0;
	bool fixSkip = true;

	for (int x = 0; x < G->maxclones; x++) {

		//20.09.2019 - Jesus fucking christ thanks younger me for not hardcoding a thing in here, holy shit saved me a lot of time
		//           - Doing clone free aspect btw

		if (!fixSkip) {
			if (rowX * scale * lw > G->sw) {
				rowX = 0;
				rowY++;
			}

			//we have correct row and column here already.

			int boxX = rowX * scale * lw;
			int boxY = rowY * scale * lh;

			if (boxX + 0.5 * scale * lw > G->sw || boxY + 0.5 * scale * lh > G->sh) {
				if (boxX < G->sw && boxY < G->sh) {
					measure = source.getPixel((int)(boxX), (int)(boxY));
				}
				else if ((rowY + 1) * scale * lh < G->sh) {
					measure = source.getPixel((int)(0), (int)((rowY + 1) * scale * lh));
				}
				else {
					break;
				}

			}
			else {
				measure = source.getPixel((int)(boxX + 0.5 * scale * lw), (int)(boxY + 0.5 * scale * lh));
			}

			if (G->noise > 0) {
				clones[counter] = G->sprites[Helpers::getNearest3D(sf::Color(measure.r + rand() % (int)G->noise - G->noise / 2, measure.g + rand() % (int)G->noise - G->noise / 2, measure.b + rand() % (int)G->noise - G->noise / 2), i)];
			}
			else {
				clones[counter] = G->sprites[Helpers::getNearest3D(measure, i)];
			}

			clones[counter].setPosition(rowX * scale * lw, rowY * scale * lh);

			rowX++;
			counter++;
			cumter++;

			if (cumter > 586) {
				
				G->Stage = "Generating the image... " + std::to_string(x + 1) + "/" + std::to_string(G->maxclones);
				loadingbar.setSize(sf::Vector2f(BAR_X * 0.75 * 1 / 1.4f + 0.29 * (x / G->maxclones), loadingbar.getSize().y));

				cumter = 0;
				
			}
			if (G->abortE) {
				G->Stage = "Aborted";
				loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));
				G->abortE = false;
				return ERROR_SUCCESS;
			}
		}
		else {
			measure = source.getPixel((int)(1), (int)(1));
			clones[counter] = G->sprites[Helpers::getNearest3D(measure, i)];
			clones[counter].setPosition(0, 0);
			fixSkip = false;
			counter++;
			//yay another shit fix
		}


	}

	G->genWin.create(sf::VideoMode(G->sw, G->sh), "Generating", sf::Style::None);
	G->genWin.setPosition(sf::Vector2i(10000, 10000));

	int render = 0;
	int countere = 0;
	
	G->Stage = "Generated!";
	loadingbar.setSize(sf::Vector2f(BAR_X, loadingbar.getSize().y));


	

	
	while (G->genWin.isOpen()) {
		if (render == 3) { //SFML and double-buffering, so 3
			sf::Image capture;
			capture = G->genWin.capture();
			*imag = capture;
			G->result = capture;
			G->genWin.close();
		}
		G->genWin.clear();
		for (int x = 1; x < counter; x++) {
			G->genWin.draw(clones[x]);
		}
		if (counter > G->maxclones - 2) {
			G->genWin.draw(G->notenough);
		}
		G->genWin.display();
		render++;
	}

	G->generated = true;
	G->generating = false;

	

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

		G->viewportX = moninfo.rcMonitor.right - moninfo.rcMonitor.left;
		G->viewportY = moninfo.rcMonitor.bottom - moninfo.rcMonitor.top;
	}


	G->saveBut.w = 100;
	G->saveBut.h = 30;
	G->saveBut.x = G->sw / 2 - 50;
	if (G->sh > G->viewportY) {
		G->saveBut.y = G->viewportY - 70 + (G->sh - G->viewportY)/2;
	}
	else {
		G->saveBut.y = G->sh - 70;

		
	}
	G->saveBut.callbackID = NONE;
	G->saveBut.caption = "Save as...";

	G->saveButD.bg.setSize(sf::Vector2f(G->saveBut.w, G->saveBut.h));
	G->saveButD.bg.setPosition(sf::Vector2f(G->saveBut.x, G->saveBut.y));
	G->saveButD.bg.setFillColor(sf::Color(74, 74, 74));
	G->saveButD.fg.setSize(sf::Vector2f(G->saveBut.w - 4, G->saveBut.h - 4));
	G->saveButD.fg.setPosition(sf::Vector2f(G->saveBut.x + 2, G->saveBut.y + 2));
	G->saveButD.fg.setFillColor(sf::Color(85, 85, 85));
	G->saveButD.text.setFont(*font);
	G->saveButD.text.setString("Save as...");
	G->saveButD.text.setCharacterSize(16);
	G->saveButD.text.setFillColor(sf::Color::White);
	G->saveButD.text.setPosition(G->saveBut.w / 2 - G->saveButD.text.getLocalBounds().width / 2 + G->saveBut.x, G->saveBut.h / 2 - G->saveButD.text.getLocalBounds().height / 2 + G->saveBut.y - 2);
	

	G->saveBut.drawable = &G->saveBut;
}

void rendSave() {
	int mx, my;
	Helpers::GetCursorToWindow(&mx, &my, &G->genWin);

	int x, y, w, h;
	x = G->saveBut.x;
	y = G->saveBut.y;
	w = G->saveBut.w;
	h = G->saveBut.h;

	if (x < mx && mx < x + w && y < my && my < y + h) {  //hover
		G->genWin.draw(G->saveButD.bg);
		G->saveButD.fg.setFillColor(sf::Color(149, 149, 149));
		G->genWin.draw(G->saveButD.fg);
		G->genWin.draw(G->saveButD.text);
	}
	else {
		G->genWin.draw(G->saveButD.bg);
		G->saveButD.fg.setFillColor(sf::Color(85, 85, 85));
		G->genWin.draw(G->saveButD.fg);
		G->genWin.draw(G->saveButD.text);
	}
}

void SaveAs() {
	butoncallbacks(OpenFileDialogC, nullptr, nullptr, nullptr, nullptr);
}



void Generate::doView(sf::Font* font) {

	G->genWin.create(sf::VideoMode(G->sw, G->sh), "Preview", sf::Style::None);

	G->prevT2.loadFromImage(G->result);
	G->prevS2.setColor(sf::Color(255, 255, 255, 200));
	G->prevS2.setTexture(G->prevT2);
	G->prevS2.setTextureRect(sf::IntRect(0, 0, G->sw, G->sh));

	setupSave(font);

	sf::Image icon;
	if (icon.loadFromFile("resource/icon.png")) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	int wnx, wny;
	HWND window = FindWindow(NULL, TEXT("Preview"));
	SetForegroundWindow(window);
	GetWindowPos(&wnx, &wny, &G->genWin);
	SetWindowPos(window, HWND_TOPMOST, wnx, wny, G->sw, G->sh, SWP_NOMOVE);

	while (G->genWin.isOpen()) {

		sf::Event event;
		while (G->genWin.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				G->genWin.close();
			if (event.type == sf::Event::LostFocus)
				G->genWin.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					G->genWin.close();
				}
			}
			if (event.type == event.MouseButtonPressed) {
				int mx, my;
				Helpers::GetCursorToWindow(&mx, &my, &G->genWin);

				int x, y, w, h;
				x = G->saveBut.x;
				y = G->saveBut.y;
				w = G->saveBut.w;
				h = G->saveBut.h;

				if (x < mx && mx < x + w && y < my && my < y + h) {  //hover
					SaveAs();
					if (savepath != "ERROR") {
						std::stringstream filename;
						filename << savepath;
						G->result.saveToFile(filename.str());
					}
				}
			}
		}
		G->genWin.clear();
		G->genWin.draw(G->prevS2);
		G->genWin.draw(prevWarn);
		rendSave();
		G->genWin.display();
	}
}

void Generate::abort() {
	G->abortE = true;
	return;
}

extern std::string sourcepath;
extern std::string compositepath;
extern bool threadJoined;
extern sf::Image rendered;

void Generate::Thread(sf::Font* pfont) {
	while (0x14) {
		switch (G->request) {
		case generate_image:
			if(compositepath != "" && sourcepath != "")
				Generate::generateImage(compositepath, sourcepath, &G->renderResult);
			rendered = G->renderResult;
			G->request = empty;
			break;
		case show_window:
			Generate::doView(pfont);
			G->request = empty;
			break;
		case abort_render:
			Generate::abort();
			G->request = empty;
			break;
		case exit_thread:
			return;
			break;
		default:
			G->request = empty;
			break;
		}

		threadJoined = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void Generate::makeRequest(int rq) {
	G->request = rq;
	return;
}