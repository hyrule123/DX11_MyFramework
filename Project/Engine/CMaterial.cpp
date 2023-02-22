#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructBuffer.h"

CMaterial::CMaterial()
	: CRes(eRES_TYPE::MATERIAL)
	, m_SBufferMtrlScalar()
	, m_MtrlTex{}
	, m_arrTex{}
	, m_uRenderCount()
{	
	//�⺻ ������ ���� ��ο���
	m_CBufferMtrlScalar = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MTRL_SCALAR);
}

CMaterial::CMaterial(const CMaterial& _Clone)
	: CRes(_Clone)
	, m_SBufferMtrlScalar(nullptr)//���� ������ SBuffer �ƿ� ���� �Ƚ�Ŵ
	, m_pShader(_Clone.m_pShader)
	, m_MtrlTex(_Clone.m_MtrlTex)
	, m_uRenderCount(1u)
	, m_CBufferMtrlScalar()
{
	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		m_arrTex[i] = _Clone.m_arrTex[i];
	}

	m_CBufferMtrlScalar = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MTRL_SCALAR);
}



CMaterial::~CMaterial()
{
	//������۴� CDevice���� �����ϰ�, ����ȭ���۴� ���⼭ �����ϹǷ� ����ȭ���۸� �����Ѵ�.
	DESTRUCTOR_DELETE(m_SBufferMtrlScalar);
}

void CMaterial::SetInstancedRender(bool _bEnable)
{
	if(true == _bEnable)
	{
		//��������ּ� �����ϰ�
		m_CBufferMtrlScalar = nullptr;

		//����ȭ���� ����
		m_SBufferMtrlScalar = new CStructBuffer(
			eSTRUCT_BUFFER_TYPE::READ_ONLY,
			eSHADER_PIPELINE_STAGE::__ALL,
			eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR
			, e_t_SBUFFER_MTRL_SCALAR,
			e_u_UAV_NONE
		);
		m_SBufferMtrlScalar->Create((UINT)sizeof(tMtrlScalarData), 100u, nullptr, 0u);
	}
	else
	{
		SAFE_DELETE(m_SBufferMtrlScalar);

		m_CBufferMtrlScalar = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MTRL_SCALAR);
	}
}


void CMaterial::BindData()
{
	if (nullptr == m_pShader)
		return;
	
	//�׸��� ���� ��� return

	if (0u == m_vecMtrlScalar.size())
		return;
		
	//�ν��Ͻ��� ������� ���� ���(����ȭ���� ���� ���) ������ۿ� ��� �ٷ� ������
	else if(nullptr == m_SBufferMtrlScalar)
	{
		//1���� �׸��� �������� �׸���(�ν��Ͻ�) ���δ� SBUFFER SHAREDATA ��Ƽ� ����.
		//���⿡ ���� �ν��Ͻ� �ε��� ī��Ʈ���� 0�� ���� ��� CBuffer�� Ȱ���ؼ� �������ϴ� ���
		CConstBuffer* CShareDataBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_SBUFFER_SHAREDATA);

		//SBuffer�� �׸����� �����Ƿ� 0�� ��Ƽ� �����ش�.
		g_arrSBufferShareData[(int)eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR] = {};
		CShareDataBuffer->UploadData(g_arrSBufferShareData);
		CShareDataBuffer->BindBuffer();

		m_CBufferMtrlScalar->UploadData(&(m_vecMtrlScalar[0]));
		m_CBufferMtrlScalar->BindBuffer();

		m_uRenderCount = 1;
	}
	else//����ȭ���� �ּҰ� ���� ���(�ν��Ͻ��� �� ���) 
	{
		//���� SBuffer�� ���ε� �ɾ��ָ� �ȴ�.
		m_uRenderCount = (UINT)m_vecMtrlScalar.size();
		m_SBufferMtrlScalar->UploadData(m_vecMtrlScalar.data(), m_uRenderCount);
		m_SBufferMtrlScalar->BindBufferSRV();
	}

	//���ε��� �����ʹ� ���� ����
	m_vecMtrlScalar.clear();


	//���̴��� �����͸� ���ε����ش�.
	m_pShader->BindData();

	//�ؽ�ó������ ���ε� �ɾ��ش�.
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







void CMaterial::SetTexParam(eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex)
{
	m_arrTex[(int)_Param] = _Tex;

	//nullptr�� ��� ����
	BOOL bIsExist = (nullptr != _Tex);

	switch (_Param)
	{
	case eMTRLDATA_PARAM_TEX::_0: m_MtrlTex.bTEX_0 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_1: m_MtrlTex.bTEX_1 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_2: m_MtrlTex.bTEX_2 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_3: m_MtrlTex.bTEX_3 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_4: m_MtrlTex.bTEX_4 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_5: m_MtrlTex.bTEX_5 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_6: m_MtrlTex.bTEX_6 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_7: m_MtrlTex.bTEX_7 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_END: break;
	default: break;
	}
}

