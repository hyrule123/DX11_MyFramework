#include "pch.h"
#include "CCS_ParticleUpdate.h"

#include "CResMgr.h"

#include "strKeyDefaultRes.h"
#include "CStructBuffer.h"
#include "CConstBuffer.h"
#include "CGameObject.h"
#include "CParticleSystem.h"
#include "CTransform.h"

CCS_ParticleUpdate::CCS_ParticleUpdate(UINT _uNumThreadsX, UINT _uNumThreadsY, UINT _uNumThreadsZ)
	: CComputeShader(_uNumThreadsX, _uNumThreadsY, _uNumThreadsZ)
	, m_pBufferOwner()
	, m_pSBuffer_Transform()
	, m_pSBufferRW_Shared()
	, m_pCBuffer_ModuleData()
{
}

CCS_ParticleUpdate::~CCS_ParticleUpdate()
{
}

bool CCS_ParticleUpdate::BindDataCS()
{
	if (nullptr == m_Tex_Noise)
	{
		m_Tex_Noise = CResMgr::GetInst()->FindRes<CTexture>(DEFAULT_RES::TEXTURE::NOISE_TEXTURE_0);


		assert(nullptr != m_Tex_Noise);
		const Vec2& TexSize = m_Tex_Noise->GetSize();
		SetMtrlScalarParam(MTRL_SCALAR_TEXTURE_NOISE_RESOLUTION, &TexSize);
	}


	//세 버퍼중 하나라도 등록되어있지 않으면 return false해서 excute 과정을 중단
	if (
		nullptr == m_pSBuffer_Transform
		||
		nullptr == m_pSBufferRW_Shared
		||
		nullptr == m_pCBuffer_ModuleData
		)
		return false;

	//파티클 컴포넌트의 월드위치를 컴퓨트쉐이더의 버퍼에 담아서 전달한다.
	SetParticleOwnerPos(m_pBufferOwner->Transform()->GetWorldPos());

	//스레드 그룹 수 계산. 파티클은 무조건 배열 형태이므로 x축으로 스레드를 배열한다.
	CalcGroupNumber(m_pSBuffer_Transform->GetCapacity(), 1u, 1u);

	//데이터를 컴퓨트쉐이더에 일괄적으로 전달
	m_pSBuffer_Transform->BindBufferUAV();

	m_pSBufferRW_Shared->BindBufferUAV();

	m_pCBuffer_ModuleData->BindBuffer(eSHADER_PIPELINE_STAGE::__COMPUTE);

	m_Tex_Noise->BindData_SRV(e_t_TEXUTRE_NOISE, eSHADER_PIPELINE_STAGE::__COMPUTE);

	return true;
}

void CCS_ParticleUpdate::UnBindCS()
{
	//계산 후 UAV 바인딩을 해제.
	m_pSBuffer_Transform->UnBindUAV();
	m_pSBufferRW_Shared->UnBindUAV();

	//에러 방지를 위해 nullptr로 변경
	m_pBufferOwner = nullptr;
	m_pSBuffer_Transform = nullptr;
	m_pSBufferRW_Shared = nullptr;
	m_pCBuffer_ModuleData = nullptr;
}
