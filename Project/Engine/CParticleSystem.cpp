#include "pch.h"
#include "CParticleSystem.h"

#include "CResMgr.h"

#include "CStructBuffer.h"

#include "strKeys.h"

#include "CTransform.h"

#include "CCS_ParticleUpdate_Basic.h"

#include "CTimeMgr.h"

//상수버퍼 생성용
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

	//컴퓨트쉐이더 전용
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

	//버퍼 바인딩은 쉐이더 클래스에서 일괄적으로 진행함.
	//여기는 데이터 업로드만 담당.

	//모듈데이터 전송
	static CConstBuffer* const s_CBuffer_ModuleData = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_PARTICLE_MODULEDATA);
	s_CBuffer_ModuleData->UploadData(&m_tModuleData);
	s_CBuffer_ModuleData->BindBuffer(eSHADER_PIPELINE_STAGE::__ALL);


	//몇개 스폰할지 정보를 SharedRW 버퍼에 담아서 전송
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

	//파티클 위치정보를 계산시킴.
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

	//일단 하나밖에 없으므로 컴퓨트쉐이더는 고정(나중에 해제)
	SetParticleCS(RESOURCE::SHADER::COMPUTE::PARTICLE_UPDATE);

	m_tModuleData.iMaxParticleCount = 100;

	//파티클을 처리할 버퍼 생성
	m_pSBufferRW_ParticleTransform->Create(sizeof(tParticleTransform), 100u, nullptr, 0u);

	m_tModuleData.bModule_Spawn = TRUE;

	m_tModuleData.iSpawnRate = 10;

	m_tModuleData.vSpawnColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.vSpawnScaleMin = Vec3(15.f, 15.f, 1.f);
	m_tModuleData.vSpawnScaleMax = Vec3(50.f, 50.f, 1.f);

	m_tModuleData.eSpawnShapeType = 0;
	m_tModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);
	m_tModuleData.bFollowing = 0; // 시뮬레이션 좌표계

	m_tModuleData.fMinLifeTime = 1.f;
	m_tModuleData.fMaxLifeTime = 3.f;

	m_tModuleData.bModule_ScaleChange = TRUE;
	m_tModuleData.fStartScale = 2.f;
	m_tModuleData.fEndScale = 0.1f;

	m_tModuleData.bModule_ColorChange = TRUE;
	m_tModuleData.vStartColor = Vec3(0.2f, 0.3f, 1.0f);
	m_tModuleData.vEndColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.bModule_AddVelocity = TRUE;
	m_tModuleData.eAddVelocityType = 0; // From Center
	m_tModuleData.fSpeed = 150.f;
	m_tModuleData.vVelocityDir;
	m_tModuleData.fOffsetAngle;

	m_tModuleData.bModule_Drag = TRUE;
	m_tModuleData.fStartDrag = 200.f;
	m_tModuleData.fEndDrag = 0.f;

	m_tModuleData.bModule_Rotation = TRUE;
	m_tModuleData.vRotRadPerSec = Vec3(0.f, 0.f, 0.5f);
	m_tModuleData.vRotRandomRange = Vec3(0.f, 0.f, 0.3f);


	//노이즈 텍스처 지정


	//공유 데이터 구조화 버퍼 생성
	tRWParticleBuffer rwbuffer = { (int)0, };
	m_pSBufferRW_Shared->Create((UINT)sizeof(tRWParticleBuffer), 1, &rwbuffer, 1u);

}

void CParticleSystem::SetParticleCS(const string& _ResKeyCS)
{
	m_pCSParticle = CResMgr::GetInst()->FindRes<CComputeShader>(_ResKeyCS);
}
