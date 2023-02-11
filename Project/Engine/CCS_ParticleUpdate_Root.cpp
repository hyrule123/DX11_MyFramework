#include "pch.h"
#include "CCS_ParticleUpdate_Root.h"

CCS_ParticleUpdate_Root::CCS_ParticleUpdate_Root(UINT _uNumThreadsX, UINT _uNumThreadsY, UINT _uNumThreadsZ)
	: CComputeShader(_uNumThreadsX, _uNumThreadsY, _uNumThreadsZ)
	, m_pBufferOwner()
	, m_pSBuffer_Transform()
	, m_pSBufferRW_Shared()
	, m_pCBuffer_ModuleData()
{
}

CCS_ParticleUpdate_Root::~CCS_ParticleUpdate_Root()
{
}
