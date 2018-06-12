#pragma once

#define DIRECTINPUT_VERSION 0x0800

#define DIRECTINPUT_HEADER_VERSION 0x0800

#ifndef DIRECTINPUT_VERSION

#define DIRECTINPUT_VERSION DIRECTINPUT_HEADER_VERSION

#pragma message(__FILE__ ": DIRECTINPUT_VERSION undefined. Defaulting to version 0x0800")

#endif

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();
	bool Initialize(HINSTANCE, HWND);
	void Shutdown();
	bool Frame();
	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
	bool IsKeyPressd(BYTE key);

	bool isMouseWheelUp();
	bool isMouseWheelDown();
	bool isMouseGoLeft();
	bool isMouseGoRight();
	bool isMouseGoUp();
	bool isMouseGoDown();
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsAPressed();
	bool IsZPressed();
	bool IsPgUpPressed();
	bool IsPgDownPressed();

	bool isQPressed();
	bool isEPressed();
	bool isFPressed();
	bool isRPressed();

	static InputClass* GetInstance();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
	HWND handle;

private:
	static InputClass* Instance;

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	unsigned char m_keyboardState[256] = { 0, };;
	DIMOUSESTATE m_mouseState;
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	int m_pre_mouseX, m_pre_mouseY;
};

