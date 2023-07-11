#include "pch.h"
#include "CCS_SetColor.h"

#include "CTexture.h"

#include "CStructBuffer.h"

#include "strKey_Default.h"

#include "DefaultShader\S_C_SetColor_Debug.h"

CCS_SetColor::CCS_SetColor()
	: CComputeShader(32u, 32u, 1u)
{
}

CCS_SetColor::~CCS_SetColor()
{
}

bool CCS_SetColor::Load(std::filesystem::path const& _FilePath)
{
	HRESULT hr = CreateShaderFromHeader(g_CS_SetColor, sizeof(g_CS_SetColor));

	if (FAILED(hr))
		return false;

	tSBufferDesc Desc = {};
	Desc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
	Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_WRITE;
	Desc.REGISLOT_t_SRV = REGISLOT_t_SBUFFER_SETCOLOR;
	Desc.REGISLOT_u_UAV = REGISLOT_u_SETCOLOR_SBUFFERRW;

	m_StructBufferTest = std::make_unique<CStructBuffer>();
	m_StructBufferTest->SetDesc(Desc);

	for (int i = 0; i < 1280u; ++i)
	{
		float c = (float)i / 1280.f;
		m_vecSBuffer[i] = Vec4(c, c, c, 1.f);
	}

	//UAV 바인딩
	m_StructBufferTest->Create((UINT)sizeof(Vec4), 1280u, m_vecSBuffer, 1280u);

	//데이터 다시 받기(테스트)
	m_StructBufferTest->GetData(m_vecSBuffer, (UINT)sizeof(m_vecSBuffer));

	//SRV에 바인딩5
	m_StructBufferTest->BindBufferSRV();

	return true;
}

bool CCS_SetColor::BindDataCS()
{
	m_OutTex->BindData_UAV(REGISLOT_u_SETCOLOR_TEXTURERW);

	m_StructBufferTest->UploadData((void*)m_vecSBuffer, 1280u);
	m_StructBufferTest->BindBufferUAV();

	// 그룹 개수 계산
	CalcGroupNumber((UINT)m_OutTex->GetWidth(), (UINT)m_OutTex->GetHeight(), 1u);

	return true;
}

void CCS_SetColor::UnBindCS()
{
	m_OutTex->UnBind();
}
