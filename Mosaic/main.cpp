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

//--------------------------//
//total recode since 2.0, yay
//--------------------------//


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

extern bool generating;

int lmx, lmy;
bool dragging = false;
int what = errortype;
int whatid = -1;
BOOL shader = false;

sf::RenderWindow* pWind = &window;
sf::Font* pFont = &font;
high_resolution_clock::time_point timeorigin;
high_resolution_clock::time_point frame;

static bool terminateB = false;

void Helpers::Terminate() {
	terminateB = true;
	return;
}

int main(int argc, char* argv[]) {


	//check for running mosaic
	HWND isLaunched = NULL;
	HMONITOR monitor;
	isLaunched = FindWindow(NULL, TEXT("Mosaic 2"));
	monitor = MonitorFromWindow(isLaunched, MONITOR_DEFAULTTONULL);
	if (monitor != NULL) {
		SetForegroundWindow(isLaunched);
		return ERROR;
	}

	if (sf::Shader::isAvailable()) {
		shader = true;
	}

	pWind->create(sf::VideoMode(WINDX, WINDY), "Mosaic 2", sf::Style::None);
	pWind->setFramerateLimit(240);

	MARGINS margins;
	margins.cxLeftWidth = -1;

	SetWindowLong(pWind->getSystemHandle(), GWL_STYLE, WS_POPUP | WS_VISIBLE);
	DwmExtendFrameIntoClientArea(pWind->getSystemHandle(), &margins);


	if (!pFont->loadFromFile("resource/font.ttf")) {
		MessageBox(nullptr, "Couldn't load font", TEXT("Fatal Error"), MB_OK);
		return ERROR;
	}

	sf::Image icon;
	if (icon.loadFromFile("resource/icon.png")) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}

	//setup all
	Render::SetParams(pFont);
	Render::InitUI(pFont);
	if (shader) Render::InitShader();
	
	while (pWind->isOpen()) {   //main window loop

		if (terminateB) {
			return 0;
			//end program
		}

		timeorigin = high_resolution_clock::now();
		sf::Event event;
		while (pWind->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				pWind->close();
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					int id, type;
					type = Helpers::GetClickedTarget(&id, pWind);
					int mx, my;
					Helpers::GetCursorToWindow(&mx, &my, pWind);

					switch (type) {
					case topbar:
						//move window
						dragging = true;
						what = topbar;
						break;
					case button:
						//buttonclick
						butoncallbacks(reinterpret_cast<CButton*>(getPointerA(button, id))->callbackID, pFont, pWind, nullptr);
						break;
					case exitbutton:
						//exit button
						butoncallbacks(reinterpret_cast<CExit*>(getPointerA(exitbutton, id))->callback, pFont, pWind, nullptr);
						break;
					case slider:
						//slider
						dragging = true;
						what = slider;
						whatid = id;
						break;
					}
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					dragging = false;
				}
			}
		}

		if (dragging && what != errortype) {
			switch (what) {
			case topbar:
				int mx, my;
				Helpers::GetCursorToWindow(&mx, &my, pWind);

				int wx, wy;
				GetWindowPos(&wx, &wy, pWind);

				if (lmx == 0 || lmy == 0) {
					lmx = mx;
					lmy = my;
				}

				pWind->setPosition(sf::Vector2i(wx + (mx - lmx), wy + (my - lmy)));
				break;
			case slider:
				Helpers::GetCursorToWindow(&mx, &my, pWind);
				float rs = calcSlider(mx, my, whatid);
				setSliderVal(whatid, rs);
				break;
			}
		}
		else {
			lmx = lmy = 0;
			what = errortype;
			whatid = -1;
		}


		

		pWind->clear(sf::Color::Transparent);
		Render::GUI(pWind);

		frame = high_resolution_clock::now();
		std::chrono::steady_clock::duration delta = frame - timeorigin;

		Render::DebugInfo(pWind, (double)((double)delta.count()/ (double)1000000000));
		pWind->display();
		generating = false;

	}
	return ERROR_SUCCESS;
}




