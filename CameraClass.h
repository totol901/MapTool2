#pragma once

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetPosition(D3DXVECTOR3 vec);
	void SetRotation(float, float, float);
	void SetRotation(D3DXVECTOR3 rot);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);
	void GetView_WorldMatrix(D3DXMATRIX& world);

	void SetTransCarmeraPosX(float speedX);
	void SetTransCarmeraPosZ(float speedZ);
	void SetBaseMat(D3DXMATRIX mat){m_baseMat = mat;}
	D3DXMATRIX GetBaseMat(){return m_baseMat;}
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
	D3DXVECTOR3 m_LookAt;
	D3DXMATRIX m_baseMat;

};

