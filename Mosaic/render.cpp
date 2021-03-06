#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <SFML\Graphics.hpp>
#include <string>
#include <dwmapi.h>
#include <unknwn.h>
#include <gdiplus.h>
#pragma comment( lib, "gdiplus" )
#include "controls.h"
#include "render.h"
#include "helpers.h"
#include "parsing.h"
#include <future>
#include "GVars.h"

extern std::mutex mu;

static int framecount = 0;
static int lastframe = -1;

bool dScan;

extern std::string sourcepath;
extern std::string compositepath;
extern sf::Image rendered;
extern CButtonDrawable saveButD;
extern BOOL shader;

sf::Sprite prevS;

bool tt = false;

sf::RectangleShape bgrend;
sf::RectangleShape bgrend2;

sf::RectangleShape loadingbarbg(sf::Vector2f(900, 40));
sf::RectangleShape loadingbar(sf::Vector2f(900, 40));

sf::RectangleShape tabbar;
sf::RectangleShape divider;

sf::Text sourceinfo;

sf::Sprite blur;
sf::Image blurI;

sf::Text prevWarn;

sf::Shader S_gaussian;



Color c = { 255,0,0 };

float R = 0;      
float Gc = 0;     
float B = 255;    
float curCol = 0; 
float speed = 0.1f;  

Color getCur(){
	if (curCol == 0) {
		if (R < 255) {
			R = R + speed;
			B = B - speed;
		}
		else {
			curCol = 1;
			R = 255;
			B = 0;
		}
	}
	else if (curCol == 1) {
		if (Gc < 255) {
			Gc = Gc + speed;
			R = R - speed;
		}
		else {
			curCol = 2;
			Gc = 255;
			R = 0;
		}
	}
	else {
		if (B < 255) {
			B = B + speed;
			Gc = Gc - speed;
		}
		else {
			curCol = 0;
			B = 255;
			Gc = 0;
		}
	}
	Color ret = { R, Gc, B };
	return ret;
}

void Render::SetParams(sf::Font* font) {
	
	backgroundShape.setSize(sf::Vector2f(WINDX - 40, WINDY - 40));
	backgroundShape.setPosition(20, 20);
	backgroundShape.setFillColor(sf::Color(17, 17, 17));
	debuginfo.setFont(*font);
	debuginfo.setCharacterSize(12);
	debuginfo.setFillColor(sf::Color::White);
	debuginfo.setPosition(25, 20);
	watermark.setFont(*font);
	watermark.setCharacterSize(12);
	watermark.setFillColor(sf::Color::White);
	watermark.setPosition(30, 760);
	bgrend.setPosition(50, 70);
	bgrend.setFillColor(sf::Color(180, 180, 180, 100));
	bgrend.setSize(sf::Vector2f(900, 500));
	bgrend2.setPosition(50, 70);
	bgrend2.setFillColor(sf::Color(180, 180, 180, 50));
	bgrend2.setSize(sf::Vector2f(900, 500));
	blur.setPosition(sf::Vector2f(0, 0));
	if (!G->t.loadFromFile("resource/blur.png")) {
		blur.setPosition(100000, 100000);
	}
	if (!blurI.loadFromFile("resource/blur.png")) {
		blur.setPosition(100000, 100000);
	}
	blur.setTexture(G->t);
	blur.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(1320, 840)));
	prevWarn.setFont(*font);
	
	G->loadingcapt.setFont(*font);
	G->loadingcapt.setCharacterSize(24);
	G->loadingcapt.setFillColor(sf::Color(255, 204, 0, 255));
	G->loadingcapt.setOutlineThickness(0.3f);
	G->loadingcapt.setOutlineColor(sf::Color(0, 0, 0, 249));
	
	loadingbarbg.setPosition(sf::Vector2f(50, 600));
	loadingbarbg.setFillColor(sf::Color(50, 50, 50, 60));
	
	loadingbar.setPosition(sf::Vector2f(50, 600));
	loadingbar.setFillColor(sf::Color(57, 239, 115, 255));
	loadingbar.setSize(sf::Vector2f(0, loadingbar.getSize().y));

	tabbar.setSize(sf::Vector2f(1220, 20));  //1280 - 40 (margins) - 20 (exit)
	tabbar.setPosition(20, 20);
	tabbar.setFillColor(sf::Color(10, 10, 10, 200));

	divider.setSize(sf::Vector2f(5, 740));  // 800 - 40 (margins) - 20 (tabbar)
	divider.setPosition(990, 40);
	divider.setFillColor(sf::Color(10, 10, 10, 200));

	sourceinfo.setFont(*font);
	sourceinfo.setCharacterSize(12);
	sourceinfo.setFillColor(sf::Color::White);
	sourceinfo.setPosition(50, 650);
	
	G->notenough.setFont(*font);

	G->overlay.setColor(sf::Color(255, 255, 255, 255));
	G->overlay2.setColor(sf::Color(255, 255, 255, 255));
}

void Render::background(sf::RenderWindow* pwind) {
	pwind->draw(backgroundShape);
}

sf::String sourcei() {
	try {
		return (sf::String)("Source path: " + sourcepath + "\nSource info: " + std::to_string(G->sw) + "x" + std::to_string(G->sh) + " " + sourcepath.substr(sourcepath.find_last_of(".")) + "\nComposite folder path: " + compositepath);
	}
	catch (...) {
		return (sf::String)("Source path: " + sourcepath + "\nSource info: " + std::to_string(G->sw) + "x" + std::to_string(G->sh) + " " + "" + "\nComposite folder path: " + compositepath);
	}
	
}

void Render::GUI(sf::RenderWindow* pwind) {

    //nofunc elements

	G->loadingcapt.setString(G->Stage);
	G->loadingcapt.setPosition(BAR_X / 2 - G->loadingcapt.getLocalBounds().width / 2 + 50, BAR_Y / 2 - G->loadingcapt.getLocalBounds().height / 2 + 598);
	G->notenough.setFillColor(sf::Color(200, 200, 200, 200));
	G->notenough.setCharacterSize(50);
	G->notenough.setString("There was not enough clones to show this image.");
	G->notenough.setPosition(G->sw / 2 - G->notenough.getLocalBounds().width / 2, G->sh / 2 - G->notenough.getLocalBounds().height / 2);

	Color temp = getCur();
	c = temp;
	blur.setColor(sf::Color(c.r, c.g, c.b));
	pwind->draw(blur);
	pwind->draw(backgroundShape);
	pwind->draw(bgrend);
	pwind->draw(loadingbarbg);
	pwind->draw(loadingbar);
	pwind->draw(G->loadingcapt);
	pwind->draw(tabbar);
	pwind->draw(divider);

	sourceinfo.setString(sourcei());

	pwind->draw(sourceinfo);

	//prevwin
	if(!G->generating)
	    Render::RenderPrev(pwind);


	//buttons
	for (int i = 0; i < BUTTONS; i++) {
		if (drawing::drawButton(pwind, i) == -1) break;
	}

	//sliders
	for (int i = 0; i < SLIDERS; i++) {
		if (drawing::drawSlider(pwind, i) == -1) break;
	}

	//checkboxes
	for (int i = 0; i < CHECKBOXES; i++) {
		if (drawing::drawCheckbox(pwind, i) == -1) break;
	}

	//dropboxes
	for (int i = 0; i < DROPBOXES; i++) {
		if (drawing::drawComboBox(pwind, i) == -1) break;
	}

	//drops down drop
	for (int i = 0; i < DROPBOXES; i++) {
		if (drawing::drawComboBoxDrop(pwind, i) == -1) break;
	}

	//exit
	drawing::drawExit(pwind);
}

void Render::DebugInfo(sf::RenderWindow* pwind, double frametime) {

	framecount++;

	int mx, my;
	Helpers::GetCursorToWindow(&mx, &my, pwind);

	if (framecount >= 10) {
		debuginfo.setString("Mosaic 2 - LIDL visuals edition | mx: " + std::to_string(mx) + " my: " + std::to_string(my) + " fps: " + std::to_string((int)((double)1 / frametime)));
		framecount = 0;
		lastframe = (int)((double)1 / frametime);
	}
	else {
		debuginfo.setString("Mosaic 2 - LIDL visuals edition | mx: " + std::to_string(mx) + " my: " + std::to_string(my) + " fps: " + std::to_string(lastframe));
	}

	watermark.setString("Mosaic 2.4. Report bugs/suggestions at GitHub.");
	
	pwind->draw(watermark);
	pwind->draw(debuginfo);
}

void Render::InitUI(sf::Font* font) {
	creating::CreateButton(1010, 100, 240, 30, "Select Source", font, OpenFileDialogA, true);
	creating::CreateButton(1010, 150, 240, 30, "Select Composite", font, OpenFileDialogB, true);
	creating::CreateButton(1010, 700, 240, 50, "Generate!", font, CallGenerate, true);
	creating::CreateButton(400, 295, 200, 50, "Click here to view...", font, CallViewRender, false);
	creating::CreateSlider(1010, 230, 240, "Precision point", font, false, 0.01f, 0.0001f, 0.02f);
	creating::CreateSlider(1010, 270, 240, "Max clones", font, false, 10000, 10000, 50000000);
	creating::CreateSlider(1010, 310, 240, "Noise amount", font, false, 0.f, 0.f, 15.f);
	creating::CreateSlider(1010, 350, 240, "Overlay Alpha", font, false, 0.f, 0.f, 255.f);
	creating::CreateDropbox(1010, 390, 3, font, new std::string[MAX_ITEMS]{"16:9", "4:3", "1:1"}, 0, "Clone aspect ratio");
	creating::CreateDropbox(1010, 430, 2, font, new std::string[MAX_ITEMS]{ "None","Floyd-Steinberg (beta)" }, 0, "Dithering");
	creating::CreateExit(font);
}

void Render::InitShader() {
	
	if (G->prevT.loadFromImage(rendered)) {
		S_gaussian.setUniform("texture", G->prevT);
		S_gaussian.setUniform("blur_radius", sf::Vector2f(0.005f, 0));
		S_gaussian.setUniform("blur_radius2", sf::Vector2f(0, 0.005f));
	}
	if (!S_gaussian.loadFromFile("resource/gaussianBlur.hlsl", sf::Shader::Fragment)) {
		MessageBox(NULL, TEXT("shader failed."), TEXT("Initializing error."), NULL);
	}
}

float scael = 1;

void Render::SetPrev() {
	tt = true;
	G->prevT.loadFromImage(rendered);
	prevS.setColor(sf::Color(255, 255, 255, 200));
	prevS.setTexture(G->prevT);
	prevS.setTextureRect(sf::IntRect(0, 0, G->sw, G->sh));


	float t1, t2;
	t1 = (float)((float)900 / (float)G->sw);
	t2 = (float)((float)500 / (float)G->sh);
	if (t1 > t2) {
		scael = t2;
	}
	else {
		scael = t1;
	}

	
	S_gaussian.setUniform("texture", G->prevT);
	S_gaussian.setUniform("blur_radius", sf::Vector2f(0.005f, 0));
	S_gaussian.setUniform("blur_radius2", sf::Vector2f(0, 0.005f));

	prevS.setPosition(sf::Vector2f(900 / 2 - (G->sw * scael) / 2 + 50, 500 / 2 - (G->sh * scael) / 2 + 70)); //+700 +430
	prevS.setScale(sf::Vector2f(scael, scael));
	G->overlay.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(G->sw, G->sh)));
	G->overlay.setPosition(sf::Vector2f(900 / 2 - (G->sw * scael) / 2 + 50, 500 / 2 - (G->sh * scael) / 2 + 70)); //+700 +430
	G->overlay.setScale(sf::Vector2f(scael, scael));
	G->overlay2.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(G->sw, G->sh)));
	G->overlay2.setPosition(sf::Vector2f(0,0));
	G->overlay2.setScale(sf::Vector2f(1, 1));
	

	prevWarn.setCharacterSize(18);
	prevWarn.setFillColor(sf::Color::White);
	prevWarn.setOutlineColor(sf::Color::Black);
	prevWarn.setOutlineThickness(0.7f);
	prevWarn.setString("To exit the preview click out of it, alt+tab or hit Esc.");
	if (G->sh > G->viewportY) {
		prevWarn.setCharacterSize(27);
		prevWarn.setPosition(sf::Vector2f(G->sw / 2 - prevWarn.getLocalBounds().width / 2, G->viewportY - prevWarn.getLocalBounds().height + (G->sh - G->viewportY) /2  - 10));
	}
	else {
		prevWarn.setPosition(sf::Vector2f(G->sw / 2 - prevWarn.getLocalBounds().width / 2, G->sh - prevWarn.getLocalBounds().height - 10));
	}
}

void Render::PrevClick(sf::RenderWindow* pwind) {
	pwind->draw(bgrend2);
	setrenderable(3, true);
}

void Render::RenderPrev(sf::RenderWindow* pwind) {

	if (G->sw == 0 || G->sh == 0 || !G->generated) return;

	

	int mx, my;
	Helpers::GetCursorToWindow(&mx, &my, pwind);

	int x, y, w, h;
	x = 50;
	y = 70;
	w = 900;
	h = 500;

	if (x < mx && mx < x + w && y < my && my < y + h) {  //hover
		pwind->draw(prevS, &S_gaussian);
		Render::PrevClick(pwind);
	}
	else {
		pwind->draw(prevS);
		setrenderable(3, false);
		if (G->overlayA != 0) {
			G->overlay.setColor(sf::Color(255, 255, 255, G->overlayA));
			pwind->draw(G->overlay);
		}
	}
}