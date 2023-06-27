#include "pch.h"
#include "CComputeShader.h"

//쉐이더 코드 파일 주소를 통한 컴파일
#include "CPathMgr.h"

//쉐이더 컴파일
#include "CDevice.h"

//상수버퍼
#include "CConstBuffer.h"

#include "jsoncpp.h"

CComputeShader::CComputeShader()
	: CShader(eRES_TYPE::COMPUTE_SHADER)
	, m_uNumGroupArr{}
	, m_uNumThreadPerGroupArr{1u, 1u, 1u}
	, m_CBuffer_CSShared{}
{
}

CComputeShader::CComputeShader(UINT _uThreadX, UINT _uThreadY, UINT _uThreadZ)
	: CShader(eRES_TYPE::COMPUTE_SHADER)
	, m_uNumThreadPerGroupArr{_uThreadX, _uThreadY, _uThreadZ}
	, m_uNumGroupArr{}
	, m_CBuffer_CSShared{}
{
	//이 스레드 수는 분모로 사용되어야 하므로 스레드값이 0이 들어오면 에러 발생
	assert(0u != _uThreadX && 0u != _uThreadY && 0u != _uThreadZ);
}


CComputeShader::~CComputeShader()
{
}

//bool CComputeShader::Save(const std::filesystem::path& _fileName)
//{
//	std::filesystem::path _path(RELATIVE_PATH::SHADER_COMPUTE::A);
//	_path /= _fileName;
//	std::ofstream fp(_path);
//
//	if (fp.is_open())
//	{
//		Json::Value jVal;
//
//		if (true == SaveJson(&jVal))
//		{
//			fp << jVal;
//		}
//
//		fp.close();
//	}
//
//	return false;
//}

//bool CComputeShader::Load(const std::filesystem::path& _fileName)
//{
//	std::filesystem::path CSPath(RELATIVE_PATH::SHADER_COMPUTE::A);
//	CSPath /= _fileName;
//
//	std::ifstream fp(CSPath);
//
//	if (fp.is_open())
//	{
//		Json::Value jVal;
//
//		fp >> jVal;
//		fp.close();
//
//		return LoadJson(&jVal);
//	}
//	return false;
//}

bool CComputeShader::SaveJson(Json::Value* _jsonVal)
{
	if (false == CShader::SaveJson(_jsonVal))
		return false;

	Json::Value& jVal = *_jsonVal;

	jVal[string(RES_INFO::SHADER::COMPUTE::JSON_KEY::uarrNumThreadXYZ)] = Json::Value(Json::arrayValue);

	for (int i = 0; i < ThreadAxis::NumAxis; ++i)
	{
		jVal[string(RES_INFO::SHADER::COMPUTE::JSON_KEY::uarrNumThreadXYZ)].append(m_uNumThreadPerGroupArr[i]);
	}

	return true;
}

bool CComputeShader::LoadJson(Json::Value* _jsonVal)
{
	if (false == CShader::LoadJson(_jsonVal))
		return false;

	if (true == _jsonVal->isMember(string(RES_INFO::SHADER::COMPUTE::JSON_KEY::uarrNumThreadXYZ)))
	{
		if (NumAxis == (*_jsonVal)[string(RES_INFO::SHADER::COMPUTE::JSON_KEY::uarrNumThreadXYZ)].size())
		{
			for (int i = 0; i < NumAxis; ++i)
			{
				m_uNumThreadPerGroupArr[i] = (*_jsonVal)[RES_INFO::SHADER::COMPUTE::JSON_KEY::uarrNumThreadXYZ][i].asInt();
			}
		}

	}
		

	std::filesystem::path ShaderPath = CPathMgr::GetInst()->GetPathRel_Content();
	ShaderPath /= DIRECTORY_NAME::SHADER_CSO;
	ShaderPath /= GetKey();
	ShaderPath.replace_extension(RES_INFO::SHADER::Ext_ShaderCode);

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


HRESULT CComputeShader::CreateShaderFromHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize)
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


HRESULT CComputeShader::CreateShader(const std::filesystem::path& _FileName, const string_view _strFuncName)
{
	// 1. Shader 파일 경로 받아옴
	std::filesystem::path shaderPath(CPathMgr::GetInst()->GetPathRel_Resource(GetResType()));
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

HRESULT CComputeShader::CreateShaderPrivate(const void* _pByteCode, size_t _ByteCodeSize)
{
	assert(_pByteCode && _ByteCodeSize);

	return DEVICE->CreateComputeShader(_pByteCode, _ByteCodeSize, nullptr, m_CS.GetAddressOf());
}


#define CS_ELEM_COUNT_X eMTRLDATA_PARAM_SCALAR::INT_0
#define CS_ELEM_COUNT_Y eMTRLDATA_PARAM_SCALAR::INT_1
#define CS_ELEM_COUNT_Z eMTRLDATA_PARAM_SCALAR::INT_2
void CComputeShader::CalcGroupNumber(UINT _ElemCountX, UINT _ElemCountY, UINT _ElemCountZ)
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

void CComputeShader::SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
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

bool CComputeShader::Execute()
{
	if (false == BindDataCS())
		return false;

	//컴퓨트쉐이더 관련 공유 데이터를 상수버퍼를 통해서 전달
	CConstBuffer* pCBuffer = CDevice::GetInst()->GetConstBuffer(idx_b_CBUFFER_MTRL_SCALAR);
	pCBuffer->UploadData(&m_CBuffer_CSShared, sizeof(tMtrlScalarData));
	pCBuffer->BindBuffer();

	//처리해줄 쉐이더를 지정하고 계산 진행.
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_uNumGroupArr[X], m_uNumGroupArr[Y], m_uNumGroupArr[Z]);
	
	//처리가 완료되었으면 재정의된 UnBind를 통해 데이터 바인딩을 해제.
	UnBindCS();

	return true;
}
