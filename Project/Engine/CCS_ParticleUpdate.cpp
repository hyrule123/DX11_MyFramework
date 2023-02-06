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

	//������ �׷� �� ���
	CalcGroupNumber(m_ParticleBuffer->GetElemCount(), 1, 1);

	//�����͸� ���ε�
	m_ParticleBuffer->BindBufferUAV();

	return true;
}

void CCS_ParticleUpdate::UnBindCS()
{
	//��� �� UAV ���ε��� ����.
	m_ParticleBuffer->UnBindUAV();
}

void CCS_ParticleUpdate::SetParticleBuffer(CStructBuffer* _pBuffer)
{
	if (nullptr == _pBuffer)
		return;

	m_ParticleBuffer = _pBuffer;
	m_ParticleBuffer->AddPipelineTarget(eSHADER_PIPELINE_STAGE::__COMPUTE);
}
