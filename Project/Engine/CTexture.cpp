#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

void CTexture::UnBind()
{
	if (eCURRENT_BOUND_VIEW::NONE == m_eCurBoundView)
		return;

	else if (eCURRENT_BOUND_VIEW::SRV == m_eCurBoundView)
	{
		assert(0 <= m_iCurBoundRegister);

		ID3D11ShaderResourceView* pSrv = nullptr;

		if (eSHADER_PIPELINE_STAGE::__VERTEX & m_flagCurBoundPipeline)
		{
			CONTEXT->VSSetShaderResources(m_iCurBoundRegister, 1, &pSrv);
		}

		if (eSHADER_PIPELINE_STAGE::__HULL & m_flagCurBoundPipeline)
		{
			CONTEXT->HSSetShaderResources(m_iCurBoundRegister, 1, &pSrv);
		}

		if (eSHADER_PIPELINE_STAGE::__DOMAIN & m_flagCurBoundPipeline)
		{
			CONTEXT->DSSetShaderResources(m_iCurBoundRegister, 1, &pSrv);
		}

		if (eSHADER_PIPELINE_STAGE::__GEOMETRY & m_flagCurBoundPipeline)
		{
			CONTEXT->GSSetShaderResources(m_iCurBoundRegister, 1, &pSrv);
		}

		if (eSHADER_PIPELINE_STAGE::__PIXEL & m_flagCurBoundPipeline)
		{
			CONTEXT->PSSetShaderResources(m_iCurBoundRegister, 1, &pSrv);
		}

		if (eSHADER_PIPELINE_STAGE::__COMPUTE & m_flagCurBoundPipeline)
		{
			CONTEXT->CSSetShaderResources(m_iCurBoundRegister, 1, &pSrv);
		}

		//���� ����� �������� ��ȣ�� ������������ �ʱ�ȭ
		m_iCurBoundRegister = -1;
		m_flagCurBoundPipeline = 0u;
	}

	else if (eCURRENT_BOUND_VIEW::UAV == m_eCurBoundView)
	{
		assert(0 <= m_iCurBoundRegister);

		ID3D11UnorderedAccessView* pUAV = nullptr;
		UINT u = -1;
		CONTEXT->CSSetUnorderedAccessViews(m_iCurBoundRegister, 1, &pUAV, &u);

		//���� ����� �������� ��ȣ�� ������������ �ʱ�ȭ
		m_iCurBoundRegister = -1;
		m_flagCurBoundPipeline = 0u;
	}

	else if (
		eCURRENT_BOUND_VIEW::RTV == m_eCurBoundView
		||
		eCURRENT_BOUND_VIEW::DSV == m_eCurBoundView
		)
	{
		ID3D11RenderTargetView* pRTV = nullptr;
		ID3D11DepthStencilView* pDSV = nullptr;

		CONTEXT->OMSetRenderTargets(1, &pRTV, pDSV);
	}
	
	m_eCurBoundView = eCURRENT_BOUND_VIEW::NONE;
}

CTexture::CTexture()
	: CRes(eRES_TYPE::TEXTURE)
	, m_Desc{}
	, m_Image{}

{
}

CTexture::~CTexture()
{
}


void CTexture::BindData_SRV(int _iRegisterNum, UINT _eSHADER_PIPELINE_STAGE)
{
	//��ǻƮ���̴����� ���ε� ����
	UnBind();

	m_iCurBoundRegister = _iRegisterNum;
	m_flagCurBoundPipeline = _eSHADER_PIPELINE_STAGE;
	m_eCurBoundView = eCURRENT_BOUND_VIEW::SRV;

	if (eSHADER_PIPELINE_STAGE::__VERTEX & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__HULL & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__DOMAIN & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__GEOMETRY & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__PIXEL & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__COMPUTE & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}
}

void CTexture::BindData_UAV(int _iRegisterNum)
{
	UnBind();
	
	//���ε��� Compute Shader ���� ��ȣ�� ���
	m_eCurBoundView = eCURRENT_BOUND_VIEW::UAV;
	m_iCurBoundRegister = _iRegisterNum;

	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_UAV.GetAddressOf(), &i);
}



int CTexture::Load(const wstring& _strFilePath)
{
	wchar_t szExt[50] = L"";
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);
	wstring strExt = szExt;

	HRESULT hr = S_OK;
	if (L".dds" == strExt || L".DDS" == strExt)
	{
		// dds, DDS
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
	}

	else if (L".tga" == strExt || L".TGA" == strExt)
	{
		// tga, TGA
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}

	else
	{
		// png, jpg, jpeg, bmp
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
	}
	
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���ҽ� �ε� ����", L"�ؽ��� �ε� ����", MB_OK);
		return E_FAIL;
	}

	hr = CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ShaderResourceView ���� ����", L"�ؽ��� �ε� ����", MB_OK);
		return E_FAIL;
	}

	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());
	m_Tex2D->GetDesc(&m_Desc);

	return S_OK;
}


int CTexture::Save(const wstring& _strRelativePath)
{
	return S_OK;
}

int CTexture::Create(UINT _uWidth, UINT _uHeight, DXGI_FORMAT _pixelFormat, UINT _D3D11_BIND_FLAG, D3D11_USAGE _Usage)
{
	m_Desc.Format = _pixelFormat;

	m_Desc.Width = _uWidth;
	m_Desc.Height = _uHeight;
	m_Desc.ArraySize = 1;

	m_Desc.BindFlags = _D3D11_BIND_FLAG;
	m_Desc.Usage = _Usage;

	//CPU�� �б�/���� ���� ���θ� ����
	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (D3D11_USAGE::D3D11_USAGE_STAGING == _Usage)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	//������ ����.(�ڼ��� ������ �Ӹ��� ã�ƺ� ��)
	m_Desc.MipLevels = 1;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	//�ؽ�ó�� ����
	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
		return E_FAIL;

	//���ε� �÷��׿� �´� View�� �������ش�.
	//DEPTH_STENCIL ��� ����ϴ� �ؽ�ó�� ��� �ٸ� ��� ���ÿ� ���ε��� �� �� �����Ƿ� ������ ��.
	return CreateView();
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _pTex2D)
{
	if (nullptr == _pTex2D.Get())
		return E_FAIL;

	m_Tex2D = _pTex2D;
	m_Tex2D->GetDesc(&m_Desc);


	return CreateView();
}

int CTexture::CreateView()
{
	if (0u == m_Desc.BindFlags)
		return E_FAIL;

	if (D3D11_BIND_DEPTH_STENCIL & m_Desc.BindFlags)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
			return E_FAIL;
	}
	else
	{
		if (D3D11_BIND_SHADER_RESOURCE & m_Desc.BindFlags)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
				return E_FAIL;
		}

		if (D3D11_BIND_UNORDERED_ACCESS & m_Desc.BindFlags)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
				return E_FAIL;
		}
	}

	return S_OK;
}
