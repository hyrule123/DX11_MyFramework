#include "pch.h"
#include "CCS_ParticleUpdate.h"

#include "CStructBuffer.h"

#include "CDevice.h"

CCS_ParticleUpdate::CCS_ParticleUpdate(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: CComputeShader(_iGroupPerThreadX, _iGroupPerThreadY, _iGroupPerThreadZ)
{
}

CCS_ParticleUpdate::~CCS_ParticleUpdate()
{
}


bool CCS_ParticleUpdate::BindDataCS()
{
	if (nullptr == m_ParticleBuffer)
		return false;

	//스레드 그룹 수 계산
	CalcGroupNumber(m_ParticleBuffer->GetElemCount(), 1, 1);

	//데이터를 바인딩
	m_ParticleBuffer->BindBufferUAV();

	return true;
}

void CCS_ParticleUpdate::UnBindCS()
{
	//계산 후 UAV 바인딩을 해제.
	m_ParticleBuffer->UnBindUAV();
}

void CCS_ParticleUpdate::SetParticleBuffer(CStructBuffer* _pBuffer)
{
	if (nullptr == _pBuffer)
		return;

	m_ParticleBuffer = _pBuffer;
	m_ParticleBuffer->AddPipelineTarget(eSHADER_PIPELINE_STAGE::__COMPUTE);
}
