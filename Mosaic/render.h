#pragma once


#define WINDX        1280
#define WINDY        800


namespace Render {
	void SetParams(sf::Font* font);
	void background(sf::RenderWindow* pwind);
	void GUI(sf::RenderWindow* pwind);
	void DebugInfo(sf::RenderWindow* pwind, double frametime);
	void InitUI(sf::Font*);
	void InitShader();
	void SetPrev();
	void RenderPrev(sf::RenderWindow* pwind);
	void PrevClick(sf::RenderWindow* pwind);
}

static sf::RenderWindow window;
static sf::RectangleShape backgroundShape;
static sf::Text debuginfo;
static sf::Text watermark;

struct Color {
	float r, g, b;
};

