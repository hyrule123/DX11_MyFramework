#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructBuffer.h"

CMaterial::CMaterial(bool _bUseInstancing)
	: CRes(eRES_TYPE::MATERIAL)
	, m_SBufferMtrlScalar()
	, m_MtrlTex{}
	, m_arrTex{}
	, m_uCurFrmInstancingCount()
{	
	if (true == _bUseInstancing)
	{
		m_SBufferMtrlScalar = new CStructBuffer(
			eSTRUCT_BUFFER_TYPE::READ_ONLY,
			eSHADER_PIPELINE_STAGE::__ALL,
			eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR
			, e_t_SBUFFER_MTRL_SCALAR,
			e_u_UAV_NONE
		);

		m_SBufferMtrlScalar->Create((UINT)sizeof(tMtrlScalarData), 100u, nullptr, 0u);
	}
}

CMaterial::CMaterial(const CMaterial& _Clone)
	: CRes(_Clone)
	, m_SBufferMtrlScalar(nullptr)	//SBuffer�� �ƿ� ���� �Ƚ�Ŵ
	, m_pShader(_Clone.m_pShader)
	, m_MtrlTex(_Clone.m_MtrlTex)
{
	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		m_arrTex[i] = _Clone.m_arrTex[i];
	}
}



CMaterial::~CMaterial()
{
	DESTRUCTOR_DELETE(m_SBufferMtrlScalar);
}


void CMaterial::BindData()
{
	if (nullptr == m_pShader)
		return;
	
	//�׸��� �ϳ��� ��� return
	m_uCurFrmInstancingCount = m_vecMtrlScalar.size();
	if ((size_t)0 == m_uCurFrmInstancingCount)
		return;
		
	else if((size_t)1 == m_uCurFrmInstancingCount || nullptr == m_SBufferMtrlScalar)
	{
		//1���� �׸��� �������� �׸���(�ν��Ͻ�) ���δ� SBUFFER SHAREDATA ��Ƽ� ����.
		//���⿡ ���� �ν��Ͻ� �ε��� ī��Ʈ���� 0�� ���� ��� CBuffer�� Ȱ���ؼ� �������ϴ� ���
		CConstBuffer* CShareDataBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_SBUFFER_SHAREDATA);

		//SBuffer�� �׸����� �����Ƿ� 0�� ��Ƽ� �����ش�.
		g_arrSBufferShareData[(int)eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR] = {};
		CShareDataBuffer->UploadData(g_arrSBufferShareData);
		CShareDataBuffer->BindBuffer();

		CConstBuffer* CBufferMtrl = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MTRL_SCALAR);
		CBufferMtrl->UploadData(&(m_vecMtrlScalar[0]));
		CBufferMtrl->BindBuffer();
	}
	else	//���Ⱑ �ν��Ͻ� ����
	{
		//���� SBuffer�� ���ε� �ɾ��ָ� �ȴ�.
		m_SBufferMtrlScalar->UploadData(m_vecMtrlScalar.data(), (UINT)m_uCurFrmInstancingCount);
		m_SBufferMtrlScalar->BindBufferSRV();

	}

	//���̴��� �����͸� ���ε����ش�.
	m_pShader->BindData();

	// Texture Update
	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		if (true == IsTexture((eMTRLDATA_PARAM_TEX)i))
			m_arrTex[i]->BindData_SRV(i, eSHADER_PIPELINE_STAGE::__PIXEL);
	}
	//Texture Const Buffer Update
	CConstBuffer* CMtrlTexBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MTRL_TEX);
	CMtrlTexBuffer->UploadData(&m_MtrlTex);
	CMtrlTexBuffer->BindBuffer();
}




void CMaterial::SetScalarParam(UINT32 _iRenderComEntityID, eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
{
	tMtrlScalarData* ScalarDataAddress = nullptr;

	const auto& iter = m_mapEntityID.find(_iRenderComEntityID);

	//��ϵ��� ���� ID �ּ��� ���->���� ���� ������ ��������� ���ٴ� �ǹ�: ���� ������ش�.
	if (iter == m_mapEntityID.end())
	{
		m_vecMtrlScalar.emplace_back();
		size_t idx = m_vecMtrlScalar.size() - (size_t)1;
		m_mapEntityID[_iRenderComEntityID] = idx;

		ScalarDataAddress = &m_vecMtrlScalar[idx];
	}
	else
	{
		ScalarDataAddress = &m_vecMtrlScalar[iter->second];
	}

	switch (_Param)
	{
	case eMTRLDATA_PARAM_SCALAR::INT_0: ScalarDataAddress->INT_0 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_1: ScalarDataAddress->INT_1 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_2: ScalarDataAddress->INT_2 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_3: ScalarDataAddress->INT_3 = *((int*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::FLOAT_0: ScalarDataAddress->FLOAT_0 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_1: ScalarDataAddress->FLOAT_1 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_2: ScalarDataAddress->FLOAT_2 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_3: ScalarDataAddress->FLOAT_3 = *((float*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::VEC2_0: ScalarDataAddress->VEC2_0 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_1: ScalarDataAddress->VEC2_1 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_2: ScalarDataAddress->VEC2_2 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_3: ScalarDataAddress->VEC2_3 = *((Vec2*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC4_0: ScalarDataAddress->VEC4_0 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_1: ScalarDataAddress->VEC4_1 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_2: ScalarDataAddress->VEC4_2 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_3: ScalarDataAddress->VEC4_3 = *((Vec4*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::MAT_0: ScalarDataAddress->MAT_0 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_1: ScalarDataAddress->MAT_1 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_2: ScalarDataAddress->MAT_2 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_3: ScalarDataAddress->MAT_3 = *((Matrix*)_Src);
		break;	

	}
}


void CMaterial::SetTexParam(eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex)
{
	assert(nullptr != _Tex);

	m_arrTex[(int)_Param] = _Tex;

	switch (_Param)
	{
	case eMTRLDATA_PARAM_TEX::_0: m_MtrlTex.bTEX_0 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_1: m_MtrlTex.bTEX_1 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_2: m_MtrlTex.bTEX_2 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_3: m_MtrlTex.bTEX_3 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_4: m_MtrlTex.bTEX_4 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_5: m_MtrlTex.bTEX_5 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_6: m_MtrlTex.bTEX_6 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_7: m_MtrlTex.bTEX_7 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_END: break;
	default: break;
	}
}

