#pragma once
#include "windef.h"
#include <string>

namespace Global
{
	static int ScreenWidth;
	static int ScreenHeight;
	inline void SetScreenSize(int width, int hight) {
		ScreenWidth = width;
		ScreenHeight = hight;
	}
}