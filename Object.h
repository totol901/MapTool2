#pragma once

class Object
{
protected:
	D3DXVECTOR3 mRight;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mForward;

	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mScale;

	D3DXMATRIX mLocal;
	D3DXMATRIX mWorld;

	Object* mParent;
	vector<Object*> mChilds;

	Material mMaterial;
public:
	Object();
	virtual ~Object();

	virtual void Update(const float deltaTime = 0.0f);

	void SetPosition(D3DXVECTOR3 pos);
	void MovePosition(D3DXVECTOR3 delta);
	void SetScale(D3DXVECTOR3 scale);
	void SetRotation(D3DXVECTOR3 rot);

	void LookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));
	void LookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));

	void Walk(float delta);
	void Strafe(float delta);

	void Pitch(float angle);
	void RotationY(float angle);

	VOID MouseMove(POINT pt);

	VOID Moving(float delta = 0.0f);

	const D3DXVECTOR3& GetPosition() { return mPosition; }
	const D3DXVECTOR3& GetForward() { return mForward; }
	const D3DXVECTOR3& GetScale() { return mScale; }
	const D3DXVECTOR3& GetUp() { return mUp; }
	const D3DXVECTOR3& GetRight() { return mRight; }

	const D3DXMATRIX& GetLocalMatrix() { return mLocal; }
	const D3DXMATRIX& GetWorldMatrix() { return mWorld; }

	Object* GetThis() { return this; }
};
