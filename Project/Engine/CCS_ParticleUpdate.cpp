#include "pch.h"
#include "CCS_ParticleUpdate.h"

#include "CStructBuffer.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CCS_ParticleUpdate::CCS_ParticleUpdate()
	: CComputeShader(128u, 1u, 1u)
	, m_pSBuffer_Transform()
	, m_pSBuffer_SharedRW()
	, m_pCBuffer_ModuleData()
{
}

CCS_ParticleUpdate::~CCS_ParticleUpdate()
{
}


bool CCS_ParticleUpdate::BindDataCS()
{
	//�� ������ �ϳ��� ��ϵǾ����� ������ return false�ؼ� excute ������ �ߴ�
	if (
		nullptr == m_pSBuffer_Transform
		||
		nullptr == m_pSBuffer_SharedRW
		||
		nullptr == m_pCBuffer_ModuleData
		)
		return false;

	//������ �׷� �� ���. ��ƼŬ�� ������ �迭 �����̹Ƿ� x������ �����带 �迭�Ѵ�.
	CalcGroupNumber(m_pSBuffer_Transform->GetElemCount(), 1u, 1u);

	//�����͸� ��ǻƮ���̴��� �ϰ������� ����
	m_pSBuffer_Transform->BindBufferUAV();

	m_pSBuffer_SharedRW->BindBufferUAV();

	m_pCBuffer_ModuleData->BindBuffer(eSHADER_PIPELINE_STAGE_FLAG::__COMPUTE);

	return true;
}

void CCS_ParticleUpdate::UnBindCS()
{
	//��� �� UAV ���ε��� ����.
	m_pSBuffer_Transform->UnBindUAV();
	m_pSBuffer_SharedRW->UnBindUAV();

	//���� ������ ���� nullptr�� ����
	m_pSBuffer_Transform = nullptr;
	m_pSBuffer_SharedRW = nullptr;
	m_pCBuffer_ModuleData = nullptr;
}



