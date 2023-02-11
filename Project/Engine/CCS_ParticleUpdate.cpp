#include "pch.h"
#include "CCS_ParticleUpdate.h"

#include "CStructBuffer.h"
#include "CConstBuffer.h"

#include "CDevice.h"

#include "CTexture.h"

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
	//세 버퍼중 하나라도 등록되어있지 않으면 return false해서 excute 과정을 중단
	if (
		nullptr == m_pSBuffer_Transform
		||
		nullptr == m_pSBuffer_SharedRW
		||
		nullptr == m_pCBuffer_ModuleData
		||
		nullptr == m_Tex_Noise
		)
		return false;

	//스레드 그룹 수 계산. 파티클은 무조건 배열 형태이므로 x축으로 스레드를 배열한다.
	CalcGroupNumber(m_pSBuffer_Transform->GetElemCount(), 1u, 1u);

	//데이터를 컴퓨트쉐이더에 일괄적으로 전달
	m_pSBuffer_Transform->BindBufferUAV();

	m_pSBuffer_SharedRW->BindBufferUAV();

	m_pCBuffer_ModuleData->BindBuffer(eSHADER_PIPELINE_STAGE_FLAG::__COMPUTE);

	m_Tex_Noise->BindData_CS((int)eSRV_REGISTER_IDX::NOISE_TEXTURE);

	return true;
}

void CCS_ParticleUpdate::UnBindCS()
{
	//계산 후 UAV 바인딩을 해제.
	m_pSBuffer_Transform->UnBindUAV();
	m_pSBuffer_SharedRW->UnBindUAV();

	//에러 방지를 위해 nullptr로 변경
	m_pSBuffer_Transform = nullptr;
	m_pSBuffer_SharedRW = nullptr;
	m_pCBuffer_ModuleData = nullptr;
}



