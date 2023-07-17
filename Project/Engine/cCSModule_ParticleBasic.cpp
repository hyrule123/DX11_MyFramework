#include "pch.h"
#include "cCSModule_ParticleBasic.h"

#include "cResMgr.h"

#include "strKey_Default.h"
#include "cStructBuffer.h"
#include "cConstBuffer.h"
#include "cGameObject.h"
#include "cCom_Renderer_ParticleBasic.h"
#include "cCom_Transform.h"

#ifdef _DEBUG
#include "DefaultShader/S_C_ParticleBasic_Debug.h"
#else
#include "DefaultShader/S_C_ParticleBasic_Release.h"
#endif

cCSModule_ParticleBasic::cCSModule_ParticleBasic()
	: m_SBufferRW_Shared(std::make_unique<cStructBuffer>())
	, m_SBuffer_ParticleSpawnSetting(std::make_unique<cStructBuffer>())
	, m_SBufferRW_Transform(std::make_unique<cStructBuffer>())
	, m_Tex_Noise()
{
}

cCSModule_ParticleBasic::~cCSModule_ParticleBasic()
{
}

cCSModule_ParticleBasic::cCSModule_ParticleBasic(cCSModule_ParticleBasic const& _other)
	: cShaderDataModule(_other)
	, m_SBufferRW_Shared(_other.m_SBufferRW_Shared->Clone())
	, m_SBuffer_ParticleSpawnSetting(_other.m_SBuffer_ParticleSpawnSetting->Clone())
	, m_SBufferRW_Transform(_other.m_SBufferRW_Transform->Clone())
	, m_Tex_Noise(_other.m_Tex_Noise)
{
}


tNumDataCS cCSModule_ParticleBasic::BindDataCS()
{
	//세 버퍼중 하나라도 등록되어있지 않으면 return false해서 excute 과정을 중단
	if (
		nullptr == m_SBufferRW_Transform
		||
		nullptr == m_SBufferRW_Shared
		||
		nullptr == m_SBuffer_ParticleSpawnSetting
		)
		return tNumDataCS(0u, 0u, 0u);


	//데이터를 컴퓨트쉐이더에 일괄적으로 전달
	m_SBufferRW_Shared->BindBufferUAV();
	m_SBuffer_ParticleSpawnSetting->BindBufferSRV();
	m_SBufferRW_Transform->BindBufferUAV();

	m_Tex_Noise->BindData_SRV(REGISLOT_t_TEXUTRE_NOISE, define_Shader::ePIPELINE_STAGE_FLAG::__COMPUTE);

	return tNumDataCS(m_SBufferRW_Transform->GetCapacity(), 1u, 1u);
}

bool cCSModule_ParticleBasic::BindDataGS()
{
	m_SBufferRW_Transform->BindBufferSRV();

	return false;
}

void cCSModule_ParticleBasic::UnBind()
{
	//계산 후 UAV 바인딩을 해제.
	m_SBufferRW_Transform->UnBind();
	m_SBufferRW_Shared->UnBind();
	m_SBuffer_ParticleSpawnSetting->UnBind();
}

