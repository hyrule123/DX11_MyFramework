#include "pch.h"
#include "CComputeShader.h"

//쉐이더 코드 파일 주소를 통한 컴파일
#include "CPathMgr.h"

//쉐이더 컴파일
#include "CDevice.h"

//상수버퍼
#include "CConstBuffer.h"

CComputeShader::CComputeShader()
	: CShader(eRES_TYPE::COMPUTE_SHADER)
	, m_ShaderData{}
	, m_arrGroup{}
	, m_arrThreadsPerGroup{}
	, m_SharedCBuffer{}
{
}

CComputeShader::CComputeShader(UINT _uThreadsX, UINT _uThreadsY, UINT _uThreadsZ)
	: CShader(eRES_TYPE::COMPUTE_SHADER)
	, m_ShaderData{}
	, m_arrGroup{}
	, m_arrThreadsPerGroup{ _uThreadsX, _uThreadsY, _uThreadsZ }
	, m_SharedCBuffer{}
{
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
	// 1. Shader 파일 경로 받아옴
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	//1-1. 쉐이더 로드 타입 변경
	m_ShaderData.LoadType = eSHADER_LOADTYPE::eSHADER_RUNTIME;


	char ShaderNameVersion[32] = {};
	//밑의 switch 문에서 _ShaderType가 잘못되어 있을 경우가 걸러지므로 ALL로 설정
	//2. 쉐이더 타입에 따른 다른 파라미터용 변수를 할당
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


void CComputeShader::Execute()
{
	BindData();

	CConstBuffer* pCBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_MATERIAL);

	pCBuffer->UpdateData(&m_SharedCBuffer, sizeof(tMtrlConst));
	pCBuffer->BindData_CS();

	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_arrGroup[X], m_arrGroup[Y], m_arrGroup[Z]);
	
	UnBind();
}
