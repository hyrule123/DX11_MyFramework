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

		//현재 연결된 레지스터 번호와 파이프라인을 초기화
		m_iCurBoundRegister = -1;
		m_flagCurBoundPipeline = 0u;
	}

	else if (eCURRENT_BOUND_VIEW::UAV == m_eCurBoundView)
	{
		assert(0 <= m_iCurBoundRegister);

		ID3D11UnorderedAccessView* pUAV = nullptr;
		UINT u = -1;
		CONTEXT->CSSetUnorderedAccessViews(m_iCurBoundRegister, 1, &pUAV, &u);

		//현재 연결된 레지스터 번호와 파이프라인을 초기화
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
	//컴퓨트쉐이더와의 바인딩 해제
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
	
	//바인딩한 Compute Shader 버퍼 번호를 기억
	m_eCurBoundView = eCURRENT_BOUND_VIEW::UAV;
	m_iCurBoundRegister = _iRegisterNum;

	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_UAV.GetAddressOf(), &i);
}



int CTexture::Load(const wstring& _strFilePath)
{
	wchar_t szExt[50] = L"";
	//_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);
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
		MessageBox(nullptr, L"리소스 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	hr = CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ShaderResourceView 생성 실패", L"텍스쳐 로딩 실패", MB_OK);
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

	//CPU의 읽기/쓰기 가능 여부를 설정
	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (D3D11_USAGE::D3D11_USAGE_STAGING == _Usage)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	//원본만 생성.(자세한 내용은 밉맵을 찾아볼 것)
	m_Desc.MipLevels = 1;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	//텍스처를 생성
	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
		return E_FAIL;

	//바인드 플래그에 맞는 View를 생성해준다.
	//DEPTH_STENCIL 뷰로 사용하는 텍스처의 경우 다른 뷰와 동시에 바인딩을 할 수 없으므로 참고할 것.
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
