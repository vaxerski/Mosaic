#include <iostream>
#include <Windows.h>
#include "parsing.h"
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

bool parsing::GetImageSize(const char* fn, int* x, int* y)
{
	//because SFML already did all of this for me :)
	//thanks SFML devs

	sf::Texture temp;
	if (!temp.loadFromFile(fn)) {
		return false;
	}

	sf::Vector2u size = temp.getSize();
	*x = size.x;
	*y = size.y;

	return true;
}

std::string parsing::launchPath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

