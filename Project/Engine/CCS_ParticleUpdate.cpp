#include "pch.h"
#include "CCS_ParticleUpdate.h"

#include "CResMgr.h"
#include "strKeys.h"
#include "CStructBuffer.h"
#include "CConstBuffer.h"
#include "CGameObject.h"
#include "CParticleSystem.h"
#include "CTransform.h"

CCS_ParticleUpdate::CCS_ParticleUpdate(UINT _uNumThreadsX, UINT _uNumThreadsY, UINT _uNumThreadsZ)
	: CComputeShader(_uNumThreadsX, _uNumThreadsY, _uNumThreadsZ)
	, m_pBufferOwner()
	, m_pSBuffer_Transform()
	, m_pSBufferRW_Shared()
	, m_pCBuffer_ModuleData()
{
}

CCS_ParticleUpdate::~CCS_ParticleUpdate()
{
}

bool CCS_ParticleUpdate::BindDataCS()
{
	if (nullptr == m_Tex_Noise)
	{
		m_Tex_Noise = CResMgr::GetInst()->FindRes<CTexture>(RESOURCES::TEXTURE::NOISE_TEXTURE_0);


		assert(nullptr != m_Tex_Noise);
		const Vec2& TexSize = m_Tex_Noise->GetSize();
		SetMtrlScalarParam(MTRL_SCALAR_TEXTURE_NOISE_RESOLUTION, &TexSize);
	}


	//�� ������ �ϳ��� ��ϵǾ����� ������ return false�ؼ� excute ������ �ߴ�
	if (
		nullptr == m_pSBuffer_Transform
		||
		nullptr == m_pSBufferRW_Shared
		||
		nullptr == m_pCBuffer_ModuleData
		)
		return false;

	//��ƼŬ ������Ʈ�� ������ġ�� ��ǻƮ���̴��� ���ۿ� ��Ƽ� �����Ѵ�.
	SetParticleOwnerPos(m_pBufferOwner->Transform()->GetWorldPos());

	//������ �׷� �� ���. ��ƼŬ�� ������ �迭 �����̹Ƿ� x������ �����带 �迭�Ѵ�.
	CalcGroupNumber(m_pSBuffer_Transform->GetCapacity(), 1u, 1u);

	//�����͸� ��ǻƮ���̴��� �ϰ������� ����
	m_pSBuffer_Transform->BindBufferUAV();

	m_pSBufferRW_Shared->BindBufferUAV();

	m_pCBuffer_ModuleData->BindBuffer(eSHADER_PIPELINE_STAGE::__COMPUTE);

	m_Tex_Noise->BindData_SRV(e_t_TEXUTRE_NOISE, eSHADER_PIPELINE_STAGE::__COMPUTE);

	return true;
}

void CCS_ParticleUpdate::UnBindCS()
{
	//��� �� UAV ���ε��� ����.
	m_pSBuffer_Transform->UnBindUAV();
	m_pSBufferRW_Shared->UnBindUAV();

	//���� ������ ���� nullptr�� ����
	m_pBufferOwner = nullptr;
	m_pSBuffer_Transform = nullptr;
	m_pSBufferRW_Shared = nullptr;
	m_pCBuffer_ModuleData = nullptr;
}
