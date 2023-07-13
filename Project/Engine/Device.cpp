#include "pch.h"
#include "Device.h"

#include "cEngine.h"
#include "cConstBuffer.h"

#include "S_H_TilemapComplete.hlsli"

Device::Device()
    : m_hWnd(nullptr)  
    , m_ViewPort{}
    , m_vecConstBuffer{}
    , m_arrRSState{}
    , m_arrDSState{}
    , m_arrBSState{}
{
    //m_vecConstBuffer을 기본 상수버퍼의 크기만큼 Resize
    m_vecConstBuffer.resize(REGISLOT_b_END);
}

Device::~Device()
{
    size_t size = m_vecConstBuffer.size();
    for (int i = 0; i < size; ++i)
    {
        SAFE_DELETE(m_vecConstBuffer[i]);
    }
}

int Device::init(HWND _hWnd, UINT _uWidth, UINT _uHeight)
{
    m_hWnd = _hWnd;    
    
    g_GlobalVal.u2Res = UINT32_2(_uWidth, _uHeight);
    g_GlobalVal.v2Res = Vec2(_uWidth, _uHeight);

    int iFlag = 0;
#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
    //iFlag |= D3D11_CREATE_DEVICE_DEBUGGABLE;
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
        MessageBox(nullptr, L"스왑체인 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    // 출력 타겟 설정
    SetRenderTarget();

    // ViewPort 설정
    m_ViewPort.TopLeftX = 0.f;
    m_ViewPort.TopLeftY = 0.f;


    m_ViewPort.Width = g_GlobalVal.v2Res.x;
    m_ViewPort.Height = g_GlobalVal.v2Res.y;

    m_ViewPort.MinDepth = 0.f;
    m_ViewPort.MaxDepth = 1.f;

    m_Context->RSSetViewports(1, &m_ViewPort);



    // 샘플러 생성
    if (FAILED(CreateSampler()))
    {
        MessageBox(nullptr, L"Sampler 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    //래스터라이저 스테이트 생성
    if (FAILED(CreateRasterizeState()))
    {
        MessageBoxW(nullptr, L"Rasterizer State 생성 실패", L"Rasterizer 초기화 에러", MB_OK);
    }

    if (FAILED(CreateDepthStencilState()))
    {
        MessageBoxW(nullptr, L"Depth-Stencil State 생성 실패", L"Depth-Stencil State 초기화 에러", MB_OK);
    }

    if (FAILED(CreateBlendState()))
    {
        MessageBoxW(nullptr, L"Blend State 생성 실패", L"Blend State 초기화 에러", MB_OK);
    }

    // 상수버퍼 생성
    CreateDefaultConstBuffer();


    return S_OK; // E_FAIL;
}

void Device::SetRenderTarget()
{
    // 출력 타겟 설정
    m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());
}

int Device::CreateSwapChain()
{
    // 스팅셈?설정
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    tDesc.OutputWindow = m_hWnd;    // 출력 윈도우
    tDesc.Windowed = true;          // 창모드, 전체화면 모드

    tDesc.BufferCount = 2;              
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    tDesc.BufferDesc.Width = g_GlobalVal.u2Res.x;
    tDesc.BufferDesc.Height = g_GlobalVal.u2Res.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tDesc.BufferDesc.RefreshRate.Denominator = 1;
    tDesc.BufferDesc.RefreshRate.Numerator = 0;
    //tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    
    tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    tDesc.Flags = 0;
    

    // 스팅셈?생성
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

int Device::CreateView()
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
    tDesc.Width = g_GlobalVal.u2Res.x;
    tDesc.Height = g_GlobalVal.u2Res.y;
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

int Device::CreateSampler()
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


void Device::ClearTarget(float(&_color)[4])
{
    m_Context->ClearRenderTargetView(m_RTV.Get(), _color);
    m_Context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}


void Device::CreateDefaultConstBuffer()
{
    tConstBufferClassDesc Desc = {};

    {
        //Vertex Shader에만 상수버퍼를 전달 + Light 처리를 위해서 픽셀 쉐이더에도 값을 전달한다.
        Desc.iRegisterNum = REGISLOT_b_CBUFFER_CAM_MATIRCES;
        Desc.PipelineStageBindFlag = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
        Desc.uElementSize = (UINT)sizeof(tCamMatrices);
        Desc.uElementCount = (UINT)eCAMERA_INDEX::END;

        //상수버퍼 생성 확인
        cConstBuffer* pBuffer = CreateConstBuffer(Desc);
        assert(pBuffer);
        pBuffer = nullptr;
    }

    {
        //Vertex + Pixel Shader에만 상수버퍼를 전달
        Desc.iRegisterNum = REGISLOT_b_CBUFFER_MTRL_SCALAR;
        Desc.uElementSize = (UINT)sizeof(tMtrlScalarData);
        Desc.uElementCount = 1u;

        //상수버퍼 생성 확인
        cConstBuffer* pBuffer = CreateConstBuffer(Desc);
        assert(pBuffer);
        pBuffer = nullptr;
    }

    {
        //텍스처의 경우 Pixel Shader에만 데이터를 전달한다.
        Desc.PipelineStageBindFlag = define_Shader::ePIPELINE_STAGE_FLAG::__PIXEL;
        Desc.iRegisterNum = REGISLOT_b_CBUFFER_MTRL_TEX;
        Desc.uElementSize = sizeof(tMtrlTexData);
        Desc.uElementCount = 1u;

        //상수버퍼 생성 확인
        cConstBuffer* pBuffer = CreateConstBuffer(Desc);
        assert(pBuffer);
        pBuffer = nullptr;
    }

    {
        //글로벌 데이터는 모든 쉐이더 파이프라인에서 접근할 수 있도록 설정
        Desc.iRegisterNum = REGISLOT_b_CBUFFER_GLOBAL;
        Desc.PipelineStageBindFlag = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
        Desc.uElementSize = (UINT)sizeof(tGlobalValue);
        Desc.uElementCount = 1u;

        //상수버퍼 생성 확인
        cConstBuffer* pBuffer = CreateConstBuffer(Desc);
        assert(pBuffer);
        pBuffer = nullptr;
    }

    {
        //글로벌 데이터는 모든 쉐이더 파이프라인에서 접근할 수 있도록 설정
        Desc.iRegisterNum = REGISLOT_b_CBUFFER_SBUFFER_SHAREDATA;
        Desc.PipelineStageBindFlag = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
        Desc.uElementSize = (UINT)sizeof(tSBufferInfo);
        Desc.uElementCount = 1u;

        //상수버퍼 생성 확인
        cConstBuffer* pBuffer = CreateConstBuffer(Desc);
        assert(pBuffer);
        pBuffer = nullptr;
    }

    {
        //글로벌 데이터는 모든 쉐이더 파이프라인에서 접근할 수 있도록 설정
        Desc.iRegisterNum = REGISLOT_b_CBUFFER_PARTICLE_MODULEDATA;
        Desc.PipelineStageBindFlag = define_Shader::ePIPELINE_STAGE_FLAG::__COMPUTE;
        Desc.uElementSize = (UINT)sizeof(tParticleModule);
        Desc.uElementCount = 1u;

        //상수버퍼 생성 확인
        cConstBuffer* pBuffer = CreateConstBuffer(Desc);
        assert(pBuffer);
        pBuffer = nullptr;
    }


    ////구조화 버퍼의 공유 자원을 보내는 상수 버퍼(ex. 등록된 구조화 버퍼의 count)
    ////이 값을 전달할 버퍼 데이터는 g_arrSBufferShareData(extern.cpp)이다.
    //CBufferTarget = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
    //m_vecConstBuffer[REGISLOT_b_CBUFFER_SBUFFER_SHAREDATA] = new cConstBuffer(REGISLOT_b_CBUFFER_SBUFFER_SHAREDATA);
    //m_vecConstBuffer[REGISLOT_b_CBUFFER_SBUFFER_SHAREDATA]->Create(sizeof(tSBufferInfo), (UINT)eCBUFFER_SBUFFER_SHAREDATA_IDX::END);
    //m_vecConstBuffer[REGISLOT_b_CBUFFER_SBUFFER_SHAREDATA]->SetPipelineTarget(CBufferTarget);

    

}


HRESULT Device::CreateRasterizeState()
{
    D3D11_RASTERIZER_DESC Desc = {};

    //1. 기본값은 nullptr로 지정해준다.(기본설정을 굳이 생성할필요X)
    m_arrRSState[(UINT)define_Shader::eRASTERIZER_TYPE::CULL_BACK] = nullptr;

    //2. 프론트페이스 컬링
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;

    HRESULT Result = S_OK;
    Result = DEVICE->CreateRasterizerState(&Desc, &m_arrRSState[(UINT)define_Shader::eRASTERIZER_TYPE::CULL_FRONT]);

    //3. 컬링 하지 않음
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    Result = DEVICE->CreateRasterizerState(&Desc, &m_arrRSState[(UINT)define_Shader::eRASTERIZER_TYPE::CULL_NONE]);

    //3.와이어프레임
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    Result = DEVICE->CreateRasterizerState(&Desc, &m_arrRSState[(UINT)define_Shader::eRASTERIZER_TYPE::WIRE_FRAME]);


    return Result;
}

HRESULT Device::CreateDepthStencilState()
{
    HRESULT Result = S_OK;

    for (UINT i = 0; i < (UINT)define_Shader::eDEPTH_STENCIL_TYPE::END; ++i)
    {
        D3D11_DEPTH_STENCIL_DESC Desc = {};

        switch ((define_Shader::eDEPTH_STENCIL_TYPE)i)
        {
        case define_Shader::eDEPTH_STENCIL_TYPE::LESS:

            //이건 기본값이므로 nullptr을 준다.
            m_arrDSState[i] = nullptr;
            continue;
            break;

        case define_Shader::eDEPTH_STENCIL_TYPE::LESS_EQUAL:
            Desc.DepthEnable = true;
            Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
            Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            Desc.StencilEnable = false;
            break;

        case define_Shader::eDEPTH_STENCIL_TYPE::GREATER:
            Desc.DepthEnable = true;
            Desc.DepthFunc = D3D11_COMPARISON_GREATER;
            Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            Desc.StencilEnable = false;
            break;

        case define_Shader::eDEPTH_STENCIL_TYPE::GREATER_EQUAL:
            Desc.DepthEnable = true;
            Desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
            Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            Desc.StencilEnable = false;
            break;

        case define_Shader::eDEPTH_STENCIL_TYPE::NO_WRITE:
            Desc.DepthEnable = true;
            Desc.DepthFunc = D3D11_COMPARISON_LESS;
            Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
            Desc.StencilEnable = false;
            break;

        case define_Shader::eDEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE:
            Desc.DepthEnable = false;
            Desc.DepthFunc = D3D11_COMPARISON_NEVER;
            Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
            Desc.StencilEnable = false;
            break;

        default:
            break;
        }

        Result = DEVICE->CreateDepthStencilState(&Desc, m_arrDSState[i].GetAddressOf());
    }


    return Result;
}

HRESULT Device::CreateBlendState()
{
    HRESULT Result = S_OK;

    for (int i = 0; i < (UINT)define_Shader::eBLEND_STATE_TYPE::END; ++i)
    {
        D3D11_BLEND_DESC Desc = {};

        switch ((define_Shader::eBLEND_STATE_TYPE)i)
        {
        case define_Shader::eBLEND_STATE_TYPE::DEFAULT:
            m_arrBSState[i] = nullptr;
            continue;
            break;

        case define_Shader::eBLEND_STATE_TYPE::MASK:
            //Alpha-To-Coverage 기능을 활성화한다.
            //Alpha-To-Coverage 기능은 
            Desc.AlphaToCoverageEnable = true;
            
            //아래 옵션은 각 RenderTarget 별로 다른 옵션을 사용할 것인지 여부를 결정한다.
            //사용으로 설정할 8개의 각 Render Target 별로 다른 알파블렌딩 옵션을 지정할 수 있으며(Desc.RenderTarget[index])
            //사용하지 않음으로 설정했을 시 Desc.RenderTarget[0]의 옵션만 사용된다. 나머지는 전부 무시된다.
            Desc.IndependentBlendEnable = false;
            Desc.RenderTarget[0].BlendEnable = true;

            //ADD = 아래 지정한 설정대로 알파값을 반영한 기존(Dest) 색상과 입력(Src) 색상의 색을 더한다.
            //Mask의 경우 Alpha-To-Coverage 옵션을 사용하므로 덮어쓰는 쪽의 색상을 1로 해준다.
            Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
            
            Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

            //어떤 색상을 덮어씌울건지를 지정한다.
            //RGB 중 특정 색상만 집어넣도록 할 수도 있으나, 쉐이더 코드로도 가능한 작업이고, 오히려 Blend State 단계에서 처리하는 것이 더 불편하므로 그냥 전부 ALL로 해 주는 것이 좋다.
            Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            break;

        case define_Shader::eBLEND_STATE_TYPE::ALPHA_BLEND:
            Desc.AlphaToCoverageEnable = false;
            Desc.IndependentBlendEnable = false;
            Desc.RenderTarget[0].BlendEnable = true;

            //ADD = 기존(Dest) 색상과 입력(Src) 색상의 색을 더한다.
            //Dest는 뒷쪽의 오브젝트 색상이고 Src는 앞쪽의 오브젝트 색상이라고 보면 편함.
            //뒷쪽의 오브젝트는 이미 색상일 칠해진 픽셀이고, 그려질 때 알파값이 적용되어 색칠되었을 것이다.
            //앞쪽의 오브젝트는 이제 덮어씌워질 색상인데, 앞쪽 오브젝트의 투명도가 얼마냐에 따라서 구분해서 적용되어야 할 것이다.
            //그러므로 Src 픽셀은 alpha 값만큼 투명하게(연하게) 그려주고, Dest 픽셀은 Src의 Alpha값에서 남은 값 만큼 칠해 주면
            //반투명하다는 느낌을 줄 수 있게 된다.
            Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;


            //사실 색상이 결정된 이후에는 알파값이 필요가 없다. 
            //색상이 결정되었다는 그려졌다는 뜻인데, 이건 이제 Dest 픽셀이 되는 것이고,
            //위의 설명을 보면 그려질  Src의 알파값만 고려한다는 사실을 알 수 있다.
            //그러므로 Src의 알파값을 그대로 저장해준다.
            Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

            Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

            break;
        case define_Shader::eBLEND_STATE_TYPE::ONE_ONE: 
            Desc.AlphaToCoverageEnable = true;
            Desc.IndependentBlendEnable = false;
            Desc.RenderTarget[0].BlendEnable = true;

            //ADD = 기존(Dest) 색상과 입력(Src) 색상의 색을 더한다.

            Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

            Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

            //어떤 색상을 덮어씌울건지를 지정한다.
            //RGB 중 특정 색상만 집어넣도록 할 수도 있으나, 쉐이더 코드로 가능한 작업이고, 오히려 Blend State 단계에서 처리하는 것이 더 불편하므로 그냥 전부 ALL로 해 주는 것이 좋다.
            Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            break;

        default:
            break;
        }

        Result = DEVICE->CreateBlendState(&Desc, &m_arrBSState[i]);
    }


    return Result;
}

cConstBuffer* Device::CreateConstBuffer(tConstBufferClassDesc const& _tDesc)
{
    //이미 해당 레지스터 번호에 생성된 상수 버퍼가 있을 경우 생성 불가.
    if (_tDesc.iRegisterNum < 0 || nullptr != m_vecConstBuffer[_tDesc.iRegisterNum])
    {
        DEBUG_BREAK;
        return nullptr;
    }

    cConstBuffer* pCBuffer = new cConstBuffer;
    HRESULT hr = pCBuffer->Create(_tDesc);

    if (FAILED(hr))
    {
        DEBUG_BREAK;
        SAFE_DELETE(pCBuffer);
        return nullptr;
    }

    //벡터 크기 조절 후 데이터 삽입
    if (m_vecConstBuffer.size() <= _tDesc.iRegisterNum)
        m_vecConstBuffer.resize((size_t)(_tDesc.iRegisterNum + 1));

    m_vecConstBuffer[_tDesc.iRegisterNum] = pCBuffer;

    return pCBuffer;
}

