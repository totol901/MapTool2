#include "stdafx.h"
#include "CameraClass.h"


CameraClass::CameraClass()
{

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_LookAt.x = 0.0;
	m_LookAt.y = 0.0;
	m_LookAt.z = 1.0f;
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
	return;
}

void CameraClass::SetPosition(D3DXVECTOR3 vec)
{
	m_positionX = vec.x;
	m_positionY = vec.y;
	m_positionZ = vec.z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void CameraClass::SetRotation(D3DXVECTOR3 rot)
{
	m_rotationX = rot.x;
	m_rotationY = rot.y;
	m_rotationZ = rot.z;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformNormal(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformNormal(&up, &up, &rotationMatrix);

	m_LookAt = lookAt;

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::GetView_WorldMatrix(D3DXMATRIX & world)
{
	D3DXVECTOR3 lookAt, Pos;
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	Pos.x = m_positionX;
	Pos.y = m_positionY;
	Pos.z = m_positionZ;

	D3DXMATRIX s, r, t;
	D3DXMatrixIdentity(&s);
	D3DXMatrixIdentity(&r);
	D3DXMatrixRotationYawPitchRoll(&r,
		m_rotationX * 0.0174532925f,
		m_rotationY * 0.0174532925f,
		m_rotationZ * 0.0174532925f);
	D3DXVec3TransformNormal(&lookAt, &lookAt, &r);
	lookAt = Pos + lookAt;
	D3DXMatrixTranslation(&t, lookAt.x, lookAt.y, lookAt.z);

	world = r * t;
}

void CameraClass::SetTransCarmeraPosX(float speedX)
{
	D3DXVECTOR3 Left;
	D3DXVECTOR3 Up(0.0, 1.0, 0.0);
	D3DXVec3Cross(&Left, &m_LookAt, &Up);
	m_positionX += speedX * Left.x;
	m_positionY += speedX * Left.y;
	m_positionZ += speedX * Left.z;
}

void CameraClass::SetTransCarmeraPosZ(float speedZ)
{
	m_positionX -= speedZ * m_LookAt.x;
	m_positionY -= speedZ * m_LookAt.y;
	m_positionZ -= speedZ * m_LookAt.z;
}
