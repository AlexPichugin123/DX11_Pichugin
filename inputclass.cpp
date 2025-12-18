//////////////////////////////////////////////////////////////////////////////////
//// Filename: inputclass.cpp
//////////////////////////////////////////////////////////////////////////////////
//#include "inputclass.h"
//
//
//InputClass::InputClass()
//{
//}
//
//
//InputClass::InputClass(const InputClass& other)
//{
//}
//
//
//InputClass::~InputClass()
//{
//}
//
//
//void InputClass::Initialize()
//{
//	int i;
//
//
//	// Initialize all the keys to being released and not pressed.
//	for (i = 0; i < 256; i++)
//	{
//		m_keys[i] = false;
//	}
//
//	return;
//}
//
//
//void InputClass::KeyDown(unsigned int input)
//{
//	// If a key is pressed then save that state in the key array.
//	m_keys[input] = true;
//	return;
//}
//
//
//void InputClass::KeyUp(unsigned int input)
//{
//	// If a key is released then clear that state in the key array.
//	m_keys[input] = false;
//	return;
//}
//
//
//bool InputClass::IsKeyDown(unsigned int key)
//{
//	// Return what state the key is in (pressed/not pressed).
//	return m_keys[key];
//}


////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"

InputClass::InputClass()
{
    m_directInput = 0;
    m_keyboard = 0;
    m_mouse = 0;
    m_screenWidth = 0;
    m_screenHeight = 0;
    m_mouseX = 0;
    m_mouseY = 0;
    m_hwnd = 0;
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
    HRESULT result;

    m_hwnd = hwnd;
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // Инициализируем основной интерфейс DirectInput
    result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
        IID_IDirectInput8, (void**)&m_directInput, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Инициализируем интерфейс клавиатуры
    result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
    if (FAILED(result))
    {
        return false;
    }

    result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(result))
    {
        return false;
    }

    result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if (FAILED(result))
    {
        return false;
    }

    result = m_keyboard->Acquire();
    if (FAILED(result))
    {
        return false;
    }

    // Инициализируем интерфейс мыши
    result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
    if (FAILED(result))
    {
        return false;
    }

    result = m_mouse->SetDataFormat(&c_dfDIMouse);
    if (FAILED(result))
    {
        return false;
    }

    result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(result))
    {
        return false;
    }

    result = m_mouse->Acquire();
    if (FAILED(result))
    {
        return false;
    }

    // Центрируем курсор
    m_mouseX = screenWidth / 2;
    m_mouseY = screenHeight / 2;
    SetMousePosition(m_mouseX, m_mouseY);

    return true;
}

void InputClass::Shutdown()
{
    // Освобождаем мышь
    if (m_mouse)
    {
        m_mouse->Unacquire();
        m_mouse->Release();
        m_mouse = 0;
    }

    // Освобождаем клавиатуру
    if (m_keyboard)
    {
        m_keyboard->Unacquire();
        m_keyboard->Release();
        m_keyboard = 0;
    }

    // Освобождаем основной интерфейс DirectInput
    if (m_directInput)
    {
        m_directInput->Release();
        m_directInput = 0;
    }
}

bool InputClass::Frame()
{
    bool result;

    // Считываем текущее состояние клавиатуры и мыши
    result = ReadKeyboard();
    if (!result)
    {
        return false;
    }

    result = ReadMouse();
    if (!result)
    {
        return false;
    }

    // Обновляем положение мыши
    m_mouseX += m_mouseState.lX;
    m_mouseY += m_mouseState.lY;

    // Ограничиваем положение мыши границами экрана
    if (m_mouseX < 0) m_mouseX = 0;
    if (m_mouseY < 0) m_mouseY = 0;
    if (m_mouseX > m_screenWidth) m_mouseX = m_screenWidth;
    if (m_mouseY > m_screenHeight) m_mouseY = m_screenHeight;

    return true;
}

bool InputClass::ReadKeyboard()
{
    HRESULT result;

    // Считываем состояние клавиатуры
    result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
    if (FAILED(result))
    {
        // Если клавиатура потеряна, пытаемся снова получить контроль
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

    // Считываем состояние мыши
    result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
    if (FAILED(result))
    {
        // Если мышь потеряна, пытаемся снова получить контроль
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

bool InputClass::IsKeyDown(unsigned int key)
{
    // Проверяем, нажата ли клавиша
    return (m_keyboardState[key] & 0x80) != 0;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
    mouseX = m_mouseX;
    mouseY = m_mouseY;
}

void InputClass::SetMousePosition(int x, int y)
{
    POINT point;
    point.x = x;
    point.y = y;

    ClientToScreen(m_hwnd, &point);
    SetCursorPos(point.x, point.y);

    m_mouseX = x;
    m_mouseY = y;
}