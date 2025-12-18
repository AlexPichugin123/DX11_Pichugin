//
//////////////////////////////////////////////////////////////////////////////////
//// Filename: applicationclass.h
//////////////////////////////////////////////////////////////////////////////////
//#ifndef _APPLICATIONCLASS_H_
//#define _APPLICATIONCLASS_H_
//
//
////////////////
//// INCLUDES //
////////////////
//#include "d3dclass.h"
//#include "cameraclass.h"
//#include "modelclass.h"
//#include "colorshaderclass.h"
//
//
///////////////
//// GLOBALS //
///////////////
//const bool FULL_SCREEN = false;
//const bool VSYNC_ENABLED = true;
//const float SCREEN_DEPTH = 100.0f;
//const float SCREEN_NEAR = 0.3f;
//////////////////////////////////////////////////////////////////////////////////
//// Class name: ApplicationClass
//////////////////////////////////////////////////////////////////////////////////
//class ApplicationClass
//{
//public:
//	ApplicationClass();
//	ApplicationClass(const ApplicationClass&);
//	~ApplicationClass();
//
//	bool Initialize(int, int, HWND);
//	void Shutdown();
//	bool Frame();
//
//private:
//	bool Render();
//
//private:
//
//	D3DClass* m_Direct3D;
//	CameraClass* m_Camera;
//	ModelClass* m_Model;
//	ColorShaderClass* m_ColorShader;
//};
//
//#endif

////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
    ApplicationClass();
    ApplicationClass(const ApplicationClass&);
    ~ApplicationClass();

    bool Initialize(int, int, HWND);
    void Shutdown();
    bool Frame();

private:
    bool Render();
    void HandleInput();

private:
    InputClass* m_Input;
    D3DClass* m_Direct3D;
    CameraClass* m_Camera;
    ModelClass* m_Model;
    ColorShaderClass* m_ColorShader;
    float m_cameraSpeed;
    int m_screenWidth, m_screenHeight;
    HWND m_hwnd;
    bool m_exit; // Флаг для выхода по ESC
};

#endif