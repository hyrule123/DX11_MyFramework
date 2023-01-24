#include "pch.h"
#include "CStructBuffer.h"

#include "CDevice.h"


CStructBuffer::CStructBuffer()
	: m_uiElementSize()
	, m_uiElementCount()
	, m_ePIPELINE_STAGE_flags()
	, m_uiRegisterIdx()
	, m_BufferDesc{}
{
	//CPU���� ���۸� �ۼ��ϰ� GPU���� ���۸� �о���� ��� ���
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	//���̴� ���ҽ� �Ӽ��� ������
	m_BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	//CPU���� ����ȭ���ۿ� �ۼ��� ���̹Ƿ� AccessFlag�� write�� ����
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//��Ÿ �÷��׿� ����ȭ ���۷� �������� ����.
	m_BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
}

CStructBuffer::~CStructBuffer()
{
}

void CStructBuffer::Create(UINT _uiElementSize, UINT _uiElementCount)
{
	//������ۿ� ���������� 16����Ʈ ������ ���ĵǾ� �־�� ��.
	assert(0 == _uiElementSize % 16);

	//������ۿʹ� �ٸ��� ���� ���Ҵ��� ������. ���� ���� ������ �Ҵ��� �����Ѵ�.(ComPtr�� ���� ������ �̷�����Ƿ� nullptr�� �ٲ��ָ� ��.)
	m_TempDeleteSB = m_StructBuffer;
	m_StructBuffer = nullptr;
	m_TempDeleteSRV = m_SRV;
	m_SRV = nullptr;

	m_uiElementSize = _uiElementSize;
	m_uiElementCount = _uiElementCount;

	m_BufferDesc.StructureByteStride = m_uiElementSize;
	m_BufferDesc.ByteWidth = m_uiElementSize * m_uiElementCount;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, nullptr, m_StructBuffer.GetAddressOf())))
		assert(nullptr);


	//SRVDesc�� ������ ������ �����ؼ� ����.
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.NumElements = m_uiElementCount;

	if (FAILED(DEVICE->CreateShaderResourceView(m_StructBuffer.Get(), &SRVDesc, m_SRV.GetAddressOf())))
		assert(nullptr);
}

void CStructBuffer::SetData(void* _pData, UINT _uiCount)
{
	//���� ��� ������ ����ȭ ���۰� ���� ��� nullptr�� �����ؼ� �ϳ��� ����(���Ҵ�� ���� ���̿� ���� �༭ �� ����)
	if (nullptr != m_TempDeleteSB)
		m_TempDeleteSB = nullptr;
	else if (nullptr != m_TempDeleteSRV)
		m_TempDeleteSRV = nullptr;

	//���� �� �Ҵ�� �������� ���� ������ �� Ŭ ��� ���Ҵ��ϰ�, �ű⿡ �����͸� �߰�.
	if (_uiCount > m_uiElementCount)
		Create(m_uiElementSize, _uiCount * 2u);

	D3D11_MAPPED_SUBRESOURCE Data = {};
	CONTEXT->Map(m_StructBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
	
	memcpy(Data.pData, _pData, (size_t)(m_uiElementSize * _uiCount));

	CONTEXT->Unmap(m_StructBuffer.Get(), 0);
}

void CStructBuffer::UpdateData()
{
	if (eSHADER_PIPELINE_FLAG_VERTEX & m_ePIPELINE_STAGE_flags)
	{
		CONTEXT->VSSetShaderResources(m_uiRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_HULL & m_ePIPELINE_STAGE_flags)
	{
		CONTEXT->HSSetShaderResources(m_uiRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_DOMAIN & m_ePIPELINE_STAGE_flags)
	{
		CONTEXT->DSSetShaderResources(m_uiRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_GEOMETRY & m_ePIPELINE_STAGE_flags)
	{
		CONTEXT->GSSetShaderResources(m_uiRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_PIXEL & m_ePIPELINE_STAGE_flags)
	{
		CONTEXT->PSSetShaderResources(m_uiRegisterIdx, 1, m_SRV.GetAddressOf());
	}
}
