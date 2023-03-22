#include "pch.h"
#include "CGraphicsShader.h"

#include "CPathMgr.h"
#include "CDevice.h"

#include "jsoncpp.h"

namespace SHADER_NAME_VERSION
{
	constexpr const char* VS = "vs_5_0";
	constexpr const char* HS = "hs_5_0";
	constexpr const char* DS = "ds_5_0";
	constexpr const char* GS = "gs_5_0";
	constexpr const char* PS = "ps_5_0";
}

namespace SHADER_EXTENSION
{
	constexpr const wchar_t* CSO = L".CSO";
	constexpr const wchar_t* FX = L".FX";
}


CGraphicsShader::CGraphicsShader()
	: CShader(eRES_TYPE::GRAPHICS_SHADER)
	, m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_eRSType(eRASTERIZER_TYPE::CULL_BACK)
	, m_eDSType(eDEPTH_STENCIL_TYPE::LESS)
	, m_eBSType(eBLEND_STATE_TYPE::DEFAULT)
	, m_eShaderDomain(eSHADER_DOMAIN::_UNDEFINED)
	, m_ShaderData{}
{
}

CGraphicsShader::~CGraphicsShader()
{
}

//bool CGraphicsShader::Save(const std::filesystem::path& _fileName)
//{
//	std::ifstream FileOpen(_fileName);
//	if (FileOpen.is_open())
//	{
//		Json::Value jsonVal;
//		FileOpen >> jsonVal;
//		FileOpen.close();
//
//		return SaveJson(&jsonVal);
//	}
//
//	return false;
//}

bool CGraphicsShader::SaveJson(Json::Value* _jsonVal)
{
	if (false == CShader::SaveJson(_jsonVal))
		return false;

	Json::Value& jVal = *_jsonVal;

	string comment = "//Enumeration Values are at define.h of Engine project";
	jVal.setComment(comment, Json::CommentPlacement::commentAfter);

	//순회를 돌면서 비트마스크를 만들어준뒤 json 파일에 저장한다.
	int flagPipeline = 0;
	for (int i = 0; i < (int)eSHADER_TYPE::END; ++i)
	{
		bool bExist = false;

		switch ((eSHADER_TYPE)i)
		{
		case eSHADER_TYPE::__VERTEX:
			if (nullptr != m_VS.Get())
				bExist = true;
			break;
		case eSHADER_TYPE::__HULL:
			if(nullptr != m_HS.Get())
				bExist = true;
			break;
		case eSHADER_TYPE::__DOMAIN:
			if (nullptr != m_DS.Get())
				bExist = true;
			break;
		case eSHADER_TYPE::__GEOMETRY:
			if (nullptr != m_GS.Get())
				bExist = true;
			break;
		case eSHADER_TYPE::__PIXEL:
			if (nullptr != m_PS.Get())
				bExist = true;
			break;

		default:
			break;
		}

		if (true == bExist)
			flagPipeline |= 1 << i;
	}

	

	jVal[string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eSHADER_PIPELINE_STAGE)] = flagPipeline;

	jVal[string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::D3D_PRIMITIVE_TOPOLOGY)] = (int)m_eTopology;

	jVal[string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eRASTERIZER_TYPE)] = (int)m_eRSType;
	jVal[string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eDEPTH_STENCIL_TYPE)] = (int)m_eDSType;
	jVal[string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eBLEND_STATE_TYPE)] = (int)m_eBSType;
	jVal[string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eSHADER_DOMAIN)] = (int)m_eShaderDomain;

	return true;
}

bool CGraphicsShader::LoadJson(Json::Value* _jsonVal)
{
	if (false == CShader::LoadJson(_jsonVal))
		return false;

	const Json::Value& jVal = *_jsonVal;


	//쉐이더의 베이스 명칭은 CEntity::Name에서 로드되었을 것임.
	string strKey = string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eBLEND_STATE_TYPE);
	if(jVal.isMember(strKey))
		m_eBSType = (eBLEND_STATE_TYPE)jVal[strKey].asInt();

	strKey = string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eDEPTH_STENCIL_TYPE);
	if (jVal.isMember(strKey))
		m_eDSType = (eDEPTH_STENCIL_TYPE)jVal[strKey].asInt();

	strKey = string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eRASTERIZER_TYPE);
	if (jVal.isMember(strKey))
		m_eRSType = (eRASTERIZER_TYPE)jVal[strKey].asInt();

	strKey = string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::D3D_PRIMITIVE_TOPOLOGY);
	if(jVal.isMember(strKey))
		m_eTopology = (D3D11_PRIMITIVE_TOPOLOGY)jVal[strKey].asInt();
		
	strKey = string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eSHADER_DOMAIN);
	if(jVal.isMember(strKey))
		m_eShaderDomain = (eSHADER_DOMAIN)jVal[strKey].asInt();

	if (eSHADER_DOMAIN::_UNDEFINED == m_eShaderDomain)
	{
		string errorMessage = "The shader domain value of ";
		errorMessage += GetName();
		errorMessage += " is Not set. May cause error.";
		MessageBoxA(nullptr, errorMessage.c_str(), NULL, MB_OK);
		assert(nullptr);
		return false;
	}

	int flagPipeline = jVal[string(RES_INFO::SHADER::GRAPHICS::JSON_KEY::eSHADER_PIPELINE_STAGE)].asInt();
	int ShaderOrder = 0;

	std::ios_base::openmode openFlag = std::ios_base::ate | std::ios_base::binary; std::ios_base::in;
	for (int i = 0; i < (int)eSHADER_TYPE::END; ++i)
	{
		//특정 쉐이더 사용 설정이 되어있을 경우 쉐이더 로드 시행
		if ((1 << i) & flagPipeline)
		{
			++ShaderOrder;

			std::filesystem::path shaderPath = GETRESPATH;

			shaderPath /= "S_";
			shaderPath += std::to_string(ShaderOrder);
			shaderPath += RES_INFO::SHADER::GRAPHICS::arrPrefix[i];
			shaderPath += GetName();

			std::ifstream shaderCode(shaderPath, openFlag);

			if (true == shaderCode.is_open())
			{
				//cso를 읽어올 공간 동적할당
				std::streampos codeSize = shaderCode.tellg();
				m_ShaderData[i].LoadType = eSHADER_LOADTYPE::BYTE_CODE_FROM_FILE;
				m_ShaderData[i].ByteCodeSize = codeSize;
				m_ShaderData[i].pByteCode = new char[codeSize];
				memset(m_ShaderData[i].pByteCode, 0, codeSize);

				//파일로부터 데이터를 읽어오고, 파일은 닫아준다.
				shaderCode.seekg(0, std::ios_base::beg);
				shaderCode.read(m_ShaderData[i].pByteCode, codeSize);
				shaderCode.close();

				//읽어온 바이트 코드로부터 쉐이더를 로딩해준다.
				CreateShader(m_ShaderData[i].pByteCode, codeSize, (eSHADER_TYPE)i, eSHADER_LOADTYPE::BYTE_CODE_FROM_FILE);
			}
			else
			{
				string strErrMsg = "Shader File \n\"";
				strErrMsg += shaderPath.filename().string() + "\"\n";
				strErrMsg += "Load Failed!!";
				MessageBoxA(nullptr, strErrMsg.c_str(), NULL, MB_OK);
				assert(nullptr);
				return false;
			}
		}
	}

	return true;
}

//bool CGraphicsShader::Load(const std::filesystem::path& _fileName)
//{
//	std::filesystem::path shaderPath = RELATIVE_PATH::SHADER_GRAPHICS::A;
//	shaderPath /= _fileName;
//
//	std::ifstream fpShader(shaderPath);
//	if (fpShader.is_open())
//	{
//		Json::Value shaderInfo;
//		fpShader >> shaderInfo;
//
//		return LoadJson(&shaderInfo);
//	}
//	
//	return false;
//}

void CGraphicsShader::CreateDefaultInputLayout()
{
	// InputLayout 생성
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[2] = {};

	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;
	LayoutDesc[0].AlignedByteOffset = 0;
	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	LayoutDesc[0].InputSlot = 0;
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;
	

	//LayoutDesc[1].SemanticName = "COLOR";
	//LayoutDesc[1].SemanticIndex = 0;

	////이전 시멘틱에서 얼마나 떨어졌는지 여부를 저장. 0번 인덱스는 R32B32G32(4+4+4 = 12) 이므로 오프셋을 12로 잡아준다.
	//LayoutDesc[1].AlignedByteOffset = 12;					
	//LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//LayoutDesc[1].InputSlot = 0;
	//LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//LayoutDesc[1].InstanceDataStepRate = 0;

	LayoutDesc[1].SemanticName = "TEXCOORD";
	LayoutDesc[1].SemanticIndex = 0;

	//0번 인덱스1: 12 , 1번 인덱스 : R32B32G32A32(4 * 4 = 16) -> 12 + 16 = 28
	LayoutDesc[1].AlignedByteOffset = 12;
	LayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	LayoutDesc[1].InputSlot = 0;
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[1].InstanceDataStepRate = 0;
	

	//blob 형태로 로드했을 경우와 헤더 형태로 로드했을 경우에 따라 다른 파일을 참조해서 로드한다.
	switch (m_ShaderData[(int)eSHADER_TYPE::__VERTEX].LoadType)
	{
	case eSHADER_LOADTYPE::NOT_LOADED:
		//Vertrx Shader가 로드되어있지 않을 경우 assert
		assert((bool)m_ShaderData[(int)eSHADER_TYPE::__VERTEX].LoadType);
		break;	

	case eSHADER_LOADTYPE::RUNTIME_COMPILED:
		if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 2,
			m_ShaderData[(int)eSHADER_TYPE::__VERTEX].Blob->GetBufferPointer(),
			m_ShaderData[(int)eSHADER_TYPE::__VERTEX].Blob->GetBufferSize(),
			m_Layout.GetAddressOf())))
		{
			assert(nullptr);
		}


		break;	

	//두 경우에는 같은 방식으로 로드함.
	case eSHADER_LOADTYPE::BYTE_CODE_INCLUDED:
	case eSHADER_LOADTYPE::BYTE_CODE_FROM_FILE:
		if (FAILED(DEVICE->CreateInputLayout(
			LayoutDesc, 2,
			m_ShaderData[(int)eSHADER_TYPE::__VERTEX].pByteCode,
			m_ShaderData[(int)eSHADER_TYPE::__VERTEX].ByteCodeSize,
			m_Layout.GetAddressOf())))
		{
			assert(nullptr);
		}

		break;


	default:
		break;
	}
}


void CGraphicsShader::CreateShader(char* _pShaderByteCode, size_t _ShaderByteCodeSize, eSHADER_TYPE _ShaderType, eSHADER_LOADTYPE _LoadType)
{
	assert(eSHADER_LOADTYPE::RUNTIME_COMPILED != _LoadType);
	m_ShaderData[(int)_ShaderType].LoadType = _LoadType;
	m_ShaderData[(int)_ShaderType].pByteCode = _pShaderByteCode;
	m_ShaderData[(int)_ShaderType].ByteCodeSize = _ShaderByteCodeSize;

	HRESULT result = S_OK;

	switch (_ShaderType)
	{
	case eSHADER_TYPE::__VERTEX:
		result = DEVICE->CreateVertexShader(
			_pShaderByteCode, _ShaderByteCodeSize,
			nullptr,
			m_VS.GetAddressOf()
		);

		CreateDefaultInputLayout();
		break;

	case eSHADER_TYPE::__HULL:
		break;

	case eSHADER_TYPE::__DOMAIN:


		break;

	case eSHADER_TYPE::__GEOMETRY:
		result = DEVICE->CreateGeometryShader(
			_pShaderByteCode, _ShaderByteCodeSize,
			nullptr,
			m_GS.GetAddressOf()
		);

		break;

	case eSHADER_TYPE::__PIXEL:
		result = DEVICE->CreatePixelShader(_pShaderByteCode, _ShaderByteCodeSize, nullptr, m_PS.GetAddressOf());
		break;
	default:
		break;
	}

	if (true == FAILED(result))
	{
		MessageBoxA(nullptr, "Shader Loading Failed!!", NULL, MB_OK);
		assert(SUCCEEDED(result));
	}
}

void CGraphicsShader::CreateShader(const wstring& _strFileName, const string& _strFuncName, eSHADER_TYPE _ShaderType)
{
	// 1. Shader 파일 경로 받아옴
	std::filesystem::path shaderPath = GETRESPATH;
	shaderPath /= _strFileName;

	//1-1. 쉐이더 로드 타입 변경
	m_ShaderData[(int)_ShaderType].LoadType = eSHADER_LOADTYPE::RUNTIME_COMPILED;



	char ShaderNameVersion[32] = {};
	//2. 쉐이더 타입에 따른 다른 파라미터용 변수를 할당
	switch (_ShaderType)
	{
	case eSHADER_TYPE::__VERTEX:
		strcpy_s(ShaderNameVersion, 32u, SHADER_NAME_VERSION::VS);
		break;

	case eSHADER_TYPE::__HULL:
		strcpy_s(ShaderNameVersion, 32u, SHADER_NAME_VERSION::HS);
		break;

	case eSHADER_TYPE::__DOMAIN:
		strcpy_s(ShaderNameVersion, 32u, SHADER_NAME_VERSION::DS);
		break;

	case eSHADER_TYPE::__GEOMETRY:
		strcpy_s(ShaderNameVersion, 32u, SHADER_NAME_VERSION::GS);
		break;

	case eSHADER_TYPE::__PIXEL:
		strcpy_s(ShaderNameVersion, 32u, SHADER_NAME_VERSION::PS);
		break;

	default:
		//에러 발생시킴
		assert((int)eSHADER_TYPE::END == 0);
		break;
	}

	// Shader Compile
	if (FAILED(D3DCompileFromFile(shaderPath.wstring().c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), ShaderNameVersion, 0, 0, m_ShaderData[(int)_ShaderType].Blob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Vertex Shader Compile Failed!!", MB_OK);
	}

	//4. 다시 switch문으로 각 쉐이더별 함수를 호출
	switch (_ShaderType)
	{
	case eSHADER_TYPE::__VERTEX:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreateVertexShader(
			m_ShaderData[(int)_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[(int)_ShaderType].Blob->GetBufferSize()
			, nullptr,
			m_VS.GetAddressOf()
		)))
		{
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
				, "Vertex Shader Compile Failed!!", MB_OK);
		}

		 
		CreateDefaultInputLayout();

		break;
	case eSHADER_TYPE::__HULL:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreateHullShader(
			m_ShaderData[(int)_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[(int)_ShaderType].Blob->GetBufferSize()
			, nullptr,
			m_HS.GetAddressOf()
		)))
		{
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
				,"Hull Shader Compile Failed!!", MB_OK);
		}
		break;
	case eSHADER_TYPE::__DOMAIN:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreateDomainShader(
			m_ShaderData[(int)_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[(int)_ShaderType].Blob->GetBufferSize()
			, nullptr,
			m_DS.GetAddressOf()
		)))
		{
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
				, "Domain Shader Compile Failed!!", MB_OK);
		}
		break;
	case eSHADER_TYPE::__GEOMETRY:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreateGeometryShader(
			m_ShaderData[(int)_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[(int)_ShaderType].Blob->GetBufferSize()
			, nullptr,
			m_GS.GetAddressOf()
		)))
		{
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
				, "Geometry Shader Compile Failed!!", MB_OK);
		}
		break;
	case eSHADER_TYPE::__PIXEL:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreatePixelShader(
			m_ShaderData[(int)_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[(int)_ShaderType].Blob->GetBufferSize()
			, nullptr,
			m_PS.GetAddressOf()
		)))
		{
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
				, "Pixel Shader Compile Failed!!", MB_OK);
		}
		break;
	
	default:
		//에러 발생시킴
		assert((int)eSHADER_TYPE::END == 0);
		break;
	}
}

//void CGraphicsShader::CreateVertexShader(const wstring& _strFileName, const string& _strFuncName)
//{
//	// Shader 파일 경로
//	wstring strShaderFile = CPathMgr::GetInst()->GetContentAbsPathW();
//	strShaderFile += _strFileName;
//
//	// VertexShader Compile
//	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
//		, _strFuncName.c_str(), "vs_5_0", 0, 0, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
//	{
//		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
//			, "Vertex Shader Compile Failed!!", MB_OK);
//	}
//
//	// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
//	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
//		, nullptr, m_VS.GetAddressOf());
//
//	m_ShaderLoadType[eShaderType]
//	AddPipeLineStage(eSHADER_PIPELINE_STAGE::__VERTEX);
//}
//
//void CGraphicsShader::CreatePixelShader(const wstring& _strFileName, const string& _strFuncName)
//{
//	// Shader 파일 경로
//	wstring strShaderFile = CPathMgr::GetInst()->GetContentAbsPathW();
//	strShaderFile += _strFileName;
//
//
//	// PixelShader Compile	
//	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
//		, _strFuncName.c_str(), "ps_5_0", 0, 0, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
//	{
//		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
//			, "Pixel Shader Compile Failed!!", MB_OK);
//	}
//
//	// 컴파일된 객체로 PixelShader 를 만든다.
//	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize()
//		, nullptr, m_PS.GetAddressOf());
//
//	AddPipeLineStage(eSHADER_PIPELINE_STAGE::__PIXEL);
//}

//void CGraphicsShader::CreateVertexShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize)
//{
//
//	D3DCreateBlob(_ShaderByteCodeSize, &m_Blob[(int)eSHADER_TYPE::__VERTEX]);
//
//	//Vertex Shader Compilation by included header
//	DEVICE->CreateVertexShader(_pShaderByteCode, _ShaderByteCodeSize, nullptr, m_VS.GetAddressOf());
//	
//	// InputLayout 생성
//	D3D11_INPUT_ELEMENT_DESC LayoutDesc[3] = {};
//
//	LayoutDesc[0].SemanticName = "POSITION";
//	LayoutDesc[0].SemanticIndex = 0;
//	LayoutDesc[0].AlignedByteOffset = 0;
//	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
//	LayoutDesc[0].InputSlot = 0;
//	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//	LayoutDesc[0].InstanceDataStepRate = 0;
//
//	LayoutDesc[1].SemanticName = "COLOR";
//	LayoutDesc[1].SemanticIndex = 0;
//	LayoutDesc[1].AlignedByteOffset = 12;
//	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	LayoutDesc[1].InputSlot = 0;
//	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//	LayoutDesc[1].InstanceDataStepRate = 0;
//
//	LayoutDesc[2].SemanticName = "TEXCOORD";
//	LayoutDesc[2].SemanticIndex = 0;
//	LayoutDesc[2].AlignedByteOffset = 28;
//	LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
//	LayoutDesc[2].InputSlot = 0;
//	LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//	LayoutDesc[2].InstanceDataStepRate = 0;
//	
//
//	if (FAILED(DEVICE->CreateDefaultInputLayout(LayoutDesc, 3
//		, _pShaderByteCode, _ShaderByteCodeSize
//		, m_Layout.GetAddressOf())))
//	{
//		assert(nullptr);
//	}
//
//	AddPipeLineStage(eSHADER_PIPELINE_STAGE::__VERTEX);
//
//}
//
//void CGraphicsShader::CreatePixelShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize)
//{
//	DEVICE->CreatePixelShader(g_test_VS, sizeof(g_test_PS), nullptr, m_PS.GetAddressOf());
//}

void CGraphicsShader::BindData()
{
	ID3D11DeviceContext* pContext = CONTEXT;

	//Set Input Layout
	pContext->IASetInputLayout(m_Layout.Get());
	pContext->IASetPrimitiveTopology(m_eTopology);


	pContext->VSSetShader(m_VS.Get(), nullptr, 0);
	pContext->HSSetShader(m_HS.Get(), nullptr, 0);
	pContext->DSSetShader(m_DS.Get(), nullptr, 0);
	pContext->GSSetShader(m_GS.Get(), nullptr, 0);
	pContext->PSSetShader(m_PS.Get(), nullptr, 0);


	//Set Rasterizer
	pContext->RSSetState(CDevice::GetInst()->GetRSState(m_eRSType));

	//Set Output Merger(Depth Stencil, Blend)
	pContext->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_eDSType), 0);
	pContext->OMSetBlendState(CDevice::GetInst()->GetBSState(m_eBSType), Vec4(0.f, 0.f, 0.f, 0.f), UINT_MAX);
}




