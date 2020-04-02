#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>

#define BUTTONS         20
#define SLIDERS         20
#define CHECKBOXES      20
#define DROPBOXES       20
#define MAX_ITEMS       10

namespace creating {
	void CreateButton(int x, int y, int w, int h, std::string caption, sf::Font* font, int callback, bool renderable);
	void CreateExit(sf::Font* font);
	void CreateSlider(int x, int y, int w, std::string caption, sf::Font* font, bool locked, float def, float min, float max);
	void CreateCheckbox(int x, int y, int w, std::string text, sf::Font* font, void* callback, bool enabled);
	void CreateDropbox(int x, int y, int items, sf::Font* font, std::string itemst[MAX_ITEMS], int selected, std::string capt);
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

struct CCheckbox {
	int x, y;
	int w;
	void* callback;
	bool enabled;
	std::string caption;
	void* drawable;
};

struct CCheckboxD {
	sf::RectangleShape box;
	sf::Text text;
};

struct CDropBox {
	int x, y;
	int itemsN;
	std::string items[MAX_ITEMS];
	bool open;
	int selected;
	void* drawable;
	std::string capt;
};

struct CDropBoxD  {
	sf::RectangleShape box;
	sf::RectangleShape showSel;
	sf::Text activetext;
	sf::Text itemsT[MAX_ITEMS];
	int items;
	sf::RectangleShape openBox;
	sf::Text capt;
};

namespace drawing {
	int drawButton(sf::RenderWindow* pwind, int id);
	int drawSlider(sf::RenderWindow* pwind, int id);
	int drawExit(sf::RenderWindow* pwind);
	int drawCheckbox(sf::RenderWindow* pwind, int id);
	int drawComboBox(sf::RenderWindow* pwind, int id);
	int drawComboBoxDrop(sf::RenderWindow* pwind, int id);
}


//storage
static CButton Buttons[BUTTONS];
static CButtonDrawable ButtonsD[BUTTONS];
static CExit ExitB;
static CSlider Sliders[SLIDERS];
static CSliderD SlidersD[SLIDERS];
static CCheckbox CheckBoxes[CHECKBOXES];
static CCheckboxD CheckBoxesD[CHECKBOXES];
static CDropBox Dropboxes[DROPBOXES];
static CDropBoxD DropboxesD[DROPBOXES];


void* getPointerA(int type, int id);
void butoncallbacks(int id, sf::Font* arg1, sf::RenderWindow* arg2, void* arg3, void* arg4);
void setrenderable(int id, bool set);
float calcSlider(int mx, int my, int id);
void setSliderVal(int id, float val);

//vars
static int buts = 0;
static int checks = 0;
static int slids = 0;
static int dropbs = 0;
static sf::Font font;


//enum

enum UIelements {
	errortype = -1,
	topbar,
	button,
	exitbutton,
	slider,
	checkbox,
	dropbox
};

enum UIcallbacks {
	NONE,
	OpenFileDialogA,
	OpenFileDialogB,
	OpenFileDialogC,
	CallExit,
	CallGenerate,
	CallViewRender,
	CHECKBOXCALLBACK,
	DropBoxCallBack
};