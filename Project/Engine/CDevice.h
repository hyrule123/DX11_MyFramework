#pragma once

class CConstBuffer;

class CDevice
	: public CSingleton<CDevice>
{
private:
	HWND							m_hWnd;

	ComPtr<ID3D11Device>			m_Device;		// GPU �޸� �Ҵ� 	
	ComPtr<ID3D11DeviceContext>		m_Context;		// GPU ����, ������, ���� ����

	ComPtr<IDXGISwapChain>			m_SwapChain;

	ComPtr<ID3D11Texture2D>			m_RTTex;
	ComPtr<ID3D11RenderTargetView>	m_RTV;

	ComPtr<ID3D11Texture2D>			m_DSTex;
	ComPtr<ID3D11DepthStencilView>	m_DSV;

	// Sampler
	ComPtr<ID3D11SamplerState>		m_Sampler[2];


	D3D11_VIEWPORT					m_ViewPort;

	// ����Ÿ�� �ػ�
	Vec2							m_vRenderResolution;



						
	CConstBuffer*					m_arrConstBuffer[(UINT)CB_TYPE::END];


	// ID3D11RenderTargetView
	// ID3D11DepthStencilView
	// ID3D11ShaderResourceView
	// ID3D11UnorderedAccessView



public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void ClearTarget(float(&_color)[4]);
	void Present()	{ m_SwapChain->Present(0, 0); }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateSampler();
	void CreateConstBuffer();

public:
	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_Context.Get(); }
	CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_arrConstBuffer[(UINT)_Type]; }

public:
	CDevice();
	~CDevice();
};

