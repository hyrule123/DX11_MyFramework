#include "pch.h"
#include "cComputeShader.h"

//쉐이더 코드 파일 주소를 통한 컴파일
#include "cPathMgr.h"

//쉐이더 컴파일
#include "cDevice.h"

//상수버퍼
#include "cConstBuffer.h"

#include "jsoncpp.h"

#include "cGPUBufferModule.h"



cComputeShader::cComputeShader()
	: IShader(eRES_TYPE::COMPUTE_SHADER)
	, m_pShaderData()
	, m_CS()
	, m_arrNumThreads()
	, m_ComputeShaderInfo{}
{
}


cComputeShader::~cComputeShader()
{
}



bool cComputeShader::BindData()
{
	if (false == IsDataReady())
		return false;

	//컴퓨트쉐이더 관련 공유 데이터를 상수버퍼를 통해서 전달
	static cConstBuffer* pCBuffer = cDevice::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_COMPUTE_SHADER_INFO);
	pCBuffer->UploadData(&m_ComputeShaderInfo, sizeof(tComputeShaderInfo));
	pCBuffer->BindBuffer();

	return true;
}

bool cComputeShader::SaveJson(Json::Value* _jsonVal)
{
	if (false == IShader::SaveJson(_jsonVal))
		return false;

	Json::Value& jVal = *_jsonVal;

	jVal[JsonKey_cComputeShader::m_arrNumThreads] = Json::Value(Json::arrayValue);

	for (int i = 0; i < ThreadAxis::NumAxis; ++i)
	{
		jVal[JsonKey_cComputeShader::m_arrNumThreads].append(m_arrNumThreads[i]);
	}

	return true;
}

bool cComputeShader::LoadJson(Json::Value* _jsonVal)
{
	if (false == IShader::LoadJson(_jsonVal))
		return false;

	//이건 못 불러왔을 경우 문제가 생김
	if (true == _jsonVal->isMember(JsonKey_cComputeShader::m_arrNumThreads))
	{
		if (NumAxis == (*_jsonVal)[JsonKey_cComputeShader::m_arrNumThreads].size())
		{
			for (int i = 0; i < NumAxis; ++i)
			{
				m_arrNumThreads[i] = (*_jsonVal)[JsonKey_cComputeShader::m_arrNumThreads][i].asInt();
			}
		}
	}
	else
	{
		ERROR_MESSAGE("Failed to Load NumThread(ComputeShader)!");
		return false;
	}
	
	std::filesystem::path ShaderPath = cPathMgr::GetInst()->GetPathRel_Content();
	ShaderPath /= DIRECTORY_NAME::SHADER_BIN;
	ShaderPath /= GetKey();
	ShaderPath.replace_extension(RES_INFO::SHADER::Ext_ShaderByteCode);

	std::ios_base::openmode openMode = std::ios::in | std::ios::ate | std::ios::binary;
	std::ifstream CSFile(ShaderPath, openMode);

	if (false == CSFile.is_open())
		return false;

	std::streampos fileSize = CSFile.tellg();
	CSFile.seekg(0, std::ios::beg);

	ComPtr<ID3DBlob> blob;
	HRESULT hr = D3DCreateBlob(fileSize, blob.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		return false;

	char* pBuffer = static_cast<char*>(blob->GetBufferPointer());
	
	CSFile.read(pBuffer, fileSize);
	CSFile.close();

	hr = CreateShader(blob);

	if (FAILED(hr))
		return false;

	return true;
}


HRESULT cComputeShader::CreateShaderFromHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize)
{
	if (nullptr == _pByteCode)
		return E_POINTER;

	HRESULT hr = D3DCreateBlob(_ByteCodeSize, m_pShaderData.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	memcpy_s(m_pShaderData->GetBufferPointer(), m_pShaderData->GetBufferSize(), _pByteCode, _ByteCodeSize);

	return CreateShaderPrivate(m_pShaderData->GetBufferPointer(), m_pShaderData->GetBufferSize());
}


HRESULT cComputeShader::CreateShader(const std::filesystem::path& _FileName, const string_view _strFuncName)
{
	// 1. Shader 파일 경로 받아옴
	std::filesystem::path shaderPath(cPathMgr::GetInst()->GetPathRel_Resource(GetResType()));
	shaderPath /= _FileName;

	if (GetKey().empty())
		SetKey(_FileName.string());

	HRESULT result =
		D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, string(_strFuncName).c_str(), SHADER_NAME_VERSION::CS, 0, 0, m_pShaderData.GetAddressOf(), m_ErrBlob.GetAddressOf());

	
	if (FAILED(result))
	{
		assert(SUCCEEDED(result));
		return result;
	}

	return CreateShaderPrivate(m_pShaderData->GetBufferPointer(), m_pShaderData->GetBufferSize());
}

HRESULT cComputeShader::CreateShaderPrivate(const void* _pByteCode, size_t _ByteCodeSize)
{
	assert(_pByteCode && _ByteCodeSize);

	return DEVICE->CreateComputeShader(_pByteCode, _ByteCodeSize, nullptr, m_CS.GetAddressOf());
}


void cComputeShader::CalcGroupNumber(const tNumDataCS& _NumData)
{
	//셋 중 하나라도 0일경우 Assert
	assert(_NumData.X && _NumData.Y && _NumData.Z);

	m_ComputeShaderInfo.NumData = _NumData;

	m_ComputeShaderInfo.uNumGroupX = (UINT)ceilf((float)m_ComputeShaderInfo.NumData.X / (float)m_arrNumThreads[X]);
	m_ComputeShaderInfo.uNumGroupY = (UINT)ceilf((float)m_ComputeShaderInfo.NumData.Y / (float)m_arrNumThreads[Y]);
	m_ComputeShaderInfo.uNumGroupZ = (UINT)ceilf((float)m_ComputeShaderInfo.NumData.Z / (float)m_arrNumThreads[Z]);
}


bool cComputeShader::Execute()
{
	if (nullptr == m_CS.Get())
		return false;

	if (false == BindData())
		return false;

	//처리해줄 쉐이더를 지정하고 계산 진행.
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_ComputeShaderInfo.uNumGroupX, m_ComputeShaderInfo.uNumGroupY, m_ComputeShaderInfo.uNumGroupZ);

	//쉐이더 그룹 데이터를 초기화(에러 방지)
	memset(&m_ComputeShaderInfo, 0, sizeof(tComputeShaderInfo));

	return true;
}

inline bool cComputeShader::IsDataReady()
{
	bool bReady = false;
	if (m_ComputeShaderInfo.NumData.X && m_ComputeShaderInfo.NumData.Y && m_ComputeShaderInfo.NumData.Z)
		bReady = true;

	return bReady;
}


//bool cComputeShader::Load(const std::filesystem::path& _fileName)
//{
//	std::filesystem::path newPath = cPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::COMPUTE_SHADER);
//	newPath /= _fileName;
//	newPath += RES_INFO::SHADER::COMPUTE;
//
//	auto flag = std::ios::binary | std::ios::end;
//	std::ifstream ifs(newPath, flag);
//	if (false == ifs.is_open())
//	{
//		ERROR_MESSAGE("Failed to open Shader bytecode File.");
//		return false;
//	}
//
//	const size_t fileSize = ifs.tellg();
//	
//	ComPtr<ID3DBlob> blob;
//	HRESULT hr = D3DCreateBlob(fileSize, blob.GetAddressOf());
//	if (FAILED(hr) || fileSize != blob->GetBufferSize())
//	{
//		ERROR_MESSAGE("Failed to Create Shader blob.");
//	}
//	
//	char* pCodeBegin = reinterpret_cast<char*>(blob->GetBufferPointer());
//	
//
//	ifs.seekg(std::ios::beg);
//
//	ifs.read(pCodeBegin, fileSize);
//
//	ifs.close();
//
//	CreateShader(blob);
//}
//공유 데이터를 전달하기위한 상수버퍼용 구조체
