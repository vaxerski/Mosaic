#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <SFML\Graphics.hpp>
#include <string>
#include <dwmapi.h>
#include <unknwn.h>
#include <gdiplus.h>
#pragma comment( lib, "gdiplus" )


#include "render.h"
#include "controls.h"
#include "helpers.h"
#include "Generate.h"
#include "parsing.h"

std::string sourcepath;
std::string compositepath;
std::string savepath;
float scale = 0.01f;
sf::Image rendered;

extern long long maxclones;
extern bool generated;
extern bool generating;
extern int sw, sh;
extern float noise;

//creating

void creating::CreateButton(int x, int y, int w, int h, std::string caption, sf::Font* font, int callback, bool renderable) {
	

	Buttons[buts].x = x;
	Buttons[buts].y = y;
	Buttons[buts].w = w;
	Buttons[buts].h = h;
	Buttons[buts].callbackID = callback;
	Buttons[buts].caption = caption;
	Buttons[buts].render = renderable;

	ButtonsD[buts].bg.setSize(sf::Vector2f(w, h));
	ButtonsD[buts].bg.setPosition(sf::Vector2f(x, y));
	ButtonsD[buts].bg.setFillColor(sf::Color(74, 74, 74));
	ButtonsD[buts].fg.setSize(sf::Vector2f(w - 4, h - 4));
	ButtonsD[buts].fg.setPosition(sf::Vector2f(x + 2, y + 2));
	ButtonsD[buts].fg.setFillColor(sf::Color(85, 85, 85));
	ButtonsD[buts].text.setFont(*font);
	ButtonsD[buts].text.setString(Buttons[buts].caption);
	ButtonsD[buts].text.setCharacterSize(16);
	ButtonsD[buts].text.setFillColor(sf::Color::White);
	ButtonsD[buts].text.setPosition(w / 2 - ButtonsD[buts].text.getLocalBounds().width / 2 + x, h / 2 - ButtonsD[buts].text.getLocalBounds().height / 2 + y - 2);


	Buttons[buts].drawable = &ButtonsD[buts];

	buts++;
	
}

void creating::CreateExit(sf::Font* font) {
	ExitB.bg.setFillColor(sf::Color(255, 0, 0, 150));
	ExitB.bg.setPosition(sf::Vector2f(WINDX - 40, 20));
	ExitB.bg.setSize(sf::Vector2f(20, 20));
	ExitB.callback = CallExit;
	ExitB.x.setFont(*font);
	ExitB.x.setString("X");
	ExitB.x.setCharacterSize(14);
	ExitB.x.setFillColor(sf::Color::White);
	ExitB.x.setPosition(20 / 2 - ExitB.x.getLocalBounds().width / 2 + WINDX-40, 20 / 2 - ExitB.x.getLocalBounds().height / 2 + 18);
}

void creating::CreateSlider(int x, int y, int w, std::string caption, sf::Font* font, bool locked, float def, float min, float max) {


	Sliders[slids].x = x;
	Sliders[slids].y = y;
	Sliders[slids].w = w;
	Sliders[slids].caption = caption;
	Sliders[slids].locked = locked;
	Sliders[slids].val = def;
	Sliders[slids].min = min;
	Sliders[slids].max = max;

	SlidersD[slids].bar.setSize(sf::Vector2f(w, 5));
	SlidersD[slids].bar.setPosition(sf::Vector2f(x, y));
	SlidersD[slids].bar.setFillColor(sf::Color(180,180,180,150));

	SlidersD[slids].zip.setSize(sf::Vector2f(5, 10));
	SlidersD[slids].zip.setPosition(sf::Vector2f(def / max * w + x, y - 2));
	SlidersD[slids].zip.setFillColor(sf::Color(180, 180, 180, 230));

	SlidersD[slids].text.setFont(*font);
	SlidersD[slids].text.setString(caption);
	SlidersD[slids].text.setFillColor(sf::Color(200, 200, 200, 255));
	SlidersD[slids].text.setCharacterSize(12);
	SlidersD[slids].text.setPosition(sf::Vector2f(w / 2 - SlidersD[slids].text.getLocalBounds().width / 2 + x, 5 / 2 - SlidersD[slids].text.getLocalBounds().height / 2 + y - 10));

	SlidersD[slids].text2.setFont(*font);
	SlidersD[slids].text2.setString(std::to_string(def));
	SlidersD[slids].text2.setFillColor(sf::Color(200, 200, 200, 255));
	SlidersD[slids].text2.setCharacterSize(12);
	SlidersD[slids].text2.setPosition(sf::Vector2f(w / 2 - SlidersD[slids].text2.getLocalBounds().width / 2 + x, 5 / 2 - SlidersD[slids].text2.getLocalBounds().height / 2 + y + 8));


	Sliders[slids].drawable = &SlidersD[slids];

	slids++;

}
//drawing

int drawing::drawButton(sf::RenderWindow* pwind, int id) {

	if (id >= buts)
		return -1;
	if (Buttons[id].render == false) return 1;

	int mx, my;
	Helpers::GetCursorToWindow(&mx, &my, pwind);

	int x, y, w, h;
	x = Buttons[id].x;
	y = Buttons[id].y;
	w = Buttons[id].w;
	h = Buttons[id].h;

	if (x < mx && mx < x + w && y < my && my < y + h) {  //hover
		if (id == 2) {
			if (!generating) {
				ButtonsD[id].text.setString("Generate!");
				ButtonsD[id].text.setPosition(w / 2 - ButtonsD[id].text.getLocalBounds().width / 2 + x, h / 2 - ButtonsD[id].text.getLocalBounds().height / 2 + y);
			}
			else { 
				ButtonsD[id].text.setString("Abort"); 
		    	ButtonsD[id].text.setPosition(w / 2 - ButtonsD[id].text.getLocalBounds().width / 2 + x, h / 2 - ButtonsD[id].text.getLocalBounds().height / 2 + y);
			}
		}
		pwind->draw(ButtonsD[id].bg);
		ButtonsD[id].fg.setFillColor(sf::Color(149, 149, 149));
		pwind->draw(ButtonsD[id].fg);
		pwind->draw(ButtonsD[id].text);
	}
	else {
		if (id == 2) {
			if (!generating) {
				ButtonsD[id].text.setString("Generate!");
				ButtonsD[id].text.setPosition(w / 2 - ButtonsD[id].text.getLocalBounds().width / 2 + x, h / 2 - ButtonsD[id].text.getLocalBounds().height / 2 + y);
			}
			else {
				ButtonsD[id].text.setString("Abort");
				ButtonsD[id].text.setPosition(w / 2 - ButtonsD[id].text.getLocalBounds().width / 2 + x, h / 2 - ButtonsD[id].text.getLocalBounds().height / 2 + y);
			}
		}
		pwind->draw(ButtonsD[id].bg);
		ButtonsD[id].fg.setFillColor(sf::Color(85, 85, 85));
		pwind->draw(ButtonsD[id].fg);
		pwind->draw(ButtonsD[id].text);
	}



	return 1;
}

int drawing::drawSlider(sf::RenderWindow* pwind, int id) {

	if (id >= slids)
		return -1;

	std::string temp = std::to_string(Sliders[id].val);

	char dot = (char)".";

	int mx, my;
	Helpers::GetCursorToWindow(&mx, &my, pwind);

	int x, y, w, h;
	x = Sliders[id].x;
	y = Sliders[id].y;
	w = Sliders[id].w;
	h = 15;

	if (x < mx && mx < x + w && y < my && my < y + h) {  //hover
		pwind->draw(SlidersD[id].bar);
		SlidersD[id].zip.setFillColor(sf::Color(180, 180, 180));
		SlidersD[id].zip.setPosition(sf::Vector2f(Sliders[id].val / Sliders[id].max * w + x, y - 2));
		
		if (id == 1) {
			temp.erase(temp.find_last_of('.'), std::string::npos);
		}
		else {
			temp.erase(temp.find_last_not_of('0') + 1, std::string::npos);
			if (temp.back() == (char)0x2E) temp.erase(temp.find_last_of("."));  //0x2E is a dot. 
		}
		SlidersD[id].text2.setString(temp);
		SlidersD[id].text2.setPosition(sf::Vector2f(w / 2 - SlidersD[id].text2.getLocalBounds().width / 2 + x, 5 / 2 - SlidersD[id].text2.getLocalBounds().height / 2 + y + 8));
		pwind->draw(SlidersD[id].zip);
		pwind->draw(SlidersD[id].text);
		pwind->draw(SlidersD[id].text2);
	}
	else {
		pwind->draw(SlidersD[id].bar);
		SlidersD[id].zip.setFillColor(sf::Color(150, 150, 150, 150));
		SlidersD[id].zip.setPosition(sf::Vector2f(Sliders[id].val / Sliders[id].max * w + x, y - 2));
		
		if (id == 1) {
			temp.erase(temp.find_last_of('.'), std::string::npos);
		}
		else {
			temp.erase(temp.find_last_not_of('0') + 1, std::string::npos);
			if (temp.back() == (char)0x2E) temp.erase(temp.find_last_of("."));  //0x2E is a dot. 
		}
		SlidersD[id].text2.setString(temp);
		SlidersD[id].text2.setPosition(sf::Vector2f(w / 2 - SlidersD[id].text2.getLocalBounds().width / 2 + x, 5 / 2 - SlidersD[id].text2.getLocalBounds().height / 2 + y + 8));
		pwind->draw(SlidersD[id].zip);
		pwind->draw(SlidersD[id].text);
		pwind->draw(SlidersD[id].text2);
	}



	return 1;
}

int drawing::drawExit(sf::RenderWindow* pwind) {
	int mx, my;
	Helpers::GetCursorToWindow(&mx, &my, pwind);

	int x, y, w, h;
	x = WINDX - 40;
	y = 20;
	w = 20;
	h = 20;

	if (x < mx && mx < x + w && y < my && my < y + h) {  //hover
		ExitB.bg.setFillColor(sf::Color(255, 0, 0, 200));
		pwind->draw(ExitB.bg);
		pwind->draw(ExitB.x);
	}
	else {
		ExitB.bg.setFillColor(sf::Color(255, 0, 0, 150));
		pwind->draw(ExitB.bg);
		pwind->draw(ExitB.x);
	}

	return 1;
}

int Helpers::GetClickedTarget(int* id, sf::RenderWindow* pwnd) {
	int mx, my;
	Helpers::GetCursorToWindow(&mx, &my, pwnd);
	int x, y, w, h;
	//buttons
	for (int i = 0; i < BUTTONS; i++) {
		
		x = Buttons[i].x;
		y = Buttons[i].y;
		w = Buttons[i].w;
		h = Buttons[i].h;

		if (x < mx && mx < x + w && y < my && my < y + h) {  //on it
			*id = i;
			return button;
		}
	}

	//exit

	x = WINDX - 40;
	y = 20;
	w = 20;
	h = 20;

	if (x < mx && mx < x + w && y < my && my < y + h) {  //on it
		*id = 0;
		return exitbutton;
	}

	//topbar
	x = 20;
	y = 20;
	w = 1280;
	h = 20;

	if (x < mx && mx < x + w && y < my && my < y + h) {  //on it
		*id = 0;
		return topbar;
	}

	Helpers::GetCursorToWindow(&mx, &my, pwnd);

	//sliders
	for (int j = 0; j < SLIDERS; j++) {
		

		int x, y, w, h;
		x = Sliders[j].x;
		y = Sliders[j].y;
		w = Sliders[j].w;
		h = 15;

		if (x < mx && mx < x + w && y < my && my < y + h) {  //hover
			*id = j;
			return slider;
		}
	}

	return errortype;
}

void* getPointerA(int type, int id) {
	switch (type) {
	case button:
		return &Buttons[id];
		break;
	case exitbutton:
		return &ExitB;
		break;
	case slider:
		return &Sliders[id];
		break;
	}
}

void setrenderable(int id, bool set) {
	Buttons[id].render = set;
}

void dispatchPath(std::string path, int id) {
	if (id == OpenFileDialogA) {
		sourcepath = path;

		int sew, seh;
		parsing::GetImageSize(sourcepath.c_str(), &sew, &seh);

		sw = sew;
		sh = seh;
	}
	else if (id == OpenFileDialogB) {
		compositepath = path;

	}
	else if (id == OpenFileDialogC) {
		try {
			savepath = path;
		}
		catch (...) {
			MessageBox(nullptr, TEXT("Couldn't save the file."), TEXT("Error"), MB_OK);
			savepath = "ERROR";
		}
		
	}
}

sf::Text texto;

void butoncallbacks(int id, sf::Font* arg1, sf::RenderWindow* arg2, void* arg3) {
	std::string path;
	switch (id) {
	case NONE:
	    //none
		break;
	case OpenFileDialogA:
		path = Helpers::openfilename(0);
		dispatchPath(path, OpenFileDialogA);
		break;
	case OpenFileDialogB:
		path = Helpers::openfilename(1);
		dispatchPath(path, OpenFileDialogB);
		break;
	case CallExit:
		Helpers::Terminate();
		break;
	case CallGenerate:
		//generate the image
		if (!generating)Generate::generateImage(compositepath, sourcepath, &rendered);
		else Generate::abort();
		Render::SetPrev();
		break;
	case CallViewRender:
		Generate::doView(arg1);
		break;
	case OpenFileDialogC:
		path = Helpers::openfilename(2);
		dispatchPath(path, OpenFileDialogC);
		break;
	}
}

float calcSlider(int mx, int my, int id) {
	int x = Sliders[id].x;
	int w = Sliders[id].w;

	float max = Sliders[id].max;
	float min = Sliders[id].min;

	float fraction = (mx - x) / (float)w;

	if (fraction * max > max) {
		return (float)max;
	}
	else if (fraction * max < min) {
		return (float)min;
	}

	return (float)(fraction * max);
}

void setSliderVal(int id, float val) {
	Sliders[id].val = val;
	if (id == 0) {
		scale = val;
		if (val < 0.001f) {
			SlidersD[id].text2.setFillColor(sf::Color(255, 30, 30));
		}
		else {
			SlidersD[id].text2.setFillColor(sf::Color(255, 255, 255));
		}
	}
	else if (id == 1) {
		maxclones = (long long)val;
		Sliders[id].val = (long long)val;
	}
	else if (id == 2) {
		noise = val;
		Sliders[id].val = val;
	}
}