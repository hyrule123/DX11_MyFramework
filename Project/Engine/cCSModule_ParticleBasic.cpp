#include "pch.h"
#include "cCSModule_ParticleBasic.h"

#include "cResMgr.h"

#include "strKey_Default.h"
#include "cStructBuffer.h"
#include "cConstBuffer.h"
#include "cGameObject.h"
#include "cParticleSystem.h"
#include "cTransform.h"

#ifdef _DEBUG
#include "DefaultShader/S_C_ParticleBasic_Debug.h"
#else
#include "DefaultShader/S_C_ParticleBasic_Release.h"
#endif

cCSModule_ParticleBasic::cCSModule_ParticleBasic()
	: cComputeShader(128u, 1u, 1u)
	, m_pBufferOwner()
	, m_pSBuffer_Transform()
	, m_pSBufferRW_Shared()
	, m_pCBuffer_ModuleData()
{

}



cCSModule_ParticleBasic::~cCSModule_ParticleBasic()
{
}

cCSModule_ParticleBasic::cCSModule_ParticleBasic(cCSModule_ParticleBasic const& _other)
{
}


bool cCSModule_ParticleBasic::Init()
{
	

	return SUCCEEDED(CreateShaderFromHeader(g_CS_Particle_Basic, sizeof(g_CS_Particle_Basic)));
}


void cCSModule_ParticleBasic::UnBind()
{
}

bool cCSModule_ParticleBasic::BindDataCS()
{
	if (nullptr == m_Tex_Noise)
	{
		m_Tex_Noise = cResMgr::GetInst()->FindRes<cTexture>(string(strKey_RES_DEFAULT::TEXTURE::NOISE_1));


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
	SetParticleOwnerPos(m_pBufferOwner->Transform().GetWorldPos());

	//스레드 그룹 수 계산. 파티클은 무조건 배열 형태이므로 x축으로 스레드를 배열한다.
	CalcGroupNumber(m_pSBuffer_Transform->GetCapacity(), 1u, 1u);

	//데이터를 컴퓨트쉐이더에 일괄적으로 전달
	m_pSBuffer_Transform->BindBufferUAV();

	m_pSBufferRW_Shared->BindBufferUAV();

	m_pCBuffer_ModuleData->BindBuffer(define_Shader::ePIPELINE_STAGE_FLAG::__COMPUTE);

	m_Tex_Noise->BindData_SRV(REGISLOT_t_TEXUTRE_NOISE, define_Shader::ePIPELINE_STAGE_FLAG::__COMPUTE);

	return true;
}

bool cCSModule_ParticleBasic::BindDataGS()
{
	return false;
}

void cCSModule_ParticleBasic::UnBindCS()
{
	//계산 후 UAV 바인딩을 해제.
	m_pSBuffer_Transform->UnBind();
	m_pSBufferRW_Shared->UnBind();

	//에러 방지를 위해 nullptr로 변경
	m_pBufferOwner = nullptr;
	m_pSBuffer_Transform = nullptr;
	m_pSBufferRW_Shared = nullptr;
	m_pCBuffer_ModuleData = nullptr;
}
