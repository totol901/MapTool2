#include "stdafx.h"
#include "Object.h"

void Vector3Multiply(OUT D3DXVECTOR3& a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{
	a.x = b.x * c.x;
	a.y = b.y * c.y;
	a.z = b.z * c.z;
}
Object::Object()
	: mPosition(0, 0, 0)
	, mScale(1, 1, 1)
	, mForward(0, 0, 1)
	, mUp(0, 1, 0)
	, mRight(1, 0, 0)
	, mParent(nullptr)
{
	D3DXMatrixIdentity(&mLocal);
	D3DXMatrixIdentity(&mWorld);
}

Object::~Object()
{
	vector<Object*>::iterator siter = mChilds.begin();
	vector<Object*>::iterator eiter = mChilds.end();
	for (; siter != eiter; ++siter)
	{
		SAFE_DELETE(*siter);
	}
	mChilds.clear();
	SAFE_DELETE(mParent);
}

void Object::Update(const float deltaTime)
{
	D3DXVECTOR3 s = mScale;
	D3DXVECTOR3 r;
	D3DXVECTOR3 u;
	D3DXVECTOR3 f;
	Vector3Multiply(r, mRight, s);
	Vector3Multiply(u, mUp, s);
	Vector3Multiply(f, mForward, s);

	memcpy(&mLocal.m[0], &r, sizeof(D3DXVECTOR3));
	memcpy(&mLocal.m[1], &u, sizeof(D3DXVECTOR3));
	memcpy(&mLocal.m[2], &f, sizeof(D3DXVECTOR3));
	memcpy(&mLocal.m[3], &mPosition, sizeof(D3DXVECTOR3));

	if (mParent)
	{
		D3DXMatrixMultiply(&mWorld, &mLocal, &mParent->GetWorldMatrix());
	}
	else
	{
		mWorld = mLocal;
	}
}

void Object::SetPosition(D3DXVECTOR3 pos)
{
	mPosition = pos;
}

void Object::MovePosition(D3DXVECTOR3 d)
{
	D3DXVECTOR3 r;
	D3DXVECTOR3 u;
	D3DXVECTOR3 f;
	D3DXVec3Normalize(&r, &mRight);
	D3DXVec3Normalize(&u, &mUp);
	D3DXVec3Normalize(&f, &mForward);

	D3DXVECTOR3 m = D3DXVECTOR3(0, 0, 0);

	D3DXVECTOR3 p = mPosition;

	m += r * d.x;
	m += u * d.x;
	m += f * d.x;

	p += m;

	mPosition = p;
}

void Object::SetScale(D3DXVECTOR3 scale)
{
	mScale = scale;
}

void Object::SetRotation(D3DXVECTOR3 rot)
{
	//XMFLOAT3 Rot;
	//
	//XMStoreFloat3(&Rot, rot);
	//
	//XMMATRIX matX = XMMatrixRotationX(Rot.x);
	//XMMATRIX matY = XMMatrixRotationY(Rot.y);
	//XMMATRIX matZ = XMMatrixRotationZ(Rot.z);
	//
	////XMMATRIX XYZ = XMMatrixRotationQuaternion(rot);
	//
	//XMMATRIX XYZ = matX * matY * matZ;
	//
	//XMVECTOR r = XMVector3Normalize(XMLoadFloat3(&mRight));
	//XMVECTOR u = XMVector3Normalize(XMLoadFloat3(&mUp));
	//XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&mForward));
	//
	//r = XMVector3TransformNormal(r, XYZ);
	//u = XMVector3TransformNormal(u, XYZ);
	//f = XMVector3TransformNormal(f, XYZ);
	//
	//XMStoreFloat3(&mRight, r);
	//XMStoreFloat3(&mUp, u);
	//XMStoreFloat3(&mForward, f);
}

void Object::LookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up)
{
	//XMVECTOR newUp;
	//XMVECTOR newRight;
	//newRight = XMVector3Normalize(XMVector3Cross(up, dir));
	//
	//newUp = XMVector3Normalize(XMVector3Cross(dir, newRight));
	//
	//XMStoreFloat3(&mForward, dir);
	//XMStoreFloat3(&mUp, newUp);
	//XMStoreFloat3(&mRight, newRight);
}

void Object::LookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up)
{
	//XMVECTOR p = XMLoadFloat3(&mPosition);
	//XMVECTOR dir = XMVector3Normalize(pos - p);
	//
	//LookDirection(dir, up);
}

void Object::Walk(float delta)
{
	//XMVECTOR s = XMVectorReplicate(delta * 10);
	//XMVECTOR l = XMLoadFloat3(&mForward);
	//XMVECTOR p = XMLoadFloat3(&mPosition);
	//XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}

void Object::Strafe(float delta)
{
	//XMVECTOR s = XMVectorReplicate(delta * 10);
	//XMVECTOR r = XMLoadFloat3(&mRight);
	//XMVECTOR p = XMLoadFloat3(&mPosition);
	//XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

void Object::Pitch(float angle)
{
	//XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);
	//
	//XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	//XMStoreFloat3(&mForward, XMVector3TransformNormal(XMLoadFloat3(&mForward), R));
}

void Object::RotationY(float angle)
{
	//XMMATRIX R = XMMatrixRotationY(angle);
	//
	//XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	//XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	//XMStoreFloat3(&mForward, XMVector3TransformNormal(XMLoadFloat3(&mForward), R));
}

VOID Object::MouseMove(POINT pt)
{
	//float dx = -XMConvertToRadians(0.25f * pt.x);
	////float dy = -XMConvertToRadians(0.25f * pt.y);
	//
	////Pitch(dy);
	//RotationY(dx);
}

VOID Object::Moving(float delta)
{
	if (GetAsyncKeyState('W') & 0x8000)
		Walk(delta);
	if (GetAsyncKeyState('S') & 0x8000)
		Walk(-delta);
	if (GetAsyncKeyState('A') & 0x8000)
		Strafe(-delta);
	if (GetAsyncKeyState('D') & 0x8000)
		Strafe(delta);

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		RotationY(-delta);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		RotationY(delta);
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		Pitch(delta);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		Pitch(-delta);
}