#include "pch.h"
#include "CParticleSystem.h"

#include "CResMgr.h"

#include "CStructBuffer.h"

#include "strKey_Default.h"

#include "CTransform.h"

#include "CCS_ParticleUpdate.h"

#include "CTimeMgr.h"
#include "CRandMgr.h"

//상수버퍼 생성용
#include "CDevice.h"
#include "CConstBuffer.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(eCOMPONENT_TYPE::PARTICLE_SYSTEM)
	, m_tModuleData{}
	, m_AccTime()
	, m_bIsCreated()
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(string(strKey_RES_DEFAULT::MESH::POINT)));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(string(strKey_RES_DEFAULT::MATERIAL::PARTICLE_RENDER)));


	UINT ShaderTarget = eSHADER_PIPELINE_STAGE::__GEOMETRY | eSHADER_PIPELINE_STAGE::__PIXEL;
	m_pSBufferRW_ParticleTransform = new CStructBuffer(tSBufferDesc{ eSTRUCT_BUFFER_TYPE::READ_WRITE, ShaderTarget, eCBUFFER_SBUFFER_SHAREDATA_IDX::PARTICLE, idx_t_SBUFFER_PARTICLE_TRANSFORM, idx_u_PARTICLE_SBUFFERRW });

	//컴퓨트쉐이더 전용
	m_pSBufferRW_Shared = new CStructBuffer(tSBufferDesc{ eSTRUCT_BUFFER_TYPE::READ_WRITE, eSHADER_PIPELINE_STAGE::__NONE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE,idx_t_SRV_NONE, idx_u_PARTICLE_SBUFFERRW_SHAREDATA });
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_pSBufferRW_ParticleTransform);
	SAFE_DELETE(m_pSBufferRW_Shared);
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
	static CConstBuffer* const s_CBuffer_ModuleData = CDevice::GetInst()->GetConstBuffer(idx_b_CBUFFER_PARTICLE_MODULEDATA);
	s_CBuffer_ModuleData->UploadData(&m_tModuleData);
	//s_CBuffer_ModuleData->BindBuffer(eSHADER_PIPELINE_STAGE::__ALL);


	//몇개 스폰할지 정보를 SharedRW 버퍼에 담아서 전송
	float fSpawnCountPerTime = 1.f / (float)m_tModuleData.iSpawnRate;
	m_AccTime += DELTA_TIME;

	if (fSpawnCountPerTime < m_AccTime)
	{
		float fData = m_AccTime / fSpawnCountPerTime;

		m_AccTime = fSpawnCountPerTime * (fData - floor(fData));

		CRandMgr* pRandMgr = CRandMgr::GetInst();

		//GPU에서 사용할 시드값을 전달
		tParticleShareData rwbuffer = { (int)fData, UINT32_2(pRandMgr->GetRand<UINT>(0u, UINT_MAX), pRandMgr->GetRand<UINT>(0u, UINT_MAX)), };

		m_pSBufferRW_Shared->UploadData(&rwbuffer, 1u);
	}

	m_pCSParticle->SetBuffers(this, m_pSBufferRW_ParticleTransform, m_pSBufferRW_Shared, s_CBuffer_ModuleData);

	//파티클 위치정보를 계산시킴.
	m_pCSParticle->Execute();

	//렌더링하고 나면 PrevPos를 업데이트
	m_tModuleData.vOwnerPrevWorldPos = m_tModuleData.vOwnerCurWorldPos;
	m_tModuleData.vOwnerCurWorldPos = Transform().GetWorldPos();
}

bool CParticleSystem::render()
{
	//true 반환해서 인스턴싱 필요없다고 전달
	if (nullptr == m_pCSParticle || false == m_bIsCreated)
		return true;

	//Transform().UpdateData();

	CMaterial* pMtrl = GetCurMaterial().Get();
	pMtrl->BindData();

	BindMtrlScalarDataToCBuffer();

	m_pSBufferRW_ParticleTransform->BindBufferSRV();

	GetMesh()->renderInstanced(m_tModuleData.iMaxParticleCount);

	//드로우콜이 발생했으므로 -> 인스턴싱 필요 없다고 반환
	return true;
}



void CParticleSystem::CreateParticle()
{
	assert(nullptr != m_pCSParticle);

	m_bIsCreated = true;

	m_tModuleData.iMaxParticleCount = 200;

	//파티클을 처리할 버퍼 생성
	m_pSBufferRW_ParticleTransform->Create(sizeof(tParticleTransform), m_tModuleData.iMaxParticleCount, nullptr, 0u);

	m_tModuleData.bModule_Spawn = TRUE;

	m_tModuleData.iSpawnRate = 30;

	m_tModuleData.vSpawnColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.vSpawnScaleMin = Vec3(50.f, 50.f, 1.f);
	m_tModuleData.vSpawnScaleMax = Vec3(100.f, 100.f, 1.f);

	m_tModuleData.eSpawnShapeType = 0;
	m_tModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);
	m_tModuleData.bFollowing = 0; // 시뮬레이션 좌표계

	m_tModuleData.fMinLifeTime = 3.f;
	m_tModuleData.fMaxLifeTime = 5.f;

	m_tModuleData.bModule_ScaleChange = TRUE;
	m_tModuleData.fStartScale = 2.f;
	m_tModuleData.fEndScale = 0.1f;

	m_tModuleData.bModule_ColorChange = TRUE;
	m_tModuleData.vStartColor = Vec3(0.2f, 0.3f, 1.0f);
	m_tModuleData.vEndColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.bModule_AddVelocity = TRUE;
	m_tModuleData.eAddVelocityType = 0; // From Center
	m_tModuleData.fSpeed = 500.f;
	m_tModuleData.vVelocityDir;
	m_tModuleData.fOffsetAngle;

	m_tModuleData.bModule_Drag = TRUE;
	m_tModuleData.fStartDrag = 200.f;
	m_tModuleData.fEndDrag = 0.f;

	m_tModuleData.bModule_Rotation = TRUE;
	m_tModuleData.vRotRadPerSec = Vec3(0.f, 0.f, 10.f);
	m_tModuleData.vRotRandomRange = Vec3(0.f, 0.f, 0.3f);

	m_tModuleData.bModule_ApplyGravity = TRUE;
	m_tModuleData.bModule_ApplyGravity = TRUE;
	m_tModuleData.fGravity = 9.8f;


	//노이즈 텍스처 지정


	//공유 데이터 구조화 버퍼 생성
	CRandMgr* pRandMgr = CRandMgr::GetInst();
	tParticleShareData rwbuffer = { (int)0, UINT32_2(pRandMgr->GetRand<UINT>(0u, UINT_MAX), pRandMgr->GetRand<UINT>(0u, UINT_MAX)), };


	m_pSBufferRW_Shared->Create((UINT)sizeof(tParticleShareData), 1, &rwbuffer, 1u);
}

void CParticleSystem::SetParticleCS(const string_view _ResKeyCS)
{
	m_pCSParticle = CResMgr::GetInst()->FindRes<CComputeShader>(_ResKeyCS);
}
