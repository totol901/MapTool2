#include "stdafx.h"
#include "LightClass.h"


LightClass::LightClass()
{
}


LightClass::~LightClass()
{
}


void LightClass::SetAmbientColor(D3DXVECTOR4 color)
{
	m_ambientColor = color;
}


void LightClass::SetDiffuseColor(D3DXVECTOR4 color)
{
	m_diffuseColor = color;
}


void LightClass::SetDirection(D3DXVECTOR3 direction)
{
	m_direction = direction;
}


D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}


D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}


D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}