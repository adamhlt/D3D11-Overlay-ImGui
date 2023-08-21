#ifndef DRAWING_H
#define DRAWING_H

#include "pch.h"
#include "UI.h"

class Drawing
{
private:
	static LPCSTR lpWindowName;
	static ImVec2 vWindowSize;
	static ImGuiWindowFlags WindowFlags;
	static bool bDraw;
	static UI::WindowItem lpSelectedWindow;

public:
	static bool isActive();
	static void Draw();
};

#endif