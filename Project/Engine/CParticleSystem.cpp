#include "pch.h"
#include "CParticleSystem.h"

#include "CResMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(eCOMPONENT_TYPE::PARTICLE_SYSTEM)
{

}

CParticleSystem::~CParticleSystem()
{
}

void CParticleSystem::init()
{
	//m_pParticleCS = CResMgr::GetInst()->FindRes<CComputeShader>()
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
