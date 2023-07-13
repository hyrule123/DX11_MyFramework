#include "pch.h"
#include "cCSModule_SetColor.h"

#include "cTexture.h"

#include "cStructBuffer.h"

#include "strKey_Default.h"

#ifdef _DEBUG
#include "DefaultShader\S_C_SetColor_Debug.h"
#else
#include "DefaultShader\S_C_SetColor_Release.h"
#endif


cCSModule_SetColor::cCSModule_SetColor()
{
}

cCSModule_SetColor::~cCSModule_SetColor()
{
}

bool cCSModule_SetColor::Init()
{
	HRESULT hr = GetOwner()->CreateShaderFromHeader(g_CS_SetColor, sizeof(g_CS_SetColor));

	if (FAILED(hr))
		return false;

	tSBufferDesc Desc = {};
	Desc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
	Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_WRITE;
	Desc.REGISLOT_t_SRV = REGISLOT_t_SBUFFER_SETCOLOR;
	Desc.REGISLOT_u_UAV = REGISLOT_u_SETCOLOR_SBUFFERRW;

	m_StructBufferTest = std::make_unique<cStructBuffer>();
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

bool cCSModule_SetColor::BindData()
{
	m_OutTex->BindData_UAV(REGISLOT_u_SETCOLOR_TEXTURERW);

	m_StructBufferTest->UploadData((void*)m_vecSBuffer, 1280u);
	m_StructBufferTest->BindBufferUAV();

	// 그룹 개수 계산
	GetOwner()->CalcGroupNumber((UINT)m_OutTex->GetWidth(), (UINT)m_OutTex->GetHeight(), 1u);

	return false;
}

void cCSModule_SetColor::UnBind()
{
	m_OutTex->UnBind();
}
