#pragma once

#ifndef DEFINES
#define DEFINES 

#define MAX_INPUT        512
#define MAX_SAMPLES      2000000
#define MAX_CLONES       5000000
#define SCALE            0.01
#define BRUTYAS_COUNTRY_OF_ORIGIN Amurica
#define LOADING_X        480
#define LOADING_Y        200
#define BAR_X   900
#define BAR_Y   40
#define BILLION          1000000000
#define MILLION          1000000
#define DIV              1048576

#endif

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
#pragma comment( lib, "gdiplus" )
#include "render.h"
#include "Generate.h"
#include "GVars.h"

using namespace std::filesystem;
using namespace std::chrono;

typedef std::vector<sf::Sprite> sprite_vector;
typedef std::vector<sf::Texture> texture_vector;
typedef std::vector<sf::Image> image_vector;

void GetWindowPos(int*, int*, sf::RenderWindow*);
Color TransformH(const Color& in, float H);

namespace Helpers {
	int getNearest3D(sf::Color source, int scanRange);
	int GetPreciseNearest3D(preciseMeasure in, int scanRange);
	mfColor normalize(mfColor in);
	std::wstring s2ws(const std::string& s);
	int calculateScreenshot();
	void GetCursorToWindow(int* x, int* y, sf::RenderWindow* pwnd);
	int GetClickedTarget(int* id, sf::RenderWindow* pwnd, int*);
	std::string openfilename(int id);
	void Terminate();
	bool FrameLoopThread(sf::RenderWindow* pwnd);
}

enum GEN_ERROR_CODES {
	ERROR_VEC_INIT,
	ERROR_SOURCE,
	ERROR_COMPOSITE,
	ERROR_ABORTED,
	ERROR_GEN_SUCCESS
};

/*
extern sf::Color avg[MAX_INPUT];
extern float distances[MAX_INPUT];
extern high_resolution_clock::time_point timeorigin;
extern high_resolution_clock::time_point now;
extern high_resolution_clock::time_point framestart;
extern steady_clock::duration delta;
extern double alphaLoad;
extern bool alphaUp;
extern bool debug;
extern unsigned int maxclones;
extern float scale;*/
