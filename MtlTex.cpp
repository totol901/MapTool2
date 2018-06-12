#include "stdafx.h"
#include "MtlTex.h"


MtlTex::MtlTex()
	:m_texture(nullptr)
{
	ZeroMemory(&m_mat, sizeof(TAG_MATERIAL));
}

MtlTex::~MtlTex()
{
}
