#include "pch.h"
#include "CDevice.h"

#include "CEngine.h"
#include "CConstBuffer.h"

CDevice::CDevice()
    : m_hWnd(nullptr)  
    , m_ViewPort{}
    , m_arrConstBuffer{}
    , m_arrRSState{}
    , m_arrDSState{}
    , m_arrBSState{}
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
        MessageBox(nullptr, L"Device, Context ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"����ü�� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    // ��� Ÿ�� ����
    SetRenderTarget();

    // ViewPort ����
    m_ViewPort.TopLeftX = 0.f;
    m_ViewPort.TopLeftY = 0.f;

    Vec2 vWindowResol = CEngine::GetInst()->GetWindowResolution();
    m_ViewPort.Width = vWindowResol.x;
    m_ViewPort.Height = vWindowResol.y;

    m_ViewPort.MinDepth = 0.f;
    m_ViewPort.MaxDepth = 1.f;

    m_Context->RSSetViewports(1, &m_ViewPort);



    // ���÷� ����
    if (FAILED(CreateSampler()))
    {
        MessageBox(nullptr, L"Sampler ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    //�����Ͷ����� ������Ʈ ����
    if (FAILED(CreateRasterizeState()))
    {
        MessageBoxW(nullptr, L"Rasterizer State ���� ����", L"Rasterizer �ʱ�ȭ ����", MB_OK);
    }

    if (FAILED(CreateDepthStencilState()))
    {
        MessageBoxW(nullptr, L"Depth-Stencil State ���� ����", L"Depth-Stencil State �ʱ�ȭ ����", MB_OK);
    }

    if (FAILED(CreateBlendState()))
    {
        MessageBoxW(nullptr, L"Blend State ���� ����", L"Blend State �ʱ�ȭ ����", MB_OK);
    }

    // ������� ����
    CreateConstBuffer();


    return S_OK; // E_FAIL;
}

void CDevice::SetRenderTarget()
{
    // ��� Ÿ�� ����
    m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());
}

int CDevice::CreateSwapChain()
{
    // ����ü�� ����
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    tDesc.OutputWindow = m_hWnd;    // ��� ������
    tDesc.Windowed = true;          // â���, ��üȭ�� ���

    tDesc.BufferCount = 2;              
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tDesc.BufferDesc.RefreshRate.Denominator = 1;
    tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    
    tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    tDesc.Flags = 0;
    

    // ����ü�� ����
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

    // DepthStencil �뵵 �ؽ��� ����
    D3D11_TEXTURE2D_DESC tDesc = {};

    tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // �ݵ�� ����Ÿ�ٰ� ���� �ػ󵵷� �����ؾ� ��
    tDesc.Width = (UINT)m_vRenderResolution.x;
    tDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.ArraySize = 1;

    tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;    
    tDesc.Usage = D3D11_USAGE_DEFAULT;
    tDesc.CPUAccessFlags = 0;

    tDesc.MipLevels = 1;    // ������ ����

    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    

    if (FAILED(m_Device->CreateTexture2D(&tDesc, nullptr, m_DSTex.GetAddressOf())))
    {
        return E_FAIL;
    }

    // DepthStencilView ����
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
    //Vertex Shader���� ������۸� ���� + Light ó���� ���ؼ� �ȼ� ���̴����� ���� �����Ѵ�.
    UINT8 CBufferTarget = eSHADER_PIPELINE_STAGE_FLAG::__ALL;
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::TRANSFORM] = new CConstBuffer((UINT)eCONST_BUFFER_TYPE::TRANSFORM);
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::TRANSFORM]->SetPipelineTarget(CBufferTarget);

    //Vertex + Pixel Shader���� ������۸� ����
    CBufferTarget |= eSHADER_PIPELINE_STAGE_FLAG::__PIXEL;
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::MATERIAL] = new CConstBuffer((UINT)eCONST_BUFFER_TYPE::MATERIAL);
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::MATERIAL]->Create(sizeof(tMtrlData), 1);
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::MATERIAL]->SetPipelineTarget(CBufferTarget);

    //CBufferTarget = eSHADER_PIPELINE_STAGE_FLAG::__VERTEX | eSHADER_PIPELINE_STAGE_FLAG::__PIXEL;
    //m_arrConstBuffer[eCONST_BUFFER_DEBUGSHAPE] = new CConstBuffer(eCONST_BUFFER_DEBUGSHAPE);
    //m_arrConstBuffer[eCONST_BUFFER_DEBUGSHAPE]->SetPipelineTarget(CBufferTarget);
    //m_arrConstBuffer[eCONST_BUFFER_DEBUGSHAPE]->Create(sizeof(tDebugShapeInfo), 1);

    //�۷ι� �����ʹ� ��� ���̴� ���������ο��� ������ �� �ֵ��� ����
    CBufferTarget = eSHADER_PIPELINE_STAGE_FLAG::__ALL;
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::GLOBAL] = new CConstBuffer((UINT)eCONST_BUFFER_TYPE::GLOBAL);
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::GLOBAL]->Create(sizeof(tGlobalValue), 1);
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::GLOBAL]->SetPipelineTarget(CBufferTarget);

    //����ȭ ������ ���� �ڿ��� ������ ��� ����(ex. ��ϵ� ����ȭ ������ count)
    CBufferTarget = eSHADER_PIPELINE_STAGE_FLAG::__ALL;
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::SBUFFER_SHAREDINFO] = new CConstBuffer((UINT)eCONST_BUFFER_TYPE::SBUFFER_SHAREDINFO);
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::SBUFFER_SHAREDINFO]->Create(sizeof(tSBufferInfo), (UINT)eSBUFFER_SHARED_CBUFFER_IDX::END);
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::SBUFFER_SHAREDINFO]->SetPipelineTarget(CBufferTarget);

    
    //��ƼŬ ��� �����͸� ������ ��� ����
    CBufferTarget = eSHADER_PIPELINE_STAGE_FLAG::__COMPUTE;
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::PARTICLE_MODULEDATA] = new CConstBuffer((UINT)eCONST_BUFFER_TYPE::PARTICLE_MODULEDATA);

    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::PARTICLE_MODULEDATA]->Create(sizeof(tParticleModule), 1);
    m_arrConstBuffer[(UINT)eCONST_BUFFER_TYPE::PARTICLE_MODULEDATA]->SetPipelineTarget(CBufferTarget);
}


HRESULT CDevice::CreateRasterizeState()
{
    D3D11_RASTERIZER_DESC Desc = {};

    //1. �⺻���� nullptr�� �������ش�.(�⺻������ ���� �������ʿ�X)
    m_arrRSState[(UINT)eRASTERIZER_TYPE::CULL_BACK] = nullptr;

    //2. ����Ʈ���̽� �ø�
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;

    HRESULT Result = S_OK;
    Result = DEVICE->CreateRasterizerState(&Desc, &m_arrRSState[(UINT)eRASTERIZER_TYPE::CULL_FRONT]);

    //3. �ø� ���� ����
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    Result = DEVICE->CreateRasterizerState(&Desc, &m_arrRSState[(UINT)eRASTERIZER_TYPE::CULL_NONE]);

    //3.���̾�������
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    Result = DEVICE->CreateRasterizerState(&Desc, &m_arrRSState[(UINT)eRASTERIZER_TYPE::WIRE_FRAME]);


    return Result;
}

HRESULT CDevice::CreateDepthStencilState()
{
    HRESULT Result = S_OK;

    for (UINT i = 0; i < (UINT)eDEPTHSTENCIL_TYPE::END; ++i)
    {
        D3D11_DEPTH_STENCIL_DESC Desc = {};

        switch ((eDEPTHSTENCIL_TYPE)i)
        {
        case eDEPTHSTENCIL_TYPE::LESS:

            //�̰� �⺻���̹Ƿ� nullptr�� �ش�.
            m_arrDSState[i] = nullptr;
            continue;
            break;

        case eDEPTHSTENCIL_TYPE::LESS_EQUAL:
            Desc.DepthEnable = true;
            Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
            Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            Desc.StencilEnable = false;
            break;

        case eDEPTHSTENCIL_TYPE::GREATER:
            Desc.DepthEnable = true;
            Desc.DepthFunc = D3D11_COMPARISON_GREATER;
            Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            Desc.StencilEnable = false;
            break;

        case eDEPTHSTENCIL_TYPE::GREATER_EQUAL:
            Desc.DepthEnable = true;
            Desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
            Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            Desc.StencilEnable = false;
            break;

        case eDEPTHSTENCIL_TYPE::NO_WRITE:
            Desc.DepthEnable = true;
            Desc.DepthFunc = D3D11_COMPARISON_LESS;
            Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
            Desc.StencilEnable = false;
            break;

        case eDEPTHSTENCIL_TYPE::NO_TEST_NO_WRITE:
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

HRESULT CDevice::CreateBlendState()
{
    HRESULT Result = S_OK;

    for (int i = 0; i < (UINT)eBLENDSTATE_TYPE::END; ++i)
    {
        D3D11_BLEND_DESC Desc = {};

        switch ((eBLENDSTATE_TYPE)i)
        {
        case eBLENDSTATE_TYPE::DEFAULT:
            m_arrBSState[i] = nullptr;
            continue;
            break;

        case eBLENDSTATE_TYPE::MASK:
            //Alpha-To-Coverage ����� Ȱ��ȭ�Ѵ�.
            //Alpha-To-Coverage ����� 
            Desc.AlphaToCoverageEnable = true;
            
            //�Ʒ� �ɼ��� �� RenderTarget ���� �ٸ� �ɼ��� ����� ������ ���θ� �����Ѵ�.
            //������� ������ 8���� �� Render Target ���� �ٸ� ���ĺ��� �ɼ��� ������ �� ������(Desc.RenderTarget[index])
            //������� �������� �������� �� Desc.RenderTarget[0]�� �ɼǸ� ���ȴ�. �������� ���� ���õȴ�.
            Desc.IndependentBlendEnable = false;
            Desc.RenderTarget[0].BlendEnable = true;

            //ADD = �Ʒ� ������ ������� ���İ��� �ݿ��� ����(Dest) ����� �Է�(Src) ������ ���� ���Ѵ�.
            //Mask�� ��� Alpha-To-Coverage �ɼ��� ����ϹǷ� ����� ���� ������ 1�� ���ش�.
            Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
            
            Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

            //� ������ ���������� �����Ѵ�.
            //RGB �� Ư�� ���� ����ֵ��� �� ���� ������, ���̴� �ڵ�ε� ������ �۾��̰�, ������ Blend State �ܰ迡�� ó���ϴ� ���� �� �����ϹǷ� �׳� ���� ALL�� �� �ִ� ���� ����.
            Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            break;

        case eBLENDSTATE_TYPE::ALPHA_BLEND:
            Desc.AlphaToCoverageEnable = false;
            Desc.IndependentBlendEnable = false;
            Desc.RenderTarget[0].BlendEnable = true;

            //ADD = ����(Dest) ����� �Է�(Src) ������ ���� ���Ѵ�.
            //Dest�� ������ ������Ʈ �����̰� Src�� ������ ������Ʈ �����̶�� ���� ����.
            //������ ������Ʈ�� �̹� ������ ĥ���� �ȼ��̰�, �׷��� �� ���İ��� ����Ǿ� ��ĥ�Ǿ��� ���̴�.
            //������ ������Ʈ�� ���� ������� �����ε�, ���� ������Ʈ�� ������ �󸶳Ŀ� ���� �����ؼ� ����Ǿ�� �� ���̴�.
            //�׷��Ƿ� Src �ȼ��� alpha ����ŭ �����ϰ�(���ϰ�) �׷��ְ�, Dest �ȼ��� Src�� Alpha������ ���� �� ��ŭ ĥ�� �ָ�
            //�������ϴٴ� ������ �� �� �ְ� �ȴ�.
            Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;


            //��� ������ ������ ���Ŀ��� ���İ��� �ʿ䰡 ����. 
            //������ �����Ǿ��ٴ� �׷����ٴ� ���ε�, �̰� ���� Dest �ȼ��� �Ǵ� ���̰�,
            //���� ������ ���� �׷��� �� Src�� ���İ��� ����Ѵٴ� ����� �� �� �ִ�.
            //�׷��Ƿ� Src�� ���İ��� �״�� �������ش�.
            Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

            Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

            break;
        case eBLENDSTATE_TYPE::ONE_ONE: 
            Desc.AlphaToCoverageEnable = true;
            Desc.IndependentBlendEnable = false;
            Desc.RenderTarget[0].BlendEnable = true;

            //ADD = ����(Dest) ����� �Է�(Src) ������ ���� ���Ѵ�.

            Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

            Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

            //� ������ ���������� �����Ѵ�.
            //RGB �� Ư�� ���� ����ֵ��� �� ���� ������, ���̴� �ڵ�� ������ �۾��̰�, ������ Blend State �ܰ迡�� ó���ϴ� ���� �� �����ϹǷ� �׳� ���� ALL�� �� �ִ� ���� ����.
            Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            break;

        default:
            break;
        }

        Result = DEVICE->CreateBlendState(&Desc, &m_arrBSState[i]);
    }


    return Result;
}
