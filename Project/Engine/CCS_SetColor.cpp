#include "pch.h"
#include "CCS_SetColor.h"

#include "CTexture.h"

#include "CStructBuffer.h"

CCS_SetColor::CCS_SetColor(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: CComputeShader(_iGroupPerThreadX, _iGroupPerThreadY, _iGroupPerThreadZ)
{
	UINT8 Target = eSHADER_PIPELINE_STAGE_FLAG::__ALL;
	m_StructBufferTest = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, Target, eSBUFFER_SHARED_CBUFFER_IDX::SETCOLOR, eSRV_REGISTER_IDX::SETCOLOR, eUAV_REGISTER_IDX::SETCOLOR_SBUFFER);

	for (int i = 0; i < 1280u; ++i)
	{
		float c = (float)i / 1280.f;
		m_vecSBuffer[i] = Vec4(c, c, c, 1.f);
	}

	//UAV ���ε�
	m_StructBufferTest->Create((UINT)sizeof(Vec4), 1280u, m_vecSBuffer, 1280u);

	//������ �ٽ� �ޱ�(�׽�Ʈ)
	m_StructBufferTest->GetData(m_vecSBuffer, (UINT)sizeof(m_vecSBuffer));

	//SRV�� ���ε�5
	m_StructBufferTest->BindBufferSRV();
}

CCS_SetColor::~CCS_SetColor()
{
	delete m_StructBufferTest;
}

bool CCS_SetColor::BindDataCS()
{
	m_OutTex->BindData_CS(0);

	m_StructBufferTest->UploadData((void*)m_vecSBuffer, 1280u);
	m_StructBufferTest->BindBufferUAV();

	// �׷� ���� ���
	CalcGroupNumber((UINT)m_OutTex->GetWidth(), (UINT)m_OutTex->GetHeight(), 1u);

	return true;
}

void CCS_SetColor::UnBindCS()
{
	m_OutTex->UnBind_CS();
}
