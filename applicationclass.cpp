//////////////////////////////////////////////////////////////////////////////////
//// Filename: applicationclass.cpp
//////////////////////////////////////////////////////////////////////////////////
//#include "applicationclass.h"
//
//
//ApplicationClass::ApplicationClass()
//{
//	m_Direct3D = 0;
//	m_Camera = 0;
//	m_Model = 0;
//	m_ColorShader = 0;
//}
//
//ApplicationClass::ApplicationClass(const ApplicationClass& other)
//{
//}
//
//ApplicationClass::~ApplicationClass()
//{
//}
//
//bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
//{
//	bool result;
//
//
//	// Create and initialize the Direct3D object.
//	m_Direct3D = new D3DClass;
//
//	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
//		return false;
//	}
//
//	// Create the camera object.
//	m_Camera = new CameraClass;
//
//	// Set the initial position of the camera.
//	m_Camera->SetPosition(0.0f, 0.0f, -30.0f);
//
//	// Create and initialize the model object.
//	m_Model = new ModelClass;
//
//	result = m_Model->Initialize(m_Direct3D->GetDevice());
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
//		return false;
//	}
//
//	// Create and initialize the color shader object.
//	m_ColorShader = new ColorShaderClass;
//
//	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
//		return false;
//	}
//
//	return true;
//}
//
//void ApplicationClass::Shutdown()
//{
//	// Release the color shader object.
//	if (m_ColorShader)
//	{
//		m_ColorShader->Shutdown();
//		delete m_ColorShader;
//		m_ColorShader = 0;
//	}
//
//	// Release the model object.
//	if (m_Model)
//	{
//		m_Model->Shutdown();
//		delete m_Model;
//		m_Model = 0;
//	}
//
//	// Release the camera object.
//	if (m_Camera)
//	{
//		delete m_Camera;
//		m_Camera = 0;
//	}
//	// Release the Direct3D object.
//	if (m_Direct3D)
//	{
//		m_Direct3D->Shutdown();
//		delete m_Direct3D;
//		m_Direct3D = 0;
//	}
//
//	return;
//}
//
//bool ApplicationClass::Frame()
//{
//	bool result;
//
//
//	// Render the graphics scene.
//	result = Render();
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool ApplicationClass::Render()
//{
//	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
//	bool result;
//
//
//	// Clear the buffers to begin the scene.
//	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
//
//	// Generate the view matrix based on the camera's position.
//	m_Camera->Render();
//
//	// Get the world, view, and projection matrices from the camera and d3d objects.
//	m_Direct3D->GetWorldMatrix(worldMatrix);
//	m_Camera->GetViewMatrix(viewMatrix);
//	m_Direct3D->GetProjectionMatrix(projectionMatrix);
//
//	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
//	m_Model->Render(m_Direct3D->GetDeviceContext());
//
//	// Render the model using the color shader.
//	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
//	if (!result)
//	{
//		return false;
//	}
//
//	// Present the rendered scene to the screen.
//	m_Direct3D->EndScene();
//
//	return true;
//}

////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{
    m_Input = 0;
    m_Direct3D = 0;
    m_Camera = 0;
    m_Model = 0;
    m_ColorShader = 0;
    m_cameraSpeed = 0.05f;
    m_screenWidth = 0;
    m_screenHeight = 0;
    m_hwnd = 0;
    m_exit = false; // Добавляем флаг выхода
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    bool result;

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_hwnd = hwnd;
    m_exit = false;

    // Создаем и инициализируем объект ввода
    m_Input = new InputClass;
    result = m_Input->Initialize(hwnd, screenWidth, screenHeight);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
        return false;
    }

    // Создаем и инициализируем объект Direct3D
    m_Direct3D = new D3DClass;
    result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }

    // Создаем объект камеры
    m_Camera = new CameraClass;
    m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

    // Создаем и инициализируем объект модели
    m_Model = new ModelClass;
    result = m_Model->Initialize(m_Direct3D->GetDevice());
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    // Создаем и инициализируем объект шейдера
    m_ColorShader = new ColorShaderClass;
    result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
        return false;
    }

    return true;
}

void ApplicationClass::HandleInput()
{
    // Обновляем состояние ввода
    m_Input->Frame();

    // ПРОВЕРЯЕМ НАЖАТИЕ ESC ДЛЯ ВЫХОДА
    if (m_Input->IsKeyDown(DIK_ESCAPE))
    {
        m_exit = true;
        return;
    }

    // Проверяем нажатие клавиш WASD и движение
    if (m_Input->IsKeyDown(DIK_W))
    {
        m_Camera->MoveForward(m_cameraSpeed);
    }
    if (m_Input->IsKeyDown(DIK_S))
    {
        m_Camera->MoveBackward(m_cameraSpeed);
    }
    if (m_Input->IsKeyDown(DIK_A))
    {
        m_Camera->MoveLeft(m_cameraSpeed);
    }
    if (m_Input->IsKeyDown(DIK_D))
    {
        m_Camera->MoveRight(m_cameraSpeed);
    }
    if (m_Input->IsKeyDown(DIK_SPACE))
    {
        m_Camera->MoveUp(m_cameraSpeed);
    }
    if (m_Input->IsKeyDown(DIK_C))
    {
        m_Camera->MoveDown(m_cameraSpeed);
    }

    // Проверяем движение мыши
    int mouseX, mouseY;
    m_Input->GetMouseLocation(mouseX, mouseY);

    // Центр экрана
    int centerX = m_screenWidth / 2;
    int centerY = m_screenHeight / 2;

    // Вычисляем разницу между текущим положением и центром экрана
    int deltaX = mouseX - centerX;
    int deltaY = mouseY - centerY;

    // Если мышь сдвинулась от центра - вращаем камеру
    if (deltaX != 0 || deltaY != 0)
    {
        m_Camera->RotateCamera((float)deltaX, (float)deltaY);

        // Возвращаем курсор в центр экрана
        m_Input->SetMousePosition(centerX, centerY);
    }
}

void ApplicationClass::Shutdown()
{
    // Освобождаем объект шейдера
    if (m_ColorShader)
    {
        m_ColorShader->Shutdown();
        delete m_ColorShader;
        m_ColorShader = 0;
    }

    // Освобождаем объект модели
    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }

    // Освобождаем объект камеры
    if (m_Camera)
    {
        delete m_Camera;
        m_Camera = 0;
    }

    // Освобождаем объект Direct3D
    if (m_Direct3D)
    {
        m_Direct3D->Shutdown();
        delete m_Direct3D;
        m_Direct3D = 0;
    }

    // Освобождаем объект ввода
    if (m_Input)
    {
        m_Input->Shutdown();
        delete m_Input;
        m_Input = 0;
    }

    return;
}

bool ApplicationClass::Frame()
{
    bool result;

    // Обрабатываем ввод пользователя
    HandleInput();

    // Если нажат ESC - выходим
    if (m_exit)
    {
        return false;
    }

    // Рендерим графическую сцену
    result = Render();
    if (!result)
    {
        return false;
    }

    return true;
}

bool ApplicationClass::Render()
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result;

    // Очищаем буферы для начала сцены
    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Генерируем матрицу вида на основе позиции камеры
    m_Camera->Render();

    // Получаем мировую, видовую и проекционную матрицы
    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);

    // Помещаем буферы вершин и индексов модели в конвейер
    m_Model->Render(m_Direct3D->GetDeviceContext());

    // Рендерим модель с использованием шейдера
    result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(),
        m_Model->GetIndexCount(),
        worldMatrix,
        viewMatrix,
        projectionMatrix);
    if (!result)
    {
        return false;
    }

    // Отображаем отрендеренную сцену на экране
    m_Direct3D->EndScene();

    return true;
}