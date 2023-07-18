#include "pch.h"
#include "cCom_Renderer_ParticleBasic.h"

#include "cGameObject.h"

#include "cResMgr.h"

#include "cStructBuffer.h"

#include "strKey_Default.h"

#include "cTransform.h"

#include "cCSModule_ParticleBasic.h"

#include "cTimeMgr.h"
#include "RandGen.h"

//상수버퍼 생성용
#include "cDevice.h"
#include "cConstBuffer.h"

#include "cShaderDataModule.h"


cCom_Renderer_ParticleBasic::cCom_Renderer_ParticleBasic()
	: m_ParticleDataModule()
	, m_CS()
	, m_bCreated()
{
	SetMesh(cResMgr::GetInst()->FindRes<cMesh>(string(strKey_RES_DEFAULT::MESH::POINT)));

	//기본 파티클 렌더러
	SetMaterial(cResMgr::GetInst()->FindRes<cMaterial>(string(strKey_RES_DEFAULT::MATERIAL::PARTICLE_RENDERER_BASIC)));

	m_CS = cResMgr::GetInst()->FindRes<cComputeShader>(strKey_RES_DEFAULT::SHADER::COMPUTE::PARTICLEBASIC);
	assert(nullptr != m_CS);
}

cCom_Renderer_ParticleBasic::~cCom_Renderer_ParticleBasic()
{
}

cCom_Renderer_ParticleBasic::cCom_Renderer_ParticleBasic(cCom_Renderer_ParticleBasic const& _other)
	: IRenderer(_other)
	, m_CS(_other.m_CS)
	, m_ParticleDataModule(_other.m_ParticleDataModule)
	, m_SharedData(_other.m_SharedData)
	, m_TransformData(_other.m_TransformData)
	, m_BasicData(_other.m_BasicData)
	, m_AccTime(_other.m_AccTime)
	, m_bCreated(_other.m_bCreated)
{
}


void cCom_Renderer_ParticleBasic::init()
{
}

void cCom_Renderer_ParticleBasic::finaltick()
{
	if (m_bCreated)
		return;

	//몇개 스폰할지 정보를 SharedRW 버퍼에 담아서 전송
	float fSpawnCountPerTime = 1.f / (float)m_BasicData.iSpawnRate;
	m_AccTime += DELTA_TIME;

	if (fSpawnCountPerTime < m_AccTime)
	{
		float fData = m_AccTime / fSpawnCountPerTime;

		m_AccTime = fSpawnCountPerTime * (fData - floor(fData));

		m_SharedData.iSpawnCount = (int)fData;

		m_ParticleDataModule.m_SBufferRW_Shared->UploadData(&m_SharedData, 1u);
	}

	m_CS->SetShaderDataModule(&m_ParticleDataModule);

	//파티클 위치정보를 계산시킴.
	m_CS->Execute();

	//렌더링하고 나면 PrevPos를 업데이트
	m_BasicData.vOwnerPrevWorldPos = m_BasicData.vOwnerCurWorldPos;
	m_BasicData.vOwnerCurWorldPos = GetOwner()->Transform().GetWorldPos();
}

bool cCom_Renderer_ParticleBasic::render()
{
	//true 반환해서 인스턴싱 필요없다고 전달
	if (nullptr == m_CS)
		return true;

	cMaterial* pMtrl = GetCurMaterial().Get();
	pMtrl->BindData();

	BindMtrlScalarDataToCBuffer();

	m_ParticleDataModule.BindDataGS();

	GetMesh()->renderInstanced(m_BasicData.iMaxParticleCount);

	//드로우콜이 발생했으므로 -> 인스턴싱 필요 없다고 반환
	return true;
}



void cCom_Renderer_ParticleBasic::CreateParticle()
{
	assert(nullptr != m_CS);

	m_bCreated = true;

	m_BasicData.iMaxParticleCount = 200;

	m_BasicData.bModule_Spawn = TRUE;

	m_BasicData.iSpawnRate = 30;

	m_BasicData.vSpawnColor = Vec3(0.4f, 1.f, 0.4f);

	m_BasicData.vSpawnScaleMin = Vec3(50.f, 50.f, 1.f);
	m_BasicData.vSpawnScaleMax = Vec3(100.f, 100.f, 1.f);

	m_BasicData.eSpawnShapeType = 0;
	m_BasicData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);
	m_BasicData.bFollowing = 0; // 시뮬레이션 좌표계

	m_BasicData.fMinLifeTime = 3.f;
	m_BasicData.fMaxLifeTime = 5.f;

	m_BasicData.bModule_ScaleChange = TRUE;
	m_BasicData.fStartScale = 2.f;
	m_BasicData.fEndScale = 0.1f;

	m_BasicData.bModule_ColorChange = TRUE;
	m_BasicData.vStartColor = Vec3(0.2f, 0.3f, 1.0f);
	m_BasicData.vEndColor = Vec3(0.4f, 1.f, 0.4f);

	m_BasicData.bModule_AddVelocity = TRUE;
	m_BasicData.eAddVelocityType = 0; // From Center
	m_BasicData.fSpeed = 500.f;
	m_BasicData.vVelocityDir;
	m_BasicData.fOffsetAngle;

	m_BasicData.bModule_Drag = TRUE;
	m_BasicData.fStartDrag = 200.f;
	m_BasicData.fEndDrag = 0.f;

	m_BasicData.bModule_Rotation = TRUE;
	m_BasicData.vRotRadPerSec = Vec3(0.f, 0.f, 10.f);
	m_BasicData.vRotRandomRange = Vec3(0.f, 0.f, 0.3f);

	m_BasicData.bModule_ApplyGravity = TRUE;
	m_BasicData.bModule_ApplyGravity = TRUE;
	m_BasicData.fGravity = 9.8f;

	//파티클 생성 세팅값 전송
	m_ParticleDataModule.m_SBuffer_ParticleSpawnSetting->Create((UINT)sizeof(tParticleBasicSpawnSetting), 1u, &m_BasicData, 1u);

	//파티클의 위치를 저장할 RW 구조화 버퍼 생성
	m_ParticleDataModule.m_SBufferRW_Transform->Create((UINT)sizeof(tParticleTransform), m_BasicData.iMaxParticleCount, nullptr, 0u);

	//노이즈 텍스처 지정
	m_ParticleDataModule.m_Tex_Noise = cResMgr::GetInst()->Load<cTexture>(strKey_RES_DEFAULT::TEXTURE::NOISE_1);
	assert(nullptr != m_ParticleDataModule.m_Tex_Noise);
	m_SharedData.NoiseTexSize = m_ParticleDataModule.m_Tex_Noise->GetSize();
	

	//공유 데이터 구조화 버퍼 생성
	//GPU에서 사용할 시드값을 전달
	m_SharedData = {};
	m_SharedData.uSeeds.x = RandGen::GetRand<UINT>(0u, UINT_MAX);
	m_SharedData.uSeeds.y = RandGen::GetRand<UINT>(0u, UINT_MAX);
	m_ParticleDataModule.m_SBufferRW_Shared->Create((UINT)sizeof(tParticleShareData), 1, &m_SharedData, 1u);
}
