#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>

#define BUTTONS         20
#define SLIDERS         20

namespace creating {
	void CreateButton(int x, int y, int w, int h, std::string caption, sf::Font* font, int callback, bool renderable);
	void CreateExit(sf::Font* font);
	void CreateSlider(int x, int y, int w, std::string caption, sf::Font* font, bool locked, float def, float min, float max);
}

struct CButton {
	int type = 1;
	int x, y;
	int w, h;
	std::string caption;
	int callbackID;
	void* drawable;
	bool render;
};

struct CButtonDrawable {
	sf::RectangleShape bg;
	sf::RectangleShape fg;
	sf::Text text;
};

struct CExit {
	sf::RectangleShape bg;
	sf::Text x;
	int callback;
};

struct CSlider {
	float min;
	float max;
	int x, y;
	int w;
	bool locked;
	float val;
	void* drawable;
	std::string caption;
};

struct CSliderD {
	sf::RectangleShape bar;
	sf::RectangleShape zip;
	sf::Text text;
	sf::Text text2;
};

namespace drawing {
	int drawButton(sf::RenderWindow* pwind, int id);
	int drawSlider(sf::RenderWindow* pwind, int id);
	int drawExit(sf::RenderWindow* pwind);
}


//storage
static CButton Buttons[BUTTONS];
static CButtonDrawable ButtonsD[BUTTONS];
static CExit ExitB;
static CSlider Sliders[SLIDERS];
static CSliderD SlidersD[SLIDERS];



void* getPointerA(int type, int id);
void butoncallbacks(int id, sf::Font* arg1, sf::RenderWindow* arg2, void* arg3);
void setrenderable(int id, bool set);
void butoncallbacks(int id, sf::Font* arg1, sf::RenderWindow* arg2, void* arg3);
float calcSlider(int mx, int my, int id);
void setSliderVal(int id, float val);

//vars
static int buts = 0;
static int slids = 0;
static sf::Font font;


//enum

enum UIelements {
	errortype = -1,
	topbar,
	button,
	exitbutton,
	slider
};

enum UIcallbacks {
	NONE,
	OpenFileDialogA,
	OpenFileDialogB,
	OpenFileDialogC,
	CallExit,
	CallGenerate,
	CallViewRender,
};