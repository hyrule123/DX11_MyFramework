#include "pch.h"
#include "CDevice.h"

#include "CEngine.h"
#include "CConstBuffer.h"

CDevice::CDevice()
    : m_hWnd(nullptr)  
    , m_ViewPort{}
    , m_arrConstBuffer{}
    , m_RSState{}
{
}

CDevice::~CDevice()
{
    Safe_Del_Array(m_arrConstBuffer);
}

int CDevice::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
    m_hWnd = _hWnd;    
    m_vRenderResolution = Vec2((float)_iWidth, (float)_iHeight);

    int iFlag = 0;
#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

      

    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
        , nullptr, iFlag
        , nullptr, 0
        , D3D11_SDK_VERSION
        , m_Device.GetAddressOf(), &eLevel
        , m_Context.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"스왚체인 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }





    // 출력 타겟 설정
    m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());


    // ViewPort 설정
    m_ViewPort.TopLeftX = 0.f;
    m_ViewPort.TopLeftY = 0.f;

    Vec2 vWindowResol = CEngine::GetInst()->GetWindowResolution();
    m_ViewPort.Width = vWindowResol.x;
    m_ViewPort.Height = vWindowResol.y;

    m_ViewPort.MinDepth = 0.f;
    m_ViewPort.MaxDepth = 1.f;

    m_Context->RSSetViewports(1, &m_ViewPort);

    //래스터라이저 스테이트 생성
    if (FAILED(CreateRasterizeState()))
    {
        MessageBoxW(nullptr, L"Rasterizer 생성 실패", L"Rasterizer 초기화 문제발생", MB_OK);
    }

    // 샘플러 생성
    if (FAILED(CreateSampler()))
    {
        MessageBox(nullptr, L"샘플러 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }


    // 상수버퍼 생성
    CreateConstBuffer();


    return S_OK; // E_FAIL;
}

int CDevice::CreateSwapChain()
{
    // 스왚체인 설정
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    tDesc.OutputWindow = m_hWnd;    // 출력 윈도우
    tDesc.Windowed = true;          // 창모드, 전체화면 모드

    tDesc.BufferCount = 1;              
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tDesc.BufferDesc.RefreshRate.Denominator = 1;
    tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    
    tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    tDesc.Flags = 0;
    

    // 스왚체인 생성
    ComPtr<IDXGIDevice>  pDXGIDevice;
    ComPtr<IDXGIAdapter> pAdapter;
    ComPtr<IDXGIFactory> pFactory;

    HRESULT hr = S_OK;

    hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
    hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
    hr = pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

    hr = pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf());

    return hr;
}

int CDevice::CreateView()
{
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());
    
    if (FAILED(m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTV.GetAddressOf())))
    {
        return E_FAIL;
    }

    // DepthStencil 용도 텍스쳐 생성
    D3D11_TEXTURE2D_DESC tDesc = {};

    tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // 반드시 렌더타겟과 같은 해상도로 설정해야 함
    tDesc.Width = (UINT)m_vRenderResolution.x;
    tDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.ArraySize = 1;

    tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;    
    tDesc.Usage = D3D11_USAGE_DEFAULT;
    tDesc.CPUAccessFlags = 0;

    tDesc.MipLevels = 1;    // 원본만 생성

    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    

    if (FAILED(m_Device->CreateTexture2D(&tDesc, nullptr, m_DSTex.GetAddressOf())))
    {
        return E_FAIL;
    }

    // DepthStencilView 생성
    if (FAILED(m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSV.GetAddressOf())))
    {
        return E_FAIL;
    }


    return S_OK;
}

int CDevice::CreateSampler()
{
    D3D11_SAMPLER_DESC tSamDesc = {};

    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.Filter   = D3D11_FILTER_ANISOTROPIC;    
    DEVICE->CreateSamplerState(&tSamDesc, m_Sampler[0].GetAddressOf());

    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    DEVICE->CreateSamplerState(&tSamDesc, m_Sampler[1].GetAddressOf());

    CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

    CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());

    return S_OK;
}


void CDevice::ClearTarget(float(&_color)[4])
{
    m_Context->ClearRenderTargetView(m_RTV.Get(), _color);
    m_Context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}


void CDevice::CreateConstBuffer()
{
    m_arrConstBuffer[(UINT)eCB_TYPE::TRANSFORM] = new CConstBuffer((UINT)eCB_TYPE::TRANSFORM);
    m_arrConstBuffer[(UINT)eCB_TYPE::TRANSFORM]->Create(sizeof(Matrix), 1);

    //Vertex Shader에만 상수버퍼를 전달
    UINT8 CBufferTarget = (UINT)eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_VERTEX;
    m_arrConstBuffer[(UINT)eCB_TYPE::TRANSFORM]->SetPipelineTarget(CBufferTarget);

    m_arrConstBuffer[(UINT)eCB_TYPE::MATERIAL] = new CConstBuffer((UINT)eCB_TYPE::MATERIAL);
    m_arrConstBuffer[(UINT)eCB_TYPE::MATERIAL]->Create(sizeof(tMtrlConst), 1);

    //Vertex + Pixel Shader에만 상수버퍼를 전달
    CBufferTarget |= (UINT)eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_PIXEL;
    m_arrConstBuffer[(UINT)eCB_TYPE::MATERIAL]->SetPipelineTarget(CBufferTarget);
}

HRESULT CDevice::CreateRasterizeState()
{
    D3D11_RASTERIZER_DESC Desc = {};

    //1. 기본값은 nullptr로 지정해준다.(기본설정을 굳이 생성할필요X)
    m_RSState[(UINT)eRS_TYPE::CULL_BACK] = nullptr;

    //2. 프론트페이스 컬링
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;

    HRESULT Result = S_OK;
    Result = DEVICE->CreateRasterizerState(&Desc, &m_RSState[(UINT)eRS_TYPE::CULL_FRONT]);

    //3. 컬링 하지 않음
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    Result = DEVICE->CreateRasterizerState(&Desc, &m_RSState[(UINT)eRS_TYPE::CULL_NONE]);

    //3.와이어프레임
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    Result = DEVICE->CreateRasterizerState(&Desc, &m_RSState[(UINT)eRS_TYPE::WIRE_FRAME]);



        

    return Result;
}
