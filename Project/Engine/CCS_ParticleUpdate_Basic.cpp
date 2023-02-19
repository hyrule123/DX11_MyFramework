#include "pch.h"
#include "CCS_ParticleUpdate_Basic.h"

#include "CStructBuffer.h"
#include "CConstBuffer.h"

#include "CDevice.h"

#include "CTexture.h"

#include "CParticleSystem.h"
#include "CTransform.h"

#include "CResMgr.h"
#include "strKeys.h"

CCS_ParticleUpdate_Basic::CCS_ParticleUpdate_Basic()
	: CCS_ParticleUpdate_Root(128u, 1u, 1u)
{
}

CCS_ParticleUpdate_Basic::~CCS_ParticleUpdate_Basic()
{
}


bool CCS_ParticleUpdate_Basic::BindDataCS()
{
	if (nullptr == m_Tex_Noise)
	{
		m_Tex_Noise = CResMgr::GetInst()->FindRes<CTexture>(RESOURCE::TEXTURE::NOISE_TEXTURE_0);
		

		assert(nullptr != m_Tex_Noise);
		const Vec2& TexSize = m_Tex_Noise->GetSize();
		SetScalarParam(MTRL_SCALAR_TEXTURE_NOISE_RESOLUTION, &TexSize);
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

void CCS_ParticleUpdate_Basic::UnBindCS()
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

