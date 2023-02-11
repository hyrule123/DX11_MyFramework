#include "pch.h"
#include "CComputeShader.h"

//���̴� �ڵ� ���� �ּҸ� ���� ������
#include "CPathMgr.h"

//���̴� ������
#include "CDevice.h"

//�������
#include "CConstBuffer.h"

CComputeShader::CComputeShader()
	: CShader(eRES_TYPE::COMPUTE_SHADER)
	, m_arrGroup{}
	, m_arrThreadsPerGroup{}
	, m_SharedCBuffer{}
{
}

CComputeShader::CComputeShader(UINT _uThreadsX, UINT _uThreadsY, UINT _uThreadsZ)
	: CShader(eRES_TYPE::COMPUTE_SHADER)
	, m_arrGroup{}
	, m_arrThreadsPerGroup{ _uThreadsX, _uThreadsY, _uThreadsZ }
	, m_SharedCBuffer{}
{
	//�� ������ ���� �и�� ���Ǿ�� �ϹǷ� �����尪�� 0�� ������ ���� �߻�
	assert(0u != _uThreadsX && 0u != _uThreadsY && 0u != _uThreadsZ);
}

CComputeShader::~CComputeShader()
{
}


void CComputeShader::CreateShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize)
{

	m_ShaderData.LoadType = eSHADER_LOADTYPE::eSHADER_INCLUDE;
	m_ShaderData.pByteCode = _pShaderByteCode;
	m_ShaderData.ByteCodeSize = _ShaderByteCodeSize;

	HRESULT result = S_OK;

	if (FAILED(DEVICE->CreateComputeShader(
		_pShaderByteCode, _ShaderByteCodeSize,
		nullptr,
		m_CS.GetAddressOf())))
	{
		MessageBoxA(nullptr, "Compute Shader Compile Failed!", "Error", MB_OK);
		assert(nullptr);
	}
}

void CComputeShader::CreateShader(const wstring& _strFileName, const string& _strFuncName)
{
	// 1. Shader ���� ��� �޾ƿ�
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	//1-1. ���̴� �ε� Ÿ�� ����
	m_ShaderData.LoadType = eSHADER_LOADTYPE::eSHADER_RUNTIME;


	char ShaderNameVersion[32] = {};
	//���� switch ������ _ShaderType�� �߸��Ǿ� ���� ��찡 �ɷ����Ƿ� ALL�� ����
	//2. ���̴� Ÿ�Կ� ���� �ٸ� �Ķ���Ϳ� ������ �Ҵ�
	strcpy_s(ShaderNameVersion, 32u, "cs_5_0");


	// 3. VertexShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), ShaderNameVersion, 0, 0, m_ShaderData.Blob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Compute Shader Compile Failed!!", MB_OK);
		assert(nullptr);
	}


	if (FAILED(DEVICE->CreateComputeShader(
		m_ShaderData.Blob->GetBufferPointer(),
		m_ShaderData.Blob->GetBufferSize()
		, nullptr,
		m_CS.GetAddressOf()
	)))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Compute Shader Compile Failed!!", MB_OK);
	}
}


#define CS_TOTAL_ELEM_X eMTRLDATA_PARAM_SCALAR::INT_0
#define CS_TOTAL_ELEM_Y eMTRLDATA_PARAM_SCALAR::INT_1
#define CS_TOTAL_ELEM_Z eMTRLDATA_PARAM_SCALAR::INT_2

void CComputeShader::CalcGroupNumber(UINT _TotalCountX, UINT _TotalCountY, UINT _TotalCountZ)
{
	m_arrGroup[X] = (UINT)ceilf((float)_TotalCountX / (float)m_arrThreadsPerGroup[X]);
	m_arrGroup[Y] = (UINT)ceilf((float)_TotalCountY / (float)m_arrThreadsPerGroup[Y]);
	m_arrGroup[Z] = (UINT)ceilf((float)_TotalCountZ / (float)m_arrThreadsPerGroup[Z]);
	

	//�����尡 ������ ������ �¾ƶ������� ���� ���� �ִ�.
	//�̷� ���� ����ؼ� ������۷� TotalCount ������ �����Ͽ� ��ǻƮ���̴� �Լ� ���ο��� ����ó���� �� �� �ֵ��� ���ش�.
	SetScalarParam(CS_TOTAL_ELEM_X, &_TotalCountX);
	SetScalarParam(CS_TOTAL_ELEM_Y, &_TotalCountY);
	SetScalarParam(CS_TOTAL_ELEM_Z, &_TotalCountZ);
}

void CComputeShader::SetScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
{
	int param = (int)_Param;
	switch (_Param)
	{
	case eMTRLDATA_PARAM_SCALAR::INT_0:
	case eMTRLDATA_PARAM_SCALAR::INT_1:
	case eMTRLDATA_PARAM_SCALAR::INT_2:
	case eMTRLDATA_PARAM_SCALAR::INT_3:
		m_SharedCBuffer.arrInt[(int)_Param] = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_0:
	case eMTRLDATA_PARAM_SCALAR::FLOAT_1:
	case eMTRLDATA_PARAM_SCALAR::FLOAT_2:
	case eMTRLDATA_PARAM_SCALAR::FLOAT_3:
		m_SharedCBuffer.arrFloat[(int)_Param - (int)eMTRLDATA_PARAM_SCALAR::FLOAT_0] = *((float*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC2_0:
	case eMTRLDATA_PARAM_SCALAR::VEC2_1:
	case eMTRLDATA_PARAM_SCALAR::VEC2_2:
	case eMTRLDATA_PARAM_SCALAR::VEC2_3:
		m_SharedCBuffer.arrV2[(int)_Param - (int)eMTRLDATA_PARAM_SCALAR::VEC2_0] = *((Vec2*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC4_0:
	case eMTRLDATA_PARAM_SCALAR::VEC4_1:
	case eMTRLDATA_PARAM_SCALAR::VEC4_2:
	case eMTRLDATA_PARAM_SCALAR::VEC4_3:
		m_SharedCBuffer.arrV4[(int)_Param - (int)eMTRLDATA_PARAM_SCALAR::VEC4_0] = *((Vec4*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::MAT_0:
	case eMTRLDATA_PARAM_SCALAR::MAT_1:
	case eMTRLDATA_PARAM_SCALAR::MAT_2:
	case eMTRLDATA_PARAM_SCALAR::MAT_3:
		m_SharedCBuffer.arrMat[(int)_Param - (int)eMTRLDATA_PARAM_SCALAR::MAT_0] = *((Matrix*)_Src);
		break;

	}
}

void CComputeShader::Execute()
{
	if (false == BindDataCS())
		return;

	//��ǻƮ���̴� ���� ���� �����͸� ������۸� ���ؼ� ����
	CConstBuffer* pCBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_TYPE::MATERIAL);
	pCBuffer->UploadData(&m_SharedCBuffer, sizeof(tMtrlData));
	pCBuffer->BindBuffer();

	//ó������ ���̴��� �����ϰ� ��� ����.
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_arrGroup[X], m_arrGroup[Y], m_arrGroup[Z]);
	
	//ó���� �Ϸ�Ǿ����� �����ǵ� UnBind�� ���� ������ ���ε��� ����.
	UnBindCS();
}
