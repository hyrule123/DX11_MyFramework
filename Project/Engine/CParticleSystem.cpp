#include "pch.h"
#include "CParticleSystem.h"

#include "CResMgr.h"

#include "CStructBuffer.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(eCOMPONENT_TYPE::PARTICLE_SYSTEM)
	, m_pParticleCS()
{
}

CParticleSystem::~CParticleSystem()
{
}

void CParticleSystem::init()
{
	
}

void CParticleSystem::finaltick()
{
}

void CParticleSystem::render()
{
}

void CParticleSystem::cleanup()
{
}

void CParticleSystem::SetParticleCS(const string& _ResKey)
{
	m_pParticleCS = CResMgr::GetInst()->FindRes<CComputeShader>("ParticleUpdate");
	assert(nullptr != m_pParticleCS);
}
