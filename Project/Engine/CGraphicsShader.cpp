#include "pch.h"
#include "CGraphicsShader.h"

#include "CPathMgr.h"
#include "CDevice.h"


CGraphicsShader::CGraphicsShader()
	: CShader(eRES_TYPE::GRAPHICS_SHADER)
	, m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_ePIPELINE_STAGE_Flag()
	, m_RSType(eRASTERIZER_TYPE::CULL_BACK)
	, m_DSType(eDEPTHSTENCIL_TYPE::eDEPTHSTENCIL_TYPE_LESS)
	, m_BSType(eBLENDSTATE_TYPE::eBLENDSTATE_DEFAULT)
	, m_ShaderDomain(eSHADER_DOMAIN::eSHADER_DOMAIN_UNDEFINED)
	, m_ShaderData{}
{
}

CGraphicsShader::~CGraphicsShader()
{
}

void CGraphicsShader::CreateInputLayout()
{
	// InputLayout ����
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[3] = {};

	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;
	LayoutDesc[0].AlignedByteOffset = 0;
	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	LayoutDesc[0].InputSlot = 0;
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;

	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;

	//���� �ø�ƽ���� �󸶳� ���������� ���θ� ����. 0�� �ε����� R32B32G32(4+4+4 = 12) �̹Ƿ� �������� 12�� ����ش�.
	LayoutDesc[1].AlignedByteOffset = 12;					
	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[1].InputSlot = 0;
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[1].InstanceDataStepRate = 0;

	LayoutDesc[2].SemanticName = "TEXCOORD";
	LayoutDesc[2].SemanticIndex = 0;

	//0�� �ε��� : 12 , 1�� �ε��� : R32B32G32A32(4 * 4 = 16) -> 12 + 16 = 28
	LayoutDesc[2].AlignedByteOffset = 28;
	LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	LayoutDesc[2].InputSlot = 0;
	LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[2].InstanceDataStepRate = 0;


	//blob ���·� �ε����� ���� ��� ���·� �ε����� ��쿡 ���� �ٸ� ������ �����ؼ� �ε��Ѵ�.
	switch (m_ShaderData[eSHADERTYPE_VERTEX].LoadType)
	{
	case eSHADER_LOADTYPE::eSHADER_NOT_LOADED:
		//Vertrx Shader�� �ε�Ǿ����� ���� ��� assert
		assert((bool)m_ShaderData[eSHADERTYPE_VERTEX].LoadType);
		break;	

	case eSHADER_LOADTYPE::eSHADER_RUNTIME:
		if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 3,
			m_ShaderData[eSHADERTYPE_VERTEX].Blob->GetBufferPointer(),
			m_ShaderData[eSHADERTYPE_VERTEX].Blob->GetBufferSize(),
			m_Layout.GetAddressOf())))
		{
			assert(nullptr);
		}


		break;	

	case eSHADER_LOADTYPE::eSHADER_INCLUDE:
		if (FAILED(DEVICE->CreateInputLayout(
			LayoutDesc, 3,
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

		AddPipeLineStage(eSHADER_PIPELINE_FLAG_VERTEX);
		break;

	case eSHADERTYPE_HULL:
		AddPipeLineStage(eSHADER_PIPELINE_FLAG_HULL);
		break;

	case eSHADERTYPE_DOMAIN:
		AddPipeLineStage(eSHADER_PIPELINE_FLAG_DOMAIN);

		break;

	case eSHADERTYPE_GEOMETRY:
		AddPipeLineStage(eSHADER_PIPELINE_FLAG_GEOMETRY);
		break;

	case eSHADERTYPE_PIXEL:
		result = DEVICE->CreatePixelShader(_pShaderByteCode, _ShaderByteCodeSize, nullptr, m_PS.GetAddressOf());


		AddPipeLineStage(eSHADER_PIPELINE_FLAG_PIXEL);
		break;
	default:
		break;
	}

	assert(false == FAILED(result));
}

void CGraphicsShader::CreateShader(const wstring& _strFileName, const string& _strFuncName, eSHADERTYPE _ShaderType)
{
	// 1. Shader ���� ��� �޾ƿ�
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	//1-1. ���̴� �ε� Ÿ�� ����
	m_ShaderData[_ShaderType].LoadType = eSHADER_LOADTYPE::eSHADER_RUNTIME;


	char ShaderNameVersion[32] = {};
	//���� switch ������ _ShaderType�� �߸��Ǿ� ���� ��찡 �ɷ����Ƿ� ALL�� ����
	eSHADER_PIPELINE_STAGE_FLAG Stage = eSHADER_PIPELINE_FLAG_ALL;
	//2. ���̴� Ÿ�Կ� ���� �ٸ� �Ķ���Ϳ� ������ �Ҵ�
	switch (_ShaderType)
	{
	case eSHADERTYPE_VERTEX:
		strcpy_s(ShaderNameVersion, 32u, "vs_5_0");
		Stage = eSHADER_PIPELINE_FLAG_VERTEX;
		break;

	case eSHADERTYPE_HULL:
		strcpy_s(ShaderNameVersion, 32u, "hs_5_0");
		Stage = eSHADER_PIPELINE_FLAG_HULL;
		break;

	case eSHADERTYPE_DOMAIN:
		strcpy_s(ShaderNameVersion, 32u, "ds_5_0");
		Stage = eSHADER_PIPELINE_FLAG_DOMAIN;
		break;

	case eSHADERTYPE_GEOMETRY:
		strcpy_s(ShaderNameVersion, 32u, "gs_5_0");
		Stage = eSHADER_PIPELINE_FLAG_GEOMETRY;
		break;

	case eSHADERTYPE_PIXEL:
		strcpy_s(ShaderNameVersion, 32u, "ps_5_0");
		Stage = eSHADER_PIPELINE_FLAG_PIXEL;
		break;

	default:
		//���� �߻���Ŵ
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


	//4. �ٽ� switch������ �� ���̴��� �Լ��� ȣ��
	switch (_ShaderType)
	{
	case eSHADERTYPE_VERTEX:
		// �����ϵ� ��ü�� VertexShader, PixelShader �� �����.
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
		// �����ϵ� ��ü�� VertexShader, PixelShader �� �����.
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
		// �����ϵ� ��ü�� VertexShader, PixelShader �� �����.
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
		// �����ϵ� ��ü�� VertexShader, PixelShader �� �����.
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
		// �����ϵ� ��ü�� VertexShader, PixelShader �� �����.
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
		//���� �߻���Ŵ
		assert(eSHADERTYPE::eSHADERTYPE_END == 0);
		break;
	}
	
	//��� ������ ���������� ���������� ���������� ���
	AddPipeLineStage(Stage);
}

//void CGraphicsShader::CreateVertexShader(const wstring& _strFileName, const string& _strFuncName)
//{
//	// Shader ���� ���
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
//	// �����ϵ� ��ü�� VertexShader, PixelShader �� �����.
//	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
//		, nullptr, m_VS.GetAddressOf());
//
//	m_ShaderLoadType[eShaderType]
//	AddPipeLineStage(eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_VERTEX);
//}
//
//void CGraphicsShader::CreatePixelShader(const wstring& _strFileName, const string& _strFuncName)
//{
//	// Shader ���� ���
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
//	// �����ϵ� ��ü�� PixelShader �� �����.
//	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize()
//		, nullptr, m_PS.GetAddressOf());
//
//	AddPipeLineStage(eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_PIXEL);
//}

//void CGraphicsShader::CreateVertexShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize)
//{
//
//	D3DCreateBlob(_ShaderByteCodeSize, &m_Blob[eSHADERTYPE_VERTEX]);
//
//	//Vertex Shader Compilation by included header
//	DEVICE->CreateVertexShader(_pShaderByteCode, _ShaderByteCodeSize, nullptr, m_VS.GetAddressOf());
//	
//	// InputLayout ����
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
//	AddPipeLineStage(eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_VERTEX);
//
//}
//
//void CGraphicsShader::CreatePixelShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize)
//{
//	DEVICE->CreatePixelShader(g_test_VS, sizeof(g_test_PS), nullptr, m_PS.GetAddressOf());
//}

void CGraphicsShader::UpdateData()
{
	//Set Input Layout
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_eTopology);

	//Set Rasterizer
	CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType));

	//Set Output Merger(Depth Stencil, Blend)
	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType), 0);
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType), 0, UINT_MAX);


	if (eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_VERTEX & m_ePIPELINE_STAGE_Flag)
	{
		CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_HULL & m_ePIPELINE_STAGE_Flag)
	{
		CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_DOMAIN & m_ePIPELINE_STAGE_Flag)
	{
		CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_GEOMETRY & m_ePIPELINE_STAGE_Flag)
	{
		CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_PIXEL & m_ePIPELINE_STAGE_Flag)
	{
		CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);
	}
}


