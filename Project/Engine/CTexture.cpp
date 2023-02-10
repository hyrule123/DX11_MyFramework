#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
	: CRes(eRES_TYPE::TEXTURE)
	, m_Desc{}
	, m_Image{}
	, m_iRecentGSNum(-1)
	, m_iRecentCSNum(-1)
	, m_uRecentGSTarget()

{
}

CTexture::~CTexture()
{
}


void CTexture::BindData(int _iRegisterNum, UINT8 _eSHADER_PIPELINE_STAGE)
{
	//��ǻƮ���̴����� ���ε� ����
	UnBind_CS();

	m_iRecentGSNum = _iRegisterNum;
	m_uRecentGSTarget = _eSHADER_PIPELINE_STAGE;

	if (eSHADER_PIPELINE_STAGE_FLAG::__VERTEX & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__HULL & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__DOMAIN & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__GEOMETRY & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__PIXEL & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__COMPUTE & _eSHADER_PIPELINE_STAGE)
	{
		CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}
}

void CTexture::UnBind()
{
	if (-1 == m_iRecentGSNum)
		return;

	ID3D11ShaderResourceView* pSrv = nullptr;
	
	if (eSHADER_PIPELINE_STAGE_FLAG::__VERTEX & m_uRecentGSTarget)
	{
		CONTEXT->VSSetShaderResources(m_iRecentGSNum, 1, &pSrv);
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__HULL & m_uRecentGSTarget)
	{
		CONTEXT->HSSetShaderResources(m_iRecentGSNum, 1, &pSrv);
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__DOMAIN & m_uRecentGSTarget)
	{
		CONTEXT->DSSetShaderResources(m_iRecentGSNum, 1, &pSrv);
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__GEOMETRY & m_uRecentGSTarget)
	{
		CONTEXT->GSSetShaderResources(m_iRecentGSNum, 1, &pSrv);
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__PIXEL & m_uRecentGSTarget)
	{
		CONTEXT->PSSetShaderResources(m_iRecentGSNum, 1, &pSrv);
	}

	if (eSHADER_PIPELINE_STAGE_FLAG::__COMPUTE & m_uRecentGSTarget)
	{
		CONTEXT->CSSetShaderResources(m_iRecentGSNum, 1, &pSrv);
	}

	m_iRecentGSNum = -1;
	m_uRecentGSTarget = (UINT8)0u;
}

void CTexture::BindData_CS(int _iRegisterNum)
{
	UnBind();
	
	//���ε��� Compute Shader ���� ��ȣ�� ���
	m_iRecentCSNum = _iRegisterNum;

	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_UAV.GetAddressOf(), &i);
}

void CTexture::UnBind_CS()
{
	if (-1 == m_iRecentCSNum)
		return;

	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_iRecentCSNum, 1, &pUAV, &i);

	m_iRecentCSNum = -1;
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
