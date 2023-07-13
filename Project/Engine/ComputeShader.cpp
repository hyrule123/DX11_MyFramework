#include "pch.h"
#include "C_ComputeShader.h"

//쉐이더 코드 파일 주소를 통한 컴파일
#include "PathMgr.h"

//쉐이더 컴파일
#include "Device.h"

//상수버퍼
#include "cConstBuffer.h"

#include "jsoncpp.h"

#include "C_CShaderModule.h"

namespace JsonKey_CComputeShader
{
	//쉐이더와 json 파일이 공유하는 파일 이름. 확장자 제외
	//ex) S_C_SCMapLoader.json : S_C_SCMapLoader
	STRKEY_DECLARE(uarrNumThreadXYZ);
}

C_ComputeShader::C_ComputeShader()
	: CShader(eRES_TYPE::COMPUTE_SHADER)
	, m_uNumThreadPerGroupArr{}
	, m_uNumGroupArr{}
	, m_CBuffer_CSShared{}
{

}


C_ComputeShader::~C_ComputeShader()
{
}

//bool C_ComputeShader::Load(const std::filesystem::path& _fileName)
//{
//	std::filesystem::path newPath = PathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::COMPUTE_SHADER);
//	newPath /= _fileName;
//	newPath += RES_INFO::SHADER::Ext_ShaderByteCode;
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

void C_ComputeShader::BindData()
{
	size_t size = m_vecShaderModule.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecShaderModule[i]->BindData();
	}

	//컴퓨트쉐이더 관련 공유 데이터를 상수버퍼를 통해서 전달
	cConstBuffer* pCBuffer = Device::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_MTRL_SCALAR);
	pCBuffer->UploadData(&m_CBuffer_CSShared, sizeof(tMtrlScalarData));
	pCBuffer->BindBuffer();
}

bool C_ComputeShader::SaveJson(Json::Value* _jsonVal)
{
	if (false == CShader::SaveJson(_jsonVal))
		return false;

	Json::Value& jVal = *_jsonVal;

	jVal[JsonKey_CComputeShader::uarrNumThreadXYZ] = Json::Value(Json::arrayValue);

	for (int i = 0; i < ThreadAxis::NumAxis; ++i)
	{
		jVal[JsonKey_CComputeShader::uarrNumThreadXYZ].append(m_uNumThreadPerGroupArr[i]);
	}



	return true;
}

bool C_ComputeShader::LoadJson(Json::Value* _jsonVal)
{
	if (false == CShader::LoadJson(_jsonVal))
		return false;

	if (true == _jsonVal->isMember(JsonKey_CComputeShader::uarrNumThreadXYZ))
	{
		if (NumAxis == (*_jsonVal)[JsonKey_CComputeShader::uarrNumThreadXYZ].size())
		{
			for (int i = 0; i < NumAxis; ++i)
			{
				m_uNumThreadPerGroupArr[i] = (*_jsonVal)[JsonKey_CComputeShader::uarrNumThreadXYZ][i].asInt();
			}
		}

	}
		

	std::filesystem::path ShaderPath = PathMgr::GetInst()->GetPathRel_Content();
	ShaderPath /= DIRECTORY_NAME::SHADER_Bin;
	ShaderPath /= GetKey();
	ShaderPath.replace_extension(RES_INFO::SHADER::Ext_ShaderByteCode);

	std::ios_base::openmode openMode = std::ios::in | std::ios::ate | std::ios::binary;
	std::ifstream CSFile(ShaderPath, openMode);

	if (false == CSFile.is_open())
		return false;

	std::streampos fileSize = CSFile.tellg();
	CSFile.seekg(0, std::ios::beg);

	HRESULT hr = D3DCreateBlob(fileSize, m_pShaderData.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		return false;

	char* pBuffer = static_cast<char*>(m_pShaderData->GetBufferPointer());
	
	CSFile.read(pBuffer, fileSize);
	CSFile.close();

	hr = CreateShaderPrivate(m_pShaderData->GetBufferPointer(), m_pShaderData->GetBufferSize());
	if (FAILED(hr))
		return false;

	return true;
}


HRESULT C_ComputeShader::CreateShaderFromHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize)
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


HRESULT C_ComputeShader::CreateShader(const std::filesystem::path& _FileName, const string_view _strFuncName)
{
	// 1. Shader 파일 경로 받아옴
	std::filesystem::path shaderPath(PathMgr::GetInst()->GetPathRel_Resource(GetResType()));
	shaderPath /= _FileName;

	if (GetKey().empty())
		SetKey(_FileName.string());

	HRESULT result =
		D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, string(_strFuncName).c_str(), SHADER_NAME_VERSION::CS, 0, 0, m_pShaderData.GetAddressOf(), m_ErrBlob.GetAddressOf());

	// 3. VertexShader Compile
	if (FAILED(result))
	{
		assert(SUCCEEDED(result));
		return result;
	}

	return CreateShaderPrivate(m_pShaderData->GetBufferPointer(), m_pShaderData->GetBufferSize());
}

HRESULT C_ComputeShader::CreateShaderPrivate(const void* _pByteCode, size_t _ByteCodeSize)
{
	assert(_pByteCode && _ByteCodeSize);

	return DEVICE->CreateComputeShader(_pByteCode, _ByteCodeSize, nullptr, m_CS.GetAddressOf());
}


#define CS_ELEM_COUNT_X eMTRLDATA_PARAM_SCALAR::INT_0
#define CS_ELEM_COUNT_Y eMTRLDATA_PARAM_SCALAR::INT_1
#define CS_ELEM_COUNT_Z eMTRLDATA_PARAM_SCALAR::INT_2
void C_ComputeShader::CalcGroupNumber(UINT _ElemCountX, UINT _ElemCountY, UINT _ElemCountZ)
{

	m_uNumGroupArr[X] = (UINT)ceilf((float)_ElemCountX / (float)m_uNumThreadPerGroupArr[X]);
	m_uNumGroupArr[Y] = (UINT)ceilf((float)_ElemCountY / (float)m_uNumThreadPerGroupArr[Y]);
	m_uNumGroupArr[Z] = (UINT)ceilf((float)_ElemCountZ / (float)m_uNumThreadPerGroupArr[Z]);
	

	//쓰레드가 쓰레드 갯수와 맞아떨어지지 않을 수도 있다.
	//이럴 를 대비해서 상수버퍼로 TotalCount 변수를 전달하여 컴퓨트쉐이더 함수 내부에서 예외처리를 할 수 있도록 해준다.
	SetMtrlScalarParam(MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_X, &_ElemCountX);
	SetMtrlScalarParam(MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_Y, &_ElemCountY);
	SetMtrlScalarParam(MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_Z, &_ElemCountZ);
}

void C_ComputeShader::SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
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

bool C_ComputeShader::Execute()
{
	if (nullptr == m_CS.Get())
		return false;

	BindData();

	//처리해줄 쉐이더를 지정하고 계산 진행.
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_uNumGroupArr[X], m_uNumGroupArr[Y], m_uNumGroupArr[Z]);
	
	return true;
}
