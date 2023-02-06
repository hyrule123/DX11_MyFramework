#include "pch.h"
#include "CCS_ParticleUpdate.h"

#include "CStructBuffer.h"

#include "CDevice.h"

CCS_ParticleUpdate::CCS_ParticleUpdate()
	: CComputeShader(128u, 1u, 1u)
	, m_pParticleSBuffer()
{
	UINT8 flag = eSHADER_PIPELINE_STAGE::__ALL;
	m_pParticleSBuffer = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, flag, eSBUFFER_SHARED_CBUFFER_IDX::PARTICLE, eSRV_REGISTER_IDX::PARTICLE, eUAV_REGISTER_IDX::PARTICLE_SBUFFER);
}

CCS_ParticleUpdate::~CCS_ParticleUpdate()
{
	delete m_pParticleSBuffer;
}


bool CCS_ParticleUpdate::BindDataCS()
{
	if (nullptr == m_pParticleSBuffer)
		return false;

	//������ �׷� �� ���
	CalcGroupNumber(m_pParticleSBuffer->GetElemCount(), 1, 1);

	//�����͸� ���ε�
	m_pParticleSBuffer->BindBufferUAV();

	return true;
}

void CCS_ParticleUpdate::UnBindCS()
{
	//��� �� UAV ���ε��� ����.
	m_pParticleSBuffer->UnBindUAV();
}

void CCS_ParticleUpdate::SetData(void* _pData, UINT _uCount)
{
	if (nullptr == _pData)
		return;

	m_pParticleSBuffer->UploadData(_pData, _uCount);
	m_pParticleSBuffer->AddPipelineTarget(eSHADER_PIPELINE_STAGE::__COMPUTE);
}
