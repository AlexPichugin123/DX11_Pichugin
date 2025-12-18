//////////////////////////////////////////////////////////////////////////////////
//// Filename: cameraclass.cpp
//////////////////////////////////////////////////////////////////////////////////
//#include "cameraclass.h"
//CameraClass::CameraClass()
//{
//	m_positionX = 0.0f;
//	m_positionY = 0.0f;
//	m_positionZ = 0.0f;
//
//	m_rotationX = 0.0f;
//	m_rotationY = 0.0f;
//	m_rotationZ = 0.0f;
//}
//
//
//CameraClass::CameraClass(const CameraClass& other)
//{
//}
//
//
//CameraClass::~CameraClass()
//{
//}
//void CameraClass::SetPosition(float x, float y, float z)
//{
//	m_positionX = x;
//	m_positionY = y;
//	m_positionZ = z;
//	return;
//}
//
//
//void CameraClass::SetRotation(float x, float y, float z)
//{
//	m_rotationX = x;
//	m_rotationY = y;
//	m_rotationZ = z;
//	return;
//}
//XMFLOAT3 CameraClass::GetPosition()
//{
//	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
//}
//
//
//XMFLOAT3 CameraClass::GetRotation()
//{
//	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
//}
//void CameraClass::Render()
//{
//	XMFLOAT3 up, position, lookAt;
//	XMVECTOR upVector, positionVector, lookAtVector;
//	float yaw, pitch, roll;
//	XMMATRIX rotationMatrix;
//
//
//	// Setup the vector that points upwards.
//	up.x = 0.0f;
//	up.y = 1.0f;
//	up.z = 0.0f;
//
//	// Load it into a XMVECTOR structure.
//	upVector = XMLoadFloat3(&up);
//
//	// Setup the position of the camera in the world.
//	position.x = m_positionX;
//	position.y = m_positionY;
//	position.z = m_positionZ;
//
//	// Load it into a XMVECTOR structure.
//	positionVector = XMLoadFloat3(&position);
//
//	// Setup where the camera is looking by default.
//	lookAt.x = 0.0f;
//	lookAt.y = 0.0f;
//	lookAt.z = 1.0f;
//
//	// Load it into a XMVECTOR structure.
//	lookAtVector = XMLoadFloat3(&lookAt);
//
//	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
//	pitch = m_rotationX * 0.0174532925f;
//	yaw = m_rotationY * 0.0174532925f;
//	roll = m_rotationZ * 0.0174532925f;
//
//	// Create the rotation matrix from the yaw, pitch, and roll values.
//	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
//
//	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
//	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
//	upVector = XMVector3TransformCoord(upVector, rotationMatrix);
//
//	// Translate the rotated camera position to the location of the viewer.
//	lookAtVector = XMVectorAdd(positionVector, lookAtVector);
//
//	// Finally create the view matrix from the three updated vectors.
//	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
//
//	return;
//}
//void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
//{
//	viewMatrix = m_viewMatrix;
//	return;
//}

////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"

CameraClass::CameraClass()
{
    m_positionX = 0.0f;
    m_positionY = 0.0f;
    m_positionZ = -10.0f;

    m_rotationX = 0.0f;
    m_rotationY = 0.0f;
    m_rotationZ = 0.0f;

    // Инициализация векторов направления
    m_worldUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
    m_front = XMFLOAT3(0.0f, 0.0f, 1.0f);
    UpdateCameraVectors();
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
    m_rotationX = x;
    m_rotationY = y;
    m_rotationZ = z;
    UpdateCameraVectors();
}

XMFLOAT3 CameraClass::GetPosition()
{
    return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
    return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::MoveForward(float speed)
{
    m_positionX += m_front.x * speed;
    m_positionY += m_front.y * speed;
    m_positionZ += m_front.z * speed;
}

void CameraClass::MoveBackward(float speed)
{
    m_positionX -= m_front.x * speed;
    m_positionY -= m_front.y * speed;
    m_positionZ -= m_front.z * speed;
}

void CameraClass::MoveLeft(float speed)
{
    m_positionX -= m_right.x * speed;
    m_positionY -= m_right.y * speed;
    m_positionZ -= m_right.z * speed;
}

void CameraClass::MoveRight(float speed)
{
    m_positionX += m_right.x * speed;
    m_positionY += m_right.y * speed;
    m_positionZ += m_right.z * speed;
}

void CameraClass::MoveUp(float speed)
{
    m_positionY += speed;
}

void CameraClass::MoveDown(float speed)
{
    m_positionY -= speed;
}

void CameraClass::RotateCamera(float deltaX, float deltaY)
{
    // Умножаем на чувствительность мыши и ИНВЕРТИРУЕМ движение
    m_rotationY -= deltaX * 0.1f;  // Минус для инверсии горизонтали
    m_rotationX -= deltaY * 0.1f;  // Минус для инверсии вертикали

    // Ограничиваем вертикальный поворот, чтобы камера не переворачивалась
    if (m_rotationX > 89.0f) m_rotationX = 89.0f;
    if (m_rotationX < -89.0f) m_rotationX = -89.0f;

    UpdateCameraVectors();
}

void CameraClass::UpdateCameraVectors()
{
    // Вычисляем новые векторы направления на основе углов поворота
    float yaw = m_rotationY * 0.0174532925f; // градусы в радианы
    float pitch = m_rotationX * 0.0174532925f;

    // Вычисляем вектор направления взгляда
    XMFLOAT3 front;
    front.x = cos(yaw) * cos(pitch);
    front.y = sin(pitch);
    front.z = sin(yaw) * cos(pitch);

    // Нормализуем вектор направления
    XMVECTOR frontVector = XMLoadFloat3(&front);
    frontVector = XMVector3Normalize(frontVector);
    XMStoreFloat3(&m_front, frontVector);

    // Вычисляем правый вектор (перпендикулярно фронту и мировому up)
    XMVECTOR worldUpVector = XMLoadFloat3(&m_worldUp);
    XMVECTOR rightVector = XMVector3Cross(worldUpVector, frontVector);
    rightVector = XMVector3Normalize(rightVector);
    XMStoreFloat3(&m_right, rightVector);

    // Вычисляем вектор вверх камеры
    XMVECTOR upVector = XMVector3Cross(frontVector, rightVector);
    upVector = XMVector3Normalize(upVector);
    XMStoreFloat3(&m_up, upVector);
}

void CameraClass::Render()
{
    XMFLOAT3 up, position, lookAt;
    XMVECTOR upVector, positionVector, lookAtVector;

    // Используем вычисленные векторы камеры
    up = m_up;
    upVector = XMLoadFloat3(&up);

    // Позиция камеры
    position.x = m_positionX;
    position.y = m_positionY;
    position.z = m_positionZ;
    positionVector = XMLoadFloat3(&position);

    // Направление взгляда (позиция + фронт)
    lookAt.x = m_positionX + m_front.x;
    lookAt.y = m_positionY + m_front.y;
    lookAt.z = m_positionZ + m_front.z;
    lookAtVector = XMLoadFloat3(&lookAt);

    // Создаем матрицу вида
    m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
    viewMatrix = m_viewMatrix;
}