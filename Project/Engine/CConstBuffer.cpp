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

	// 16����Ʈ ���� �޸� ������ �Ǿ����� ���� ��� ������ �߻���Ų��.
	assert(!(iBufferSize % 16));	

	// ������� ����
	m_Desc.ByteWidth = iBufferSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void CConstBuffer::UploadData(void* _pSrc, UINT _iSize)
{
	// ũ�Ⱑ �������� ���� �����ʹ� ������� ũ��� ����.
	UINT size = _iSize;
	if (0 == _iSize)
	{
		size = m_iElementSize* m_iElementCount;
	}

	// ������� ũ�⺸�� �� ū �����Ͱ� �Է����� ���� ���
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

void CConstBuffer::BindBuffer(eSHADER_PIPELINE_STAGE::FLAG _Stage)
{
	UINT8 Flag = eSHADER_PIPELINE_STAGE::__NONE == _Stage ? m_ePIPELINE_STAGE_flags : _Stage;

	ID3D11DeviceContext* pContext = CONTEXT;
	if (Flag & eSHADER_PIPELINE_STAGE::__VERTEX)
	{
		pContext->VSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (Flag & eSHADER_PIPELINE_STAGE::__HULL)
	{
		pContext->HSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (Flag & eSHADER_PIPELINE_STAGE::__DOMAIN)
	{
		pContext->DSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (Flag & eSHADER_PIPELINE_STAGE::__GEOMETRY)
	{
		pContext->GSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}	
	if (Flag & eSHADER_PIPELINE_STAGE::__PIXEL)
	{
		pContext->PSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}
	if (Flag & eSHADER_PIPELINE_STAGE::__COMPUTE)
	{
		pContext->CSSetConstantBuffers(m_iRegisterNum, 1, m_CB.GetAddressOf());
	}
}

