#pragma once

#include "global.h"

class CConstBuffer;

class CDevice
	: public CSingleton<CDevice>
{
	SINGLETON(CDevice);
private:
	//초기화 순서 + 렌더링 파이프라인 순서대로 정렬
	//Window
	HWND							m_hWnd;

	//렌더타겟 해상도 -> GlobalVal로 옮김.


	//Device-Context
	ComPtr<ID3D11Device>			m_Device;		// GPU 메모리 할당 	
	ComPtr<ID3D11DeviceContext>		m_Context;		// GPU 제어, 렌더링, 동작 수행

	//SwapChain, Render Target

	ComPtr<IDXGISwapChain>			m_SwapChain;
	ComPtr<ID3D11Texture2D>			m_RTTex;
	ComPtr<ID3D11RenderTargetView>	m_RTV;

	D3D11_VIEWPORT					m_ViewPort;


	//Constant Buffer
	CConstBuffer* m_arrConstBuffer[idx_b_END];


	//Depth Stencil View
	ComPtr<ID3D11Texture2D>			m_DSTex;
	ComPtr<ID3D11DepthStencilView>	m_DSV;


	// Sampler
	ComPtr<ID3D11SamplerState>		m_Sampler[2];

	// Rasterizer
	ComPtr<ID3D11RasterizerState>	m_arrRSState[(UINT)define_Shader::eRASTERIZER_TYPE::END];

	//Output Merger
	////Depth Stencil State
	ComPtr<ID3D11DepthStencilState>	m_arrDSState[(UINT)define_Shader::eDEPTH_STENCIL_TYPE::END];

	////Blend State
	ComPtr<ID3D11BlendState>		m_arrBSState[(UINT)define_Shader::eBLEND_STATE_TYPE::END];



	// ID3D11RenderTargetView
	// ID3D11DepthStencilView
	// ID3D11ShaderResourceView
	// ID3D11UnorderedAccessView



public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void SetRenderTarget();
	void ClearTarget(float(&_color)[4]);
	void present()	{ m_SwapChain->Present(1, 0); }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateSampler();
	void CreateConstBuffer();
	HRESULT CreateRasterizeState();
	HRESULT CreateDepthStencilState();
	HRESULT CreateBlendState();

public:
	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_Context.Get(); }
	CConstBuffer* GetConstBuffer(int _idx_b_RegisterIdx) { return m_arrConstBuffer[_idx_b_RegisterIdx]; }
	ID3D11RasterizerState* GetRSState(define_Shader::eRASTERIZER_TYPE _Type) { return m_arrRSState[(UINT)_Type].Get(); }
	ID3D11DepthStencilState* GetDSState(define_Shader::eDEPTH_STENCIL_TYPE _Type) { return m_arrDSState[(UINT)_Type].Get(); }
	ID3D11BlendState* GetBSState(define_Shader::eBLEND_STATE_TYPE _Type) { return m_arrBSState[(UINT)_Type].Get(); }
};

