#pragma once
#include "windef.h"
#include <string>

namespace Global
{
	static LPRECT ClientRect = new RECT();
	/// <summary>
	/// ウィンドウのサイズを取得する
	/// </summary>
	/// <returns>first = right , second = bottom</returns>
	inline std::pair<int, int> WindowSize() {
		return { ClientRect->right - ClientRect->left, ClientRect->bottom - ClientRect->top };
	}
}