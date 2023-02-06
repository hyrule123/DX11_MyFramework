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
	//이 스레드 수는 분모로 사용되어야 하므로 스레드값이 0이 들어오면 에러 발생
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


#define TotalCountX eSCALAR_PARAM::INT_0
#define TotalCountY eSCALAR_PARAM::INT_1
#define TotalCountZ eSCALAR_PARAM::INT_2

void CComputeShader::CalcGroupNumber(UINT _TotalCountX, UINT _TotalCountY, UINT _TotalCountZ)
{
	m_arrGroup[X] = (UINT)(_TotalCountX / m_arrThreadsPerGroup[X]) + 1u;
	m_arrGroup[Y] = (UINT)(_TotalCountY / m_arrThreadsPerGroup[Y]) + 1u;
	m_arrGroup[Z] = (UINT)(_TotalCountZ / m_arrThreadsPerGroup[Z]) + 1u;
	

	//쓰레드가 쓰레드 갯수와 맞아떨어지지 않을 수도 있다.
	//이럴 떄를 대비해서 상수버퍼로 TotalCount 변수를 전달하여 컴퓨트쉐이더 함수 내부에서 예외처리를 할 수 있도록 해준다.
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

	//컴퓨트쉐이더 관련 공유 데이터를 상수버퍼를 통해서 전달
	CConstBuffer* pCBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_MATERIAL);
	pCBuffer->UploadData(&m_SharedCBuffer, sizeof(tMtrlConst));
	pCBuffer->BindBuffer();

	//처리해줄 쉐이더를 지정하고 계산 진행.
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_arrGroup[X], m_arrGroup[Y], m_arrGroup[Z]);
	
	//처리가 완료되었으면 재정의된 UnBind를 통해 데이터 바인딩을 해제.
	UnBindCS();
}
