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
	//기본 설정은 단일 드로우콜
	m_CBufferMtrlScalar = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MTRL_SCALAR);
}

CMaterial::CMaterial(const CMaterial& _Clone)
	: CRes(_Clone)
	, m_SBufferMtrlScalar(nullptr)//고유 재질은 SBuffer 아예 생성 안시킴
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
	//상수버퍼는 CDevice에서 관리하고, 구조화버퍼는 여기서 관리하므로 구조화버퍼만 제거한다.
	DESTRUCTOR_DELETE(m_SBufferMtrlScalar);
}

void CMaterial::SetInstancedRender(bool _bEnable)
{
	if(true == _bEnable)
	{
		//상수버퍼주소 제거하고
		m_CBufferMtrlScalar = nullptr;

		//구조화버퍼 생성
		m_SBufferMtrlScalar = new CStructBuffer(tSBufferDesc{
			eSTRUCT_BUFFER_TYPE::READ_ONLY,
			eSHADER_PIPELINE_STAGE::__ALL,
			eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR
			, e_t_SBUFFER_MTRL_SCALAR,
			e_u_UAV_NONE }
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
	
	//그릴게 없을 경우 return

	if (0u == m_vecMtrlScalar.size())
	{
		m_uRenderCount = 0u;
		return;
	}
		
		
	//인스턴싱을 사용하지 않을 경우(구조화버퍼 없을 경우) 상수버퍼에 묶어서 바로 렌더링
	else if(nullptr == m_SBufferMtrlScalar)
	{
		//1개를 그릴지 여러개를 그릴지(인스턴싱) 여부는 SBUFFER SHAREDATA 담아서 보냄.
		//여기에 만약 인스턴싱 인덱스 카운트값이 0이 있을 경우 CBuffer을 활용해서 렌더링하는 방식
		CConstBuffer* CShareDataBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_SBUFFER_SHAREDATA);

		//SBuffer에 그릴것이 없으므로 0을 담아서 보내준다.
		g_arrSBufferShareData[(int)eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR] = {};
		CShareDataBuffer->UploadData(g_arrSBufferShareData);
		CShareDataBuffer->BindBuffer();

		m_CBufferMtrlScalar->UploadData(&(m_vecMtrlScalar[0]));
		m_CBufferMtrlScalar->BindBuffer();

		m_uRenderCount = 1;
	}
	else//구조화버퍼 주소가 있을 경우(인스턴싱을 할 경우) 
	{
		//여긴 SBuffer만 바인딩 걸어주면 된다.
		m_SBufferMtrlScalar->UploadData(m_vecMtrlScalar.data(), m_uRenderCount);
		m_SBufferMtrlScalar->BindBufferSRV();

		m_uRenderCount = (UINT)m_vecMtrlScalar.size();
	}

	//바인딩한 데이터는 전부 제거
	m_vecMtrlScalar.clear();


	//쉐이더도 데이터를 바인딩해준다.
	m_pShader->BindData();

	//텍스처정보도 바인딩 걸어준다.
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

	//nullptr일 경우 해제
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

