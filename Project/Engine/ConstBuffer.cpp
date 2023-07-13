#include "pch.h"
#include "cConstBuffer.h"

#include "Device.h"

cConstBuffer::cConstBuffer()
	: m_Desc{}
	, m_CBufferClassDesc{}
{
}

cConstBuffer::~cConstBuffer()
{
}

HRESULT cConstBuffer::Create(tConstBufferClassDesc const& _tDesc)
{

	if (_tDesc.iRegisterNum < 0)
		return E_FAIL;

	UINT uBufferTotalSize = _tDesc.uElementSize * _tDesc.uElementCount;
	if (0u == uBufferTotalSize)
		return E_FAIL;

	m_CBufferClassDesc = _tDesc;


	// 16바이트 단위 메모리 정렬이 되어있지 않을 경우 에러를 발생시킨다.
	assert(!(uBufferTotalSize % 16));

	// 상수버퍼 생성
	m_Desc.ByteWidth = uBufferTotalSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf());
	if (FAILED(hr))
	{
		assert(SUCCEEDED(hr));
		return hr;
	}

	return hr;
}

void cConstBuffer::UploadData(const void* _pSrc, UINT _iSize)
{
	// 크기가 지정되지 않은 데이터는 상수버퍼 크기로 본다.
	UINT size = _iSize;
	if (0 == _iSize)
	{
		size = m_CBufferClassDesc.uElementSize * m_CBufferClassDesc.uElementCount;
	}

	// 상수버퍼 크기보다 더 큰 데이터가 입력으로 들어온 경우
	assert(!(size > m_CBufferClassDesc.uElementSize * m_CBufferClassDesc.uElementCount));

	// SysMem -> GPU Mem

	ID3D11DeviceContext* pContext = CONTEXT;
	D3D11_MAPPED_SUBRESOURCE tSubRes = {};
	if (SUCCEEDED(pContext->Map(m_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSubRes)))
	{
		memcpy(tSubRes.pData, _pSrc, size);
		pContext->Unmap(m_CB.Get(), 0);
	}
}

void cConstBuffer::BindBuffer(UINT _eSHADER_PIPELINE_FLAG)
{
	//인자로 NONE이 아닌 플래그값이 들어왔을 경우 해당 플래그값을 타겟으로 바인딩한다.
	if (define_Shader::ePIPELINE_STAGE_FLAG::__NONE == _eSHADER_PIPELINE_FLAG)
		_eSHADER_PIPELINE_FLAG = m_CBufferClassDesc.PipelineStageBindFlag;


	ID3D11DeviceContext* pContext = CONTEXT;
	if (_eSHADER_PIPELINE_FLAG & define_Shader::ePIPELINE_STAGE_FLAG::__VERTEX)
	{
		pContext->VSSetConstantBuffers(m_CBufferClassDesc.iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (_eSHADER_PIPELINE_FLAG & define_Shader::ePIPELINE_STAGE_FLAG::__HULL)
	{
		pContext->HSSetConstantBuffers(m_CBufferClassDesc.iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (_eSHADER_PIPELINE_FLAG & define_Shader::ePIPELINE_STAGE_FLAG::__DOMAIN)
	{
		pContext->DSSetConstantBuffers(m_CBufferClassDesc.iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (_eSHADER_PIPELINE_FLAG & define_Shader::ePIPELINE_STAGE_FLAG::__GEOMETRY)
	{
		pContext->GSSetConstantBuffers(m_CBufferClassDesc.iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (_eSHADER_PIPELINE_FLAG & define_Shader::ePIPELINE_STAGE_FLAG::__PIXEL)
	{
		pContext->PSSetConstantBuffers(m_CBufferClassDesc.iRegisterNum, 1, m_CB.GetAddressOf());
	}
	if (_eSHADER_PIPELINE_FLAG & define_Shader::ePIPELINE_STAGE_FLAG::__COMPUTE)
	{
		pContext->CSSetConstantBuffers(m_CBufferClassDesc.iRegisterNum, 1, m_CB.GetAddressOf());
	}
}

