#include "pch.h"
#include "CGraphicsShader.h"

#include "CPathMgr.h"
#include "CDevice.h"


CGraphicsShader::CGraphicsShader()
	: CShader(eRES_TYPE::GRAPHICS_SHADER)
	, m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_RSType(eRASTERIZER_TYPE::CULL_BACK)
	, m_DSType(eDEPTHSTENCIL_TYPE::LESS)
	, m_BSType(eBLENDSTATE_TYPE::DEFAULT)
	, m_ShaderDomain(eSHADER_DOMAIN::_UNDEFINED)
	, m_ShaderData{}
{
}

CGraphicsShader::~CGraphicsShader()
{
}

void CGraphicsShader::CreateInputLayout()
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
	switch (m_ShaderData[eSHADERTYPE_VERTEX].LoadType)
	{
	case eSHADER_LOADTYPE::eSHADER_NOT_LOADED:
		//Vertrx Shader가 로드되어있지 않을 경우 assert
		assert((bool)m_ShaderData[eSHADERTYPE_VERTEX].LoadType);
		break;	

	case eSHADER_LOADTYPE::eSHADER_RUNTIME:
		if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 2,
			m_ShaderData[eSHADERTYPE_VERTEX].Blob->GetBufferPointer(),
			m_ShaderData[eSHADERTYPE_VERTEX].Blob->GetBufferSize(),
			m_Layout.GetAddressOf())))
		{
			assert(nullptr);
		}


		break;	

	case eSHADER_LOADTYPE::eSHADER_INCLUDE:
		if (FAILED(DEVICE->CreateInputLayout(
			LayoutDesc, 2,
			m_ShaderData[eSHADERTYPE_VERTEX].pByteCode,
			m_ShaderData[eSHADERTYPE_VERTEX].ByteCodeSize,
			m_Layout.GetAddressOf())))
		{
			assert(nullptr);
		}

		break;


	default:
		break;
	}



}

void CGraphicsShader::CreateShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize, eSHADERTYPE _ShaderType)
{
	m_ShaderData[_ShaderType].LoadType = eSHADER_LOADTYPE::eSHADER_INCLUDE;
	m_ShaderData[_ShaderType].pByteCode = _pShaderByteCode;
	m_ShaderData[_ShaderType].ByteCodeSize = _ShaderByteCodeSize;

	HRESULT result = S_OK;

	switch (_ShaderType)
	{
	case eSHADERTYPE_VERTEX:
		result = DEVICE->CreateVertexShader(
			_pShaderByteCode, _ShaderByteCodeSize,
			nullptr,
			m_VS.GetAddressOf()
		);

		CreateInputLayout();
		break;

	case eSHADERTYPE_HULL:
		break;

	case eSHADERTYPE_DOMAIN:


		break;

	case eSHADERTYPE_GEOMETRY:
		result = DEVICE->CreateGeometryShader(
			_pShaderByteCode, _ShaderByteCodeSize,
			nullptr,
			m_GS.GetAddressOf()
		);

		break;

	case eSHADERTYPE_PIXEL:
		result = DEVICE->CreatePixelShader(_pShaderByteCode, _ShaderByteCodeSize, nullptr, m_PS.GetAddressOf());
		break;
	default:
		break;
	}

	assert(false == FAILED(result));
}

void CGraphicsShader::CreateShader(const wstring& _strFileName, const string& _strFuncName, eSHADERTYPE _ShaderType)
{
	// 1. Shader 파일 경로 받아옴
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	//1-1. 쉐이더 로드 타입 변경
	m_ShaderData[_ShaderType].LoadType = eSHADER_LOADTYPE::eSHADER_RUNTIME;


	char ShaderNameVersion[32] = {};
	//2. 쉐이더 타입에 따른 다른 파라미터용 변수를 할당
	switch (_ShaderType)
	{
	case eSHADERTYPE_VERTEX:
		strcpy_s(ShaderNameVersion, 32u, "vs_5_0");
		break;

	case eSHADERTYPE_HULL:
		strcpy_s(ShaderNameVersion, 32u, "hs_5_0");
		break;

	case eSHADERTYPE_DOMAIN:
		strcpy_s(ShaderNameVersion, 32u, "ds_5_0");
		break;

	case eSHADERTYPE_GEOMETRY:
		strcpy_s(ShaderNameVersion, 32u, "gs_5_0");
		break;

	case eSHADERTYPE_PIXEL:
		strcpy_s(ShaderNameVersion, 32u, "ps_5_0");
		break;

	default:
		//에러 발생시킴
		assert(eSHADERTYPE::eSHADERTYPE_END != 0);
		break;
	}

	// 3. VertexShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), ShaderNameVersion, 0, 0, m_ShaderData[_ShaderType].Blob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Vertex Shader Compile Failed!!", MB_OK);
	}

	//4. 다시 switch문으로 각 쉐이더별 함수를 호출
	switch (_ShaderType)
	{
	case eSHADERTYPE_VERTEX:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreateVertexShader(
			m_ShaderData[_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[_ShaderType].Blob->GetBufferSize()
			, nullptr,
			m_VS.GetAddressOf()
		)))
		{
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
				, "Vertex Shader Compile Failed!!", MB_OK);
		}

		 
		CreateInputLayout();

		break;
	case eSHADERTYPE_HULL:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreateHullShader(
			m_ShaderData[_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[_ShaderType].Blob->GetBufferSize()
			, nullptr,
			m_HS.GetAddressOf()
		)))
		{
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
				,"Hull Shader Compile Failed!!", MB_OK);
		}
		break;
	case eSHADERTYPE_DOMAIN:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreateDomainShader(
			m_ShaderData[_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[_ShaderType].Blob->GetBufferSize()
			, nullptr,
			m_DS.GetAddressOf()
		)))
		{
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
				, "Domain Shader Compile Failed!!", MB_OK);
		}
		break;
	case eSHADERTYPE_GEOMETRY:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreateGeometryShader(
			m_ShaderData[_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[_ShaderType].Blob->GetBufferSize()
			, nullptr,
			m_GS.GetAddressOf()
		)))
		{
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
				, "Geometry Shader Compile Failed!!", MB_OK);
		}
		break;
	case eSHADERTYPE_PIXEL:
		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		if (FAILED(DEVICE->CreatePixelShader(
			m_ShaderData[_ShaderType].Blob->GetBufferPointer(),
			m_ShaderData[_ShaderType].Blob->GetBufferSize()
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
		assert(eSHADERTYPE::eSHADERTYPE_END == 0);
		break;
	}
}

//void CGraphicsShader::CreateVertexShader(const wstring& _strFileName, const string& _strFuncName)
//{
//	// Shader 파일 경로
//	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
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
//	AddPipeLineStage(eSHADER_PIPELINE_STAGE_FLAG::__VERTEX);
//}
//
//void CGraphicsShader::CreatePixelShader(const wstring& _strFileName, const string& _strFuncName)
//{
//	// Shader 파일 경로
//	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
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
//	AddPipeLineStage(eSHADER_PIPELINE_STAGE_FLAG::__PIXEL);
//}

//void CGraphicsShader::CreateVertexShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize)
//{
//
//	D3DCreateBlob(_ShaderByteCodeSize, &m_Blob[eSHADERTYPE_VERTEX]);
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
//	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 3
//		, _pShaderByteCode, _ShaderByteCodeSize
//		, m_Layout.GetAddressOf())))
//	{
//		assert(nullptr);
//	}
//
//	AddPipeLineStage(eSHADER_PIPELINE_STAGE_FLAG::__VERTEX);
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
	pContext->RSSetState(CDevice::GetInst()->GetRSState(m_RSType));

	//Set Output Merger(Depth Stencil, Blend)
	pContext->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType), 0);
	pContext->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType), Vec4(0.f, 0.f, 0.f, 0.f), UINT_MAX);
}




