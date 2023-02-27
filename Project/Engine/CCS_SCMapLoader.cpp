#include "pch.h"
#include "CCS_SCMapLoader.h"

CCS_SCMapLoader::CCS_SCMapLoader()
	: CComputeShader(32u, 32u, 1u)	//메가타일 사이즈 = 32 * 32
{
}

CCS_SCMapLoader::~CCS_SCMapLoader()
{
}

bool CCS_SCMapLoader::BindDataCS()
{
	return false;
}

void CCS_SCMapLoader::UnBindCS()
{
}
