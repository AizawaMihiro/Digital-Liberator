#pragma once
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace Input
{
	void Initialize(HWND hWnd);
	void Update();
	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);

	bool IsMouseButton(int btnCode);
	bool IsMouseButtonDown(int btnCode);
	bool IsMouseButtonUp(int btnCode);
}

