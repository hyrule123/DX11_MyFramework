#include "pch.h"
#include "CSetColorShader.h"

#include "CTexture.h"

#include "CStructBuffer.h"

CSetColorShader::CSetColorShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: CComputeShader(_iGroupPerThreadX, _iGroupPerThreadY, _iGroupPerThreadZ)
{
	UINT8 Target = eSHADER_PIPELINE_FLAG_ALL;
	m_StructBufferTest = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, Target, eSBUFFER_SHARED_CBUFFER_IDX::TEST, eSRV_REGISTER_IDX::TEST, eUAV_REGISTER_IDX::SETCOLOR_SBUFFER);

	for (int i = 0; i < 1280; ++i)
	{
		float c = (float)i / 1280.f;
		m_vecSBuffer[i] = Vec4(c, c, c, 1.f);
	}

	//UAV 바인딩
	m_StructBufferTest->Create((UINT)sizeof(Vec4), 1280u, m_vecSBuffer, 1280u);

	//데이터 다시 받기
	m_StructBufferTest->GetData(m_vecSBuffer, (UINT)sizeof(m_vecSBuffer));

	//SRV에 바인딩5
	m_StructBufferTest->BindBufferSRV();
}

CSetColorShader::~CSetColorShader()
{
	delete m_StructBufferTest;
}

void CSetColorShader::BindData()
{
	m_OutTex->BindData_CS(0);

	//m_StructBufferTest->UploadData((void*)m_vecSBuffer, (UINT)sizeof(Vec4) * 32u);
	m_StructBufferTest->BindBufferUAV();

	// 그룹 개수 계산
	m_arrGroup[X] = (UINT)(m_OutTex->GetWidth() / m_arrThreadsPerGroup[X]);
	m_arrGroup[Y] = (UINT)(m_OutTex->GetHeight() / m_arrThreadsPerGroup[Y]);
	m_arrGroup[Z] = 1;
}

void CSetColorShader::UnBind()
{
	m_OutTex->UnBind_CS();
}
