#include "pch.h"
#include "CCS_SetColor.h"

#include "CTexture.h"

#include "CStructBuffer.h"

CCS_SetColor::CCS_SetColor(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: CComputeShader(_iGroupPerThreadX, _iGroupPerThreadY, _iGroupPerThreadZ)
{
	UINT Target = eSHADER_PIPELINE_STAGE::__ALL;
	m_StructBufferTest = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, Target, eCBUFFER_SBUFFER_SHAREDATA_IDX::SETCOLOR, e_t_SBUFFER_SETCOLOR, e_u_SETCOLOR_SBUFFERRW);

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
}

CCS_SetColor::~CCS_SetColor()
{
	delete m_StructBufferTest;
}

bool CCS_SetColor::BindDataCS()
{
	m_OutTex->BindData_UAV(e_u_SETCOLOR_TEXTURERW);

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
