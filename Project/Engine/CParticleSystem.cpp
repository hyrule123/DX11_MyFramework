#include "pch.h"
#include "CParticleSystem.h"

#include "CResMgr.h"

#include "CStructBuffer.h"

#include "strKeys.h"

#include "CTransform.h"

#include "CCS_ParticleUpdate.h"

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


	UINT8 ShaderTarget = eSHADER_PIPELINE_STAGE_FLAG::__GEOMETRY | eSHADER_PIPELINE_STAGE_FLAG::__PIXEL;
	m_pSBuffer_ParticleInfo = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, ShaderTarget, eSBUFFER_SHARED_CBUFFER_IDX::PARTICLE, eSRV_REGISTER_IDX::PARTICLE_INFO, eUAV_REGISTER_IDX::PARTICLE_SBUFFER);

	//컴퓨트쉐이더 전용
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

	//버퍼 바인딩은 쉐이더 클래스에서 일괄적으로 진행함.
	//여기는 데이터 업로드만 담당.

	//모듈데이터 전송
	static CConstBuffer* const s_CBuffer_ModuleData = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_TYPE::PARTICLE_MODULEDATA);
	s_CBuffer_ModuleData->UploadData(&m_tModuleData);
	s_CBuffer_ModuleData->BindBuffer(eSHADER_PIPELINE_STAGE_FLAG::__ALL);


	//몇개 스폰할지 정보를 SharedRW 버퍼에 담아서 전송
	float fSpawnCountPerTime = 1.f / (float)m_tModuleData.iSpawnRate;
	m_AccTime += DELTA_TIME;

	if (fSpawnCountPerTime < m_AccTime)
	{
		float fData = m_AccTime / fSpawnCountPerTime;

		m_AccTime = fSpawnCountPerTime * (fData - floor(fData));

		tRWParticleBuffer rwbuffer = { (int)fData, };

		m_pSBuffer_SharedRW->UploadData(&rwbuffer, 1u);
	}


	static_cast<CCS_ParticleUpdate*>(m_pCSParticle.Get())->SetBuffers(m_pSBuffer_ParticleInfo, m_pSBuffer_SharedRW, s_CBuffer_ModuleData);

	//파티클 위치정보를 계산시킴.
	m_pCSParticle->Execute();
}

void CParticleSystem::render()
{
	if (nullptr == m_pCSParticle || false == m_bIsCreated)
		return;

	Transform()->UpdateData();

	GetMaterial()->BindData();

	m_pSBuffer_ParticleInfo->BindBufferSRV();

	GetMesh()->renderInstanced(m_tModuleData.iMaxParticleCount);
}


void CParticleSystem::CreateParticle()
{
	m_bIsCreated = true;

	//일단 하나밖에 없으므로 컴퓨트쉐이더는 고정(나중에 해제)
	SetParticleCS(RESOURCE::SHADER::COMPUTE::PARTICLE_UPDATE);

	m_tModuleData.iSpawnRate = 20;

	m_tModuleData.vSpawnColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.vSpawnScaleMin = Vec3(15.f, 15.f, 1.f);
	m_tModuleData.vSpawnScaleMax = Vec3(50.f, 50.f, 1.f);

	m_tModuleData.eSpawnShapeType = 0;
	m_tModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);
	m_tModuleData.bFollowing = 0; // 시뮬레이션 좌표계

	m_tModuleData.fMinLifeTime = 1.f;
	m_tModuleData.fMaxLifeTime = 3.f;

	m_tModuleData.bScaleChange = true;
	m_tModuleData.fStartScale = 2.f;
	m_tModuleData.fEndScale = 0.1f;

	m_tModuleData.bColorChange = true;
	m_tModuleData.vStartColor = Vec3(0.2f, 0.3f, 1.0f);
	m_tModuleData.vEndColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.bAddVelocity = true;
	m_tModuleData.eAddVelocityType = 0; // From Center
	m_tModuleData.fSpeed = 150.f;
	m_tModuleData.vVelocityDir;
	m_tModuleData.fOffsetAngle;

	m_tModuleData.bDrag;
	m_tModuleData.fStartDrag = 200.f;
	m_tModuleData.fEndDrag = 0.f;



	//공유 데이터 구조화 버퍼 생성
	tRWParticleBuffer rwbuffer = { (int)0,};
	m_pSBuffer_SharedRW->Create((UINT)sizeof(tRWParticleBuffer), 1, &rwbuffer, 1u);

}

void CParticleSystem::SetParticleCS(const string& _ResKeyCS)
{
	m_pCSParticle = CResMgr::GetInst()->FindRes<CComputeShader>(_ResKeyCS);
}
