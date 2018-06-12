#include "stdafx.h"
#include "InputClass.h"
#include "D3dClass.h"

InputClass* InputClass::Instance = nullptr;

InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	handle = hwnd;
	HRESULT result;
	CRect rect;

	GetWindowRect(hwnd, &rect);
	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = rect.Width();
	m_screenHeight = rect.Height();
	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;
	m_pre_mouseX = 0;
	m_pre_mouseY = 0;
	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}
	// Set the data format. In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}
	SetFocus(hwnd);
	// Now acquire the keyboard.
	HR(m_keyboard->Acquire());
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}
	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void InputClass::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}
	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}
	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	SAFE_DELETE(Instance);

	return;
}

bool InputClass::Frame()
{
	bool result;
	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}
	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}
	// Process the changes in the mouse and keyboard.
	ProcessInput();
	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;
	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;
	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void InputClass::ProcessInput()
{
	m_pre_mouseX = m_mouseX;
	m_pre_mouseY = m_mouseY;

	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;
	// Ensure the mouse location doesn't exceed the screen width or height.
	//if (m_mouseX < 0)
	//{
	//	RECT rt;
	//	GetWindowRect(handle, &rt); //현재 윈도우의  위치 
	//	SetCursorPos((int)((rt.left + rt.right)*0.5f), (int)((rt.top + rt.bottom)*0.5f));
	//	m_mouseX = m_screenWidth*0.5f;
	//	m_mouseY = m_screenHeight*0.5f;
	//}
	//if (m_mouseY < 0)
	//{
	//	RECT rt;
	//	GetWindowRect(handle, &rt); //현재 윈도우의  위치 
	//	SetCursorPos((int)((rt.left + rt.right)*0.5f), (int)((rt.top + rt.bottom)*0.5f));
	//	m_mouseX = m_screenWidth*0.5f;
	//	m_mouseY = m_screenHeight*0.5f;
	//}
	//if (m_mouseX > m_screenWidth)
	//{
	//	RECT rt;
	//	GetWindowRect(handle, &rt); //현재 윈도우의  위치 
	//	SetCursorPos((int)((rt.left + rt.right)*0.5f), (int)((rt.top + rt.bottom)*0.5f));
	//	m_mouseX = m_screenWidth*0.5f;
	//	m_mouseY = m_screenHeight*0.5f;
	//}
	//if (m_mouseY > m_screenHeight)
	//{
	//	RECT rt;
	//	GetWindowRect(handle, &rt); //현재 윈도우의  위치 
	//	SetCursorPos((rt.left + rt.right)*0.5f, (rt.top + rt.bottom)*0.5f);
	//	m_mouseX = m_screenWidth*0.5f;
	//	m_mouseY = m_screenHeight*0.5f;
	//}

	return;
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}
	return false;
}
bool InputClass::IsRightPressed()
{
	if (m_keyboardState[DIK_RIGHT] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsUpPressed()
{
	if (m_keyboardState[DIK_UP] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsDownPressed()
{
	if (m_keyboardState[DIK_DOWN] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsAPressed()
{
	if (m_keyboardState[DIK_A] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsZPressed()
{
	if (m_keyboardState[DIK_Z] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsLeftPressed()
{
	if (m_keyboardState[DIK_LEFT] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsPgUpPressed()
{
	if (m_keyboardState[DIK_PGUP] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsPgDownPressed()
{
	if (m_keyboardState[DIK_PGDN] & 0x80)
	{
		return true;
	}

	return false;
}
bool InputClass::isQPressed()
{
	if (m_keyboardState[DIK_Q] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::isEPressed()
{
	if (m_keyboardState[DIK_E] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::isFPressed()
{
	if (m_keyboardState[DIK_F] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::isRPressed()
{
	if (m_keyboardState[DIK_R] & 0x80)
	{
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

bool InputClass::IsKeyPressd(BYTE key)
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[key] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::isMouseWheelUp()
{
	if (m_mouseState.lZ > 0.0f)
	{
		return true;
	}
	return false;
}

bool InputClass::isMouseWheelDown()
{
	if (m_mouseState.lZ < 0.0f)
	{
		return true;
	}
	return false;
}

bool InputClass::isMouseGoLeft()
{
	if (m_mouseX > m_pre_mouseX)
	{
		return true;
	}
	return false;
}

bool InputClass::isMouseGoRight()
{
	if (m_mouseX < m_pre_mouseX)
	{
		return true;
	}
	return false;
}

bool InputClass::isMouseGoUp()
{
	if (m_pre_mouseY > m_mouseY)
	{
		return true;
	}
	return false;
}

bool InputClass::isMouseGoDown()
{
	if (m_pre_mouseY < m_mouseY)
	{
		return true;
	}
	return false;
}

InputClass * InputClass::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new InputClass;
	}

	return Instance;
}
