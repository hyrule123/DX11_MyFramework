#include "pch.h"
#include "CSetColorShader.h"

#include "CTexture.h"

#include "CStructBuffer.h"

CSetColorShader::CSetColorShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: CComputeShader(_iGroupPerThreadX, _iGroupPerThreadY, _iGroupPerThreadZ)
{
	m_StructBufferTest = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, eSBUFFER_SHARED_CBUFFER_IDX::TEST);

	m_StructBufferTest->Create((UINT)sizeof(Vec4), 32u);

	for (int i = 0; i < 32; ++i)
	{
		float c = 1.f / (float)(i + 1);
		m_vecSBuffer[i] = Vec4(c, c, c, 1.f);
	}
}

CSetColorShader::~CSetColorShader()
{
	delete m_StructBufferTest;
}

void CSetColorShader::BindData()
{
	m_OutTex->BindData_CS(0);

	m_StructBufferTest->UpdateData((void*)m_vecSBuffer, (UINT)sizeof(Vec4) * 32u);
	m_StructBufferTest->BindData_CS(1);

	// 그룹 개수 계산
	m_arrGroup[X] = (UINT)(m_OutTex->GetWidth() / m_arrThreadsPerGroup[X]);
	m_arrGroup[Y] = (UINT)(m_OutTex->GetHeight() / m_arrThreadsPerGroup[Y]);
	m_arrGroup[Z] = 1;
}

void CSetColorShader::UnBind()
{
	m_OutTex->UnBind_CS();
}
