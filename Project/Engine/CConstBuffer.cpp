#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CConstBuffer::CConstBuffer(UINT _iRegisterNum)
	: m_Desc{}
	, m_iRegisterNum(_iRegisterNum)
	, m_iElementSize(0)
	, m_iElementCount(0)
	, m_ePIPELINE_STAGE_flags(UINT8_MAX)
{
}

CConstBuffer::~CConstBuffer()
{
}

void CConstBuffer::Create(UINT _iElementSize, UINT _iElementCount)
{
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;

	UINT iBufferSize = m_iElementSize * _iElementCount;

	// 16바이트 단위 메모리 정렬이 되어있지 않을 경우 에러를 발생시킨다.
	assert(!(iBufferSize % 16));	

	// 상수버퍼 생성
	m_Desc.ByteWidth = iBufferSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void CConstBuffer::UploadData(const void* _pSrc, UINT _iSize)
{
	// 크기가 지정되지 않은 데이터는 상수버퍼 크기로 본다.
	UINT size = _iSize;
	if (0 == _iSize)
	{
		size = m_iElementSize * m_iElementCount;
	}

	// 상수버퍼 크기보다 더 큰 데이터가 입력으로 들어온 경우
	assert(!(size > m_iElementSize * m_iElementCount));

	// SysMem -> GPU Mem

	ID3D11DeviceContext* pContext = CONTEXT;
	D3D11_MAPPED_SUBRESOURCE tSubRes = {};
	if (SUCCEEDED(pContext->Map(m_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSubRes)))
	{
		memcpy(tSubRes.pData, _pSrc, size);
		pContext->Unmap(m_CB.Get(), 0);
	}
}

void CConstBuffer::BindBuffer(UINT _eSHADER_PIPELINE_FLAG)
{
	//인자로 NONE이 아닌 플래그값이 들어왔을 경우 해당 플래그값을 타겟으로 바인딩한다.
	if (define_Shader::eSHADER_PIPELINE_STAGE::__NONE == _eSHADER_PIPELINE_FLAG)
		_eSHADER_PIPELINE_FLAG = m_ePIPELINE_STAGE_flags;


	ID3D11DeviceContext* pContext = CONTEXT;
	if (_eSHADER_PIPELINE_FLAG & define_Shader::eSHADER_PIPELINE_STAGE::__VERTEX)
	{
		pContext->VSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (_eSHADER_PIPELINE_FLAG & define_Shader::eSHADER_PIPELINE_STAGE::__HULL)
	{
		pContext->HSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (_eSHADER_PIPELINE_FLAG & define_Shader::eSHADER_PIPELINE_STAGE::__DOMAIN)
	{
		pContext->DSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (_eSHADER_PIPELINE_FLAG & define_Shader::eSHADER_PIPELINE_STAGE::__GEOMETRY)
	{
		pContext->GSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (_eSHADER_PIPELINE_FLAG & define_Shader::eSHADER_PIPELINE_STAGE::__PIXEL)
	{
		pContext->PSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}
	if (_eSHADER_PIPELINE_FLAG & define_Shader::eSHADER_PIPELINE_STAGE::__COMPUTE)
	{
		pContext->CSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}
}

