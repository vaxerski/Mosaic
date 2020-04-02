//-----------------------//
//Entire code by vaxerski.
//Licensed under GNU GPLv2.0
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
#include <filesystem>
#include <Psapi.h>
#include <sysinfoapi.h>
#include <cassert>
#include <functional>
#include <memory>
#include <dwmapi.h>
#define WIN32_LEAN_AND_MEAN
#include <dwmapi.h>
#include <unknwn.h>
#include <gdiplus.h>
#include <future> //async shit
#include <mutex>
#pragma comment( lib, "gdiplus" )

#include "INIReader.h"
#include "parsing.h"
#include "math.h"
#include "helpers.h"
#include "render.h"
#include "controls.h"
#include "GVars.h"

BOOL shader = false;

bool threadJoined = false;

sf::RenderWindow* pWind = &window;
sf::Font* pFont = &font;
high_resolution_clock::time_point timeorigin;
high_resolution_clock::time_point frame;

std::unique_ptr<GVars> G;

std::mutex mu;

int main(int argc, char* argv[]) {
	G = std::make_unique<GVars>();
	G->isInPrev = false;
	auto renderer = std::async(std::launch::async, Helpers::FrameLoopThread, pWind);
	auto generator = std::async(std::launch::async, Generate::Thread, pFont);

	renderer.get(), generator.get();

	return ERROR_SUCCESS;
}