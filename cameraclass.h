//
//////////////////////////////////////////////////////////////////////////////////
//// Filename: cameraclass.h
//////////////////////////////////////////////////////////////////////////////////
//#ifndef _CAMERACLASS_H_
//#define _CAMERACLASS_H_
//
//
////////////////
//// INCLUDES //
////////////////
//#include <directxmath.h>
//using namespace DirectX;
//
//
//////////////////////////////////////////////////////////////////////////////////
//// Class name: CameraClass
//////////////////////////////////////////////////////////////////////////////////
//class CameraClass
//{
//public:
//	CameraClass();
//	CameraClass(const CameraClass&);
//	~CameraClass();
//
//	void SetPosition(float, float, float);
//	void SetRotation(float, float, float);
//
//	XMFLOAT3 GetPosition();
//	XMFLOAT3 GetRotation();
//
//	void Render();
//	void GetViewMatrix(XMMATRIX&);
//
//private:
//	float m_positionX, m_positionY, m_positionZ;
//	float m_rotationX, m_rotationY, m_rotationZ;
//	XMMATRIX m_viewMatrix;
//};
//
//#endif

////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

//////////////
// INCLUDES //
//////////////
#include <DirectXMath.h>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
    CameraClass();
    CameraClass(const CameraClass&);
    ~CameraClass();

    void SetPosition(float, float, float);
    void SetRotation(float, float, float);

    XMFLOAT3 GetPosition();
    XMFLOAT3 GetRotation();

    void Render();
    void GetViewMatrix(XMMATRIX&);

    // Методы для управления камерой
    void MoveForward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);
    void MoveUp(float speed);
    void MoveDown(float speed);

    void RotateCamera(float deltaX, float deltaY);

private:
    void UpdateCameraVectors();

private:
    float m_positionX, m_positionY, m_positionZ;
    float m_rotationX, m_rotationY, m_rotationZ;
    XMMATRIX m_viewMatrix;

    // Векторы направления камеры
    XMFLOAT3 m_front;
    XMFLOAT3 m_right;
    XMFLOAT3 m_up;
    XMFLOAT3 m_worldUp;
};

#endif