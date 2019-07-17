#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <fstream>
#include "INIReader.h"
#include "math.h"
#include "helpers.h"
#include "parsing.h"
#include "resource.h"
#include "controls.h"
#include <shlobj.h>
#include <shlwapi.h>
#include <strsafe.h>



#define M_PI 3.14159265358979f

using namespace std::chrono;

extern sf::Color avg[MAX_INPUT];
extern float distances[MAX_INPUT];
extern high_resolution_clock::time_point timeorigin;
extern high_resolution_clock::time_point frame;
extern bool generated;

int Helpers::getNearest3D(sf::Color source, int scanRange) {
	int leader;
	float distance = 1337;
	float dis;

	int r, g, b;
	r = source.r;
	g = source.g;
	b = source.b;
	int r1, g1, b1;

	for (int x = 1; x < scanRange; x++) {
		r1 = avg[x].r;
		g1 = avg[x].g;
		b1 = avg[x].b;

		dis = sqrt(pow((r - r1), 2) + pow((g - g1), 2) + pow((b - b1), 2));
		distances[x] = dis;

		if (dis < distance) {
			leader = x;
			distance = dis;
		}
	}

	return leader;

}

std::wstring Helpers::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


void GetWindowPos(int* x, int* y, sf::RenderWindow* pwnd) {
	sf::Vector2i temp = pwnd->getPosition();

	*x = temp.x;
	*y = temp.y;
}

void Helpers::GetCursorToWindow(int* x, int* y, sf::RenderWindow* pwnd) {
	int wx, wy;
	GetWindowPos(&wx, &wy, pwnd);

	POINT cursor;
	GetCursorPos(&cursor);

	int mx, my;
	mx = cursor.x - wx;
	my = cursor.y - wy;

	*x = mx;
	*y = my;
}

LPWSTR g_path = NULL;


void PickContainer()
{
	IFileDialog* filedial;
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&filedial)))) {
		DWORD dwOptions;
		if (SUCCEEDED(filedial->GetOptions(&dwOptions))) {
			filedial->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST);
		}
		if (SUCCEEDED(filedial->Show(NULL))) {
			IShellItem* pshellitem;
			if (SUCCEEDED(filedial->GetResult(&pshellitem))) {
				if (!SUCCEEDED(pshellitem->GetDisplayName(SIGDN_FILESYSPATH, &g_path))) {
					MessageBox(NULL, "GetIdListName() failed", NULL, NULL);
				}
				pshellitem->Release();
			}
		}
		filedial->Release();
	}
}

bool cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage = CP_ACP)
{
	bool res = false;
	char* p = 0;
	int bsz;
	bsz = WideCharToMultiByte(codepage,
		0,
		pw, -1,
		0, 0,
		0, 0);
	if (bsz > 0) {
		p = new char[bsz];
		int rc = WideCharToMultiByte(codepage,
			0,
			pw, -1,
			p, bsz,
			0, 0);
		if (rc != 0) {
			p[bsz - 1] = 0;
			s = p;
			res = true;
		}
	}
	delete[] p;
	return res;
}


using namespace std;
// Returns an empty string if dialog is canceled
std::string Helpers::openfilename(int id) {
	OPENFILENAME ofn;
	HWND owner = NULL;
	char* filter;
	if (id == 0) {
		filter = "All acceptable files (*.png, *.jpg, *.jpeg)\0*.png;*.jpg;*.jpeg\0";
		ofn.lpstrTitle = "Select source...";
		char fileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = owner;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "";

		string fileNameStr;
		if (GetOpenFileName(&ofn))
			fileNameStr = fileName;
		return fileNameStr;
	}
	else if (id == 1) {

		PickContainer();

		std::string str; 
		cvtLPW2stdstring(str, g_path);

		return str;
	}
	else if (id == 2) {
		filter = "*.png files\0*.png";
		ofn.lpstrTitle = "Save as...";
		char fileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = owner;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "";

		string fileNameStr;
		if (GetSaveFileName(&ofn))
			fileNameStr = fileName;
		return fileNameStr;
	}
}

Color TransformH(const Color& in, float H)
{
	float U = cos(H * M_PI / 180);
	float W = sin(H * M_PI / 180);

	Color ret;
	ret.r = (.299 + .701 * U + .168 * W) * in.r
		+ (.587 - .587 * U + .330 * W) * in.g
		+ (.114 - .114 * U - .497 * W) * in.b;
	ret.g = (.299 - .299 * U - .328 * W) * in.r
		+ (.587 + .413 * U + .035 * W) * in.g
		+ (.114 - .114 * U + .292 * W) * in.b;
	ret.b = (.299 - .3 * U + 1.25 * W) * in.r
		+ (.587 - .588 * U - 1.05 * W) * in.g
		+ (.114 + .886 * U - .203 * W) * in.b;
	return ret;
}

bool Helpers::forceRender(sf::RenderWindow* pWind){
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
				case button:
					//buttonclick
					butoncallbacks(reinterpret_cast<CButton*>(getPointerA(button, id))->callbackID, nullptr, pWind, nullptr);
					break;
				case exitbutton:
					//exit button
					butoncallbacks(reinterpret_cast<CExit*>(getPointerA(exitbutton, id))->callback, nullptr, pWind, nullptr);
					break;
				}
			}
		}
		if (event.type == sf::Event::MouseButtonReleased) {
			//removed event handling for forced frame
		}
	}

	//removed event handling for forced frame


	pWind->clear(sf::Color::Transparent);

	Render::GUI(pWind);

	frame = high_resolution_clock::now();
	std::chrono::steady_clock::duration delta = frame - timeorigin;

	Render::DebugInfo(pWind, (double)((double)delta.count() / (double)1000000000));

	pWind->display();

	generated = false;

	return true;
}

