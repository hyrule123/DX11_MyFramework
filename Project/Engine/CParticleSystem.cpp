#include "pch.h"
#include "CParticleSystem.h"

#include "CResMgr.h"

#include "CStructBuffer.h"

#include "strKeys.h"

#include "CTransform.h"

#include "CCS_ParticleUpdate_Basic.h"

#include "CTimeMgr.h"

//������� ������
#include "CDevice.h"
#include "CConstBuffer.h"


CParticleSystem::CParticleSystem()
	: CRenderComponent(eCOMPONENT_TYPE::PARTICLE_SYSTEM)
	, m_tModuleData{}
	, m_AccTime()
	, m_bIsCreated()
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(RESOURCE::MESH::POINT));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::PARTICLE_RENDER));


	UINT ShaderTarget = eSHADER_PIPELINE_STAGE::__GEOMETRY | eSHADER_PIPELINE_STAGE::__PIXEL;
	m_pSBufferRW_ParticleTransform = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, ShaderTarget, eCBUFFER_SBUFFER_SHAREDATA_IDX::PARTICLE, e_t_SBUFFER_PARTICLE_TRANSFORM, e_u_PARTICLE_SBUFFERRW);

	//��ǻƮ���̴� ����
	m_pSBufferRW_Shared = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, eSHADER_PIPELINE_STAGE::__NONE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE,e_t_SRV_NONE, e_u_PARTICLE_SBUFFERRW_SHAREDATA);
}

CParticleSystem::~CParticleSystem()
{
	DESTRUCTOR_DELETE(m_pSBufferRW_ParticleTransform);
	DESTRUCTOR_DELETE(m_pSBufferRW_Shared);
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
	static CConstBuffer* const s_CBuffer_ModuleData = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_PARTICLE_MODULEDATA);
	s_CBuffer_ModuleData->UploadData(&m_tModuleData);
	s_CBuffer_ModuleData->BindBuffer(eSHADER_PIPELINE_STAGE::__ALL);


	//� �������� ������ SharedRW ���ۿ� ��Ƽ� ����
	float fSpawnCountPerTime = 1.f / (float)m_tModuleData.iSpawnRate;
	m_AccTime += DELTA_TIME;

	if (fSpawnCountPerTime < m_AccTime)
	{
		float fData = m_AccTime / fSpawnCountPerTime;

		m_AccTime = fSpawnCountPerTime * (fData - floor(fData));

		tRWParticleBuffer rwbuffer = { (int)fData, };

		m_pSBufferRW_Shared->UploadData(&rwbuffer, 1u);
	}

	m_pCSParticle->SetBuffers(this, m_pSBufferRW_ParticleTransform, m_pSBufferRW_Shared, s_CBuffer_ModuleData);

	//��ƼŬ ��ġ������ ����Ŵ.
	m_pCSParticle->Execute();
}

void CParticleSystem::render()
{
	if (nullptr == m_pCSParticle || false == m_bIsCreated)
		return;

	Transform()->UpdateData();

	GetMaterial()->BindData();

	m_pSBufferRW_ParticleTransform->BindBufferSRV();

	GetMesh()->renderInstanced(m_tModuleData.iMaxParticleCount);
}


void CParticleSystem::CreateParticle()
{
	m_bIsCreated = true;

	//�ϴ� �ϳ��ۿ� �����Ƿ� ��ǻƮ���̴��� ����(���߿� ����)
	SetParticleCS(RESOURCE::SHADER::COMPUTE::PARTICLE_UPDATE);

	m_tModuleData.iMaxParticleCount = 100;

	//��ƼŬ�� ó���� ���� ����
	m_pSBufferRW_ParticleTransform->Create(sizeof(tParticleTransform), 100u, nullptr, 0u);

	m_tModuleData.bModule_Spawn = TRUE;

	m_tModuleData.iSpawnRate = 10;

	m_tModuleData.vSpawnColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.vSpawnScaleMin = Vec3(15.f, 15.f, 1.f);
	m_tModuleData.vSpawnScaleMax = Vec3(50.f, 50.f, 1.f);

	m_tModuleData.eSpawnShapeType = 0;
	m_tModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);
	m_tModuleData.bFollowing = 0; // �ùķ��̼� ��ǥ��

	m_tModuleData.fMinLifeTime = 1.f;
	m_tModuleData.fMaxLifeTime = 3.f;

	m_tModuleData.bModule_ScaleChange = true;
	m_tModuleData.fStartScale = 2.f;
	m_tModuleData.fEndScale = 0.1f;

	m_tModuleData.bModule_ColorChange = true;
	m_tModuleData.vStartColor = Vec3(0.2f, 0.3f, 1.0f);
	m_tModuleData.vEndColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.bModule_AddVelocity = true;
	m_tModuleData.eAddVelocityType = 0; // From Center
	m_tModuleData.fSpeed = 150.f;
	m_tModuleData.vVelocityDir;
	m_tModuleData.fOffsetAngle;

	m_tModuleData.bModule_Drag = true;
	m_tModuleData.fStartDrag = 200.f;
	m_tModuleData.fEndDrag = 0.f;

	//������ �ؽ�ó ����


	//���� ������ ����ȭ ���� ����
	tRWParticleBuffer rwbuffer = { (int)0, };
	m_pSBufferRW_Shared->Create((UINT)sizeof(tRWParticleBuffer), 1, &rwbuffer, 1u);

}

void CParticleSystem::SetParticleCS(const string& _ResKeyCS)
{
	m_pCSParticle = CResMgr::GetInst()->FindRes<CComputeShader>(_ResKeyCS);
}
