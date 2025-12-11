#include "Input.h"

namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256] = { 0 };
	BYTE prevKeyState[256] = { 0 };

	//マウスのインプット
	LPDIRECTINPUTDEVICE8 pMouseDevice = nullptr;
	DIMOUSESTATE mouseState;//マウスの状態
	DIMOUSESTATE prevMouseState;

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		//マウスの初期化
		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}

	void Update()
	{
		memcpy(prevKeyState, keyState, sizeof(keyState));
		/*for (auto i = 0; i < 256; i++)
		{
			prevKeyState[i] = keyState[i];
		}*/

		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		//マウスの更新
		memcpy(&prevMouseState, &mouseState, sizeof(DIMOUSESTATE));
		pMouseDevice->Acquire();
		pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
		//マウスの座標データが相対座標で取得されるので注意
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 128)
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		if (keyState[keyCode] & 128 && ~prevKeyState[keyCode] & 128)
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		if (~keyState[keyCode] & 128 && prevKeyState[keyCode] & 128)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButton(int btnCode)
	{
		if (mouseState.rgbButtons[btnCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonDown(int btnCode)
	{
		if (mouseState.rgbButtons[btnCode] & 0x80 && ~prevMouseState.rgbButtons[btnCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonUp(int btnCode)
	{
		if (~mouseState.rgbButtons[btnCode] & 0x80 && prevMouseState.rgbButtons[btnCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	long GetMouseMoveX()
	{
		return mouseState.lX;
	}

	long GetMouseMoveY()
	{
		return mouseState.lY;
	}

}