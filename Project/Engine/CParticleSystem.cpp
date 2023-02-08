#include "pch.h"
#include "CParticleSystem.h"

#include "CResMgr.h"

#include "CStructBuffer.h"

#include "strKeys.h"

#include "CTransform.h"

#include "CCS_ParticleUpdate.h"

#include "CTimeMgr.h"

//������� ������
#include "CDevice.h"
#include "CConstBuffer.h"


CParticleSystem::CParticleSystem()
	: CRenderComponent(eCOMPONENT_TYPE::PARTICLE_SYSTEM)
	, m_ParticleModuleData{}
	, m_AccTime()
	, m_bIsCreated()
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(RESOURCE::MESH::POINT));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::PARTICLE_RENDER));


	UINT8 ShaderTarget = eSHADER_PIPELINE_STAGE_FLAG::__GEOMETRY | eSHADER_PIPELINE_STAGE_FLAG::__PIXEL;
	m_pSBuffer_ParticleInfo = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, ShaderTarget, eSBUFFER_SHARED_CBUFFER_IDX::PARTICLE, eSRV_REGISTER_IDX::PARTICLE_INFO, eUAV_REGISTER_IDX::PARTICLE_SBUFFER);

	//��ǻƮ���̴� ����
	m_pSBuffer_SharedRW = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, eSHADER_PIPELINE_STAGE_FLAG::__NONE, eSBUFFER_SHARED_CBUFFER_IDX::NONE, eSRV_REGISTER_IDX::NONE, eUAV_REGISTER_IDX::PARTICLE_SBUFFER_SHARED);
}

CParticleSystem::~CParticleSystem()
{
	DESTRUCTOR_DELETE(m_pSBuffer_ParticleInfo);
	DESTRUCTOR_DELETE(m_pSBuffer_SharedRW);
}

void CParticleSystem::init()
{
}

void CParticleSystem::finaltick()
{
	if (nullptr == m_pCSParticle || false == m_bIsCreated)
		return;

	//���� ���ε��� ���̴� Ŭ�������� �ϰ������� ������.
	//����� ������ ���ε常 ���.

	//��ⵥ���� ����
	static CConstBuffer* const s_CBuffer_ModuleData = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_TYPE::PARTICLE_MODULEDATA);
	s_CBuffer_ModuleData->UploadData(&m_ParticleModuleData);


	//� �������� ������ SharedRW ���ۿ� ��Ƽ� ����
	float fSpawnCountPerTime = 1.f / (float)m_ParticleModuleData.SpawnRate;
	m_AccTime += DELTA_TIME;

	if (fSpawnCountPerTime < m_AccTime)
	{
		float fData = m_AccTime / fSpawnCountPerTime;

		m_AccTime = fSpawnCountPerTime * (fData - floor(fData));

		tRWParticleBuffer rwbuffer = { (int)fData, };

		m_pSBuffer_SharedRW->UploadData(&rwbuffer, 1u);
	}


	static_cast<CCS_ParticleUpdate*>(m_pCSParticle.Get())->SetBuffers(m_pSBuffer_ParticleInfo, m_pSBuffer_SharedRW, s_CBuffer_ModuleData);

	//��ƼŬ ��ġ������ ����Ŵ.
	m_pCSParticle->Execute();

	tParticle arrParticle[100] = { };
	m_pSBuffer_ParticleInfo->GetData(arrParticle, 100u);
}

void CParticleSystem::render()
{
	if (nullptr == m_pCSParticle || false == m_bIsCreated)
		return;

	Transform()->UpdateData();

	GetMaterial()->BindData();

	m_pSBuffer_ParticleInfo->BindBufferSRV();

	GetMesh()->renderInstanced(m_ParticleModuleData.iMaxParticleCount);
}


void CParticleSystem::CreateParticle()
{
	m_bIsCreated = true;

	//�ϴ� �ϳ��ۿ� �����Ƿ� ��ǻƮ���̴��� ����(���߿� ����)
	SetParticleCS(RESOURCE::SHADER::COMPUTE::PARTICLE_UPDATE);


	// ��ƼŬ ���� �ʱ� �����͵� �켱 ���������� ����
	tParticle arrParticle[100] = { };
	float fAngle = XM_2PI / 100.f;
	float fRadius = 20.f;
	float fSpeed = 100.f;

	for (UINT i = 0; i < 100; ++i)
	{
		arrParticle[i].vWorldPos = Vec3(fRadius * cosf(fAngle * (float)i), fRadius * sinf(fAngle * (float)i), 0.f);
		arrParticle[i].vVelocity = arrParticle[i].vWorldPos;
		arrParticle[i].vVelocity.z = 0.f;
		arrParticle[i].vVelocity.Normalize();
		arrParticle[i].vVelocity *= fSpeed;
		arrParticle[i].vWorldScale = Vec3(10.f, 10.f, 1.f);
		arrParticle[i].Age = -1.f;
	}
	m_pSBuffer_ParticleInfo->Create((UINT)sizeof(tParticle), 100u, arrParticle, 100u);


	//��� �����͵� �ϵ��ڵ�(�� finaltick���� ������Ʈ)
	m_ParticleModuleData.vSpawnColor = Vec4(1.f, 0.f, 0.f, 1.f);
	m_ParticleModuleData.iMaxParticleCount = 100;
	m_ParticleModuleData.SpawnRate = 10;


	//���� ������ ����ȭ ���� ����
	tRWParticleBuffer rwbuffer = { (int)0,};
	m_pSBuffer_SharedRW->Create((UINT)sizeof(tRWParticleBuffer), 1, &rwbuffer, 1u);

}

void CParticleSystem::SetParticleCS(const string& _ResKeyCS)
{
	m_pCSParticle = CResMgr::GetInst()->FindRes<CComputeShader>(_ResKeyCS);
}
