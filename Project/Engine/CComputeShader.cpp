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
	, m_uNumGroupX()
	, m_uNumGroupY()
	, m_uNumGroupZ()
	, m_uNumThreadsPerGroupX()
	, m_uNumThreadsPerGroupY()
	, m_uNumThreadsPerGroupZ()
	, m_CBuffer_CSShared{}
{
}

CComputeShader::CComputeShader(UINT _uThreadsX, UINT _uThreadsY, UINT _uThreadsZ)
	: CShader(eRES_TYPE::COMPUTE_SHADER)
	, m_uNumThreadsPerGroupX(_uThreadsX)
	, m_uNumThreadsPerGroupY(_uThreadsY)
	, m_uNumThreadsPerGroupZ(_uThreadsZ)
	, m_uNumGroupX()
	, m_uNumGroupY()
	, m_uNumGroupZ()
	, m_CBuffer_CSShared{}
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


#define CS_ELEM_COUNT_X eMTRLDATA_PARAM_SCALAR::INT_0
#define CS_ELEM_COUNT_Y eMTRLDATA_PARAM_SCALAR::INT_1
#define CS_ELEM_COUNT_Z eMTRLDATA_PARAM_SCALAR::INT_2

void CComputeShader::CalcGroupNumber(UINT _ElemCountX, UINT _ElemCountY, UINT _ElemCountZ)
{
	m_uNumGroupX = (UINT)ceilf((float)_ElemCountX / (float)m_uNumThreadsPerGroupX);
	m_uNumGroupY = (UINT)ceilf((float)_ElemCountY / (float)m_uNumThreadsPerGroupY);
	m_uNumGroupZ = (UINT)ceilf((float)_ElemCountZ / (float)m_uNumThreadsPerGroupZ);
	

	//�����尡 ������ ������ �¾ƶ������� ���� ���� �ִ�.
	//�̷� ���� ����ؼ� ������۷� TotalCount ������ �����Ͽ� ��ǻƮ���̴� �Լ� ���ο��� ����ó���� �� �� �ֵ��� ���ش�.
	SetScalarParam(CS_ELEM_COUNT_X, &_ElemCountX);
	SetScalarParam(CS_ELEM_COUNT_Y, &_ElemCountY);
	SetScalarParam(CS_ELEM_COUNT_Z, &_ElemCountZ);
}

void CComputeShader::SetScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
{
	switch (_Param)
	{
	case eMTRLDATA_PARAM_SCALAR::INT_0: m_CBuffer_CSShared.INT_0 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_1: m_CBuffer_CSShared.INT_1 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_2: m_CBuffer_CSShared.INT_2 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_3: m_CBuffer_CSShared.INT_3 = *((int*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::FLOAT_0: m_CBuffer_CSShared.FLOAT_0 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_1: m_CBuffer_CSShared.FLOAT_1 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_2: m_CBuffer_CSShared.FLOAT_2 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_3: m_CBuffer_CSShared.FLOAT_3 = *((float*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::VEC2_0: m_CBuffer_CSShared.VEC2_0 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_1: m_CBuffer_CSShared.VEC2_1 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_2: m_CBuffer_CSShared.VEC2_2 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_3: m_CBuffer_CSShared.VEC2_3 = *((Vec2*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC4_0: m_CBuffer_CSShared.VEC4_0 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_1: m_CBuffer_CSShared.VEC4_1 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_2: m_CBuffer_CSShared.VEC4_2 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_3: m_CBuffer_CSShared.VEC4_3 = *((Vec4*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::MAT_0: m_CBuffer_CSShared.MAT_0 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_1: m_CBuffer_CSShared.MAT_1 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_2: m_CBuffer_CSShared.MAT_2 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_3: m_CBuffer_CSShared.MAT_3 = *((Matrix*)_Src);
		break;

	}
}

void CComputeShader::Execute()
{
	if (false == BindDataCS())
		return;

	//��ǻƮ���̴� ���� ���� �����͸� ������۸� ���ؼ� ����
	CConstBuffer* pCBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MATERIAL);
	pCBuffer->UploadData(&m_CBuffer_CSShared, sizeof(tMtrlData));
	pCBuffer->BindBuffer();

	//ó������ ���̴��� �����ϰ� ��� ����.
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_uNumGroupX, m_uNumGroupY, m_uNumGroupZ);
	
	//ó���� �Ϸ�Ǿ����� �����ǵ� UnBind�� ���� ������ ���ε��� ����.
	UnBindCS();
}
