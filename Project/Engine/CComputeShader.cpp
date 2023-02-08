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


#define TotalCountX eSCALAR_PARAM::INT_0
#define TotalCountY eSCALAR_PARAM::INT_1
#define TotalCountZ eSCALAR_PARAM::INT_2

void CComputeShader::CalcGroupNumber(UINT _TotalCountX, UINT _TotalCountY, UINT _TotalCountZ)
{
	m_arrGroup[X] = (UINT)ceilf((float)_TotalCountX / (float)m_arrThreadsPerGroup[X]);
	m_arrGroup[Y] = (UINT)ceilf((float)_TotalCountY / (float)m_arrThreadsPerGroup[Y]);
	m_arrGroup[Z] = (UINT)ceilf((float)_TotalCountZ / (float)m_arrThreadsPerGroup[Z]);
	

	//�����尡 ������ ������ �¾ƶ������� ���� ���� �ִ�.
	//�̷� ���� ����ؼ� ������۷� TotalCount ������ �����Ͽ� ��ǻƮ���̴� �Լ� ���ο��� ����ó���� �� �� �ֵ��� ���ش�.
	SetScalarParam(TotalCountX, &_TotalCountX);
	SetScalarParam(TotalCountY, &_TotalCountY);
	SetScalarParam(TotalCountZ, &_TotalCountZ);
}

void CComputeShader::SetScalarParam(eSCALAR_PARAM _Param, const void* _Src)
{
	switch (_Param)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		m_SharedCBuffer.arrInt[_Param] = *((int*)_Src);
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		m_SharedCBuffer.arrFloat[_Param - FLOAT_0] = *((float*)_Src);
		break;

	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_SharedCBuffer.arrV2[_Param - VEC2_0] = *((Vec2*)_Src);
		break;

	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		m_SharedCBuffer.arrV4[_Param - VEC4_0] = *((Vec4*)_Src);
		break;

	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_SharedCBuffer.arrMat[_Param - MAT_0] = *((Matrix*)_Src);
		break;

	}
}

void CComputeShader::Execute()
{
	if (false == BindDataCS())
		return;

	//��ǻƮ���̴� ���� ���� �����͸� ������۸� ���ؼ� ����
	CConstBuffer* pCBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_TYPE::MATERIAL);
	pCBuffer->UploadData(&m_SharedCBuffer, sizeof(tMtrlConst));
	pCBuffer->BindBuffer();

	//ó������ ���̴��� �����ϰ� ��� ����.
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_arrGroup[X], m_arrGroup[Y], m_arrGroup[Z]);
	
	//ó���� �Ϸ�Ǿ����� �����ǵ� UnBind�� ���� ������ ���ε��� ����.
	UnBindCS();
}
