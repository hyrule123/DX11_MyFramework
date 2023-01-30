#include "pch.h"
#include "CStructBuffer.h"

#include "CConstBuffer.h"

#include "CDevice.h"


CStructBuffer::CStructBuffer(eSBUFFER_SHARED_CBUFFER_IDX _idx)
	: m_eCBufferIdx(_idx)
	, m_uElementSize()
	, m_uElementCapacity()
	, m_flagPipelineTarget()
	, m_uRegisterIdx()
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

void CStructBuffer::Create(UINT _uiElementSize, UINT _uElementCapacity)
{
	//������ۿ� ���������� 16����Ʈ ������ ���ĵǾ� �־�� ��.
	assert(0 == _uiElementSize % 16);

	//������ۿʹ� �ٸ��� ���� ���Ҵ��� ������. ���� ���� ������ �Ҵ��� �����Ѵ�.(ComPtr�� ���� ������ �̷�����Ƿ� nullptr�� �ٲ��ָ� ��.)
	m_TempDeleteSB = m_StructBuffer;
	m_StructBuffer = nullptr;
	m_TempDeleteSRV = m_SRV;
	m_SRV = nullptr;

	m_uElementSize = _uiElementSize;
	m_uElementCapacity = _uElementCapacity;

	m_BufferDesc.StructureByteStride = m_uElementSize;
	m_BufferDesc.ByteWidth = m_uElementSize * m_uElementCapacity;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, nullptr, m_StructBuffer.GetAddressOf())))
		assert(nullptr);


	//SRVDesc�� ������ ������ �����ؼ� ����.
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.NumElements = m_uElementCapacity;

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
	if (_uiCount > m_uElementCapacity)
		Create(m_uElementSize, _uiCount * 2u);

	CDevice* pDevice = CDevice::GetInst();

	D3D11_MAPPED_SUBRESOURCE Data = {};
	pDevice->GetDeviceContext()->Map(m_StructBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
	
	memcpy(Data.pData, _pData, (size_t)(m_uElementSize * _uiCount));

	pDevice->GetDeviceContext()->Unmap(m_StructBuffer.Get(), 0);

	//g_arrStructBufferInfo�� �ڽ��� �ε����� �ش��ϴ� ��ġ�� �̹��� ������Ʈ�� ����ü�� ������ ����
	g_arrStructBufferInfo[(UINT)m_eCBufferIdx].uSBufferCount = _uiCount;
}

void CStructBuffer::UpdateData()
{
	//����ü ������ ���� ������ۿ� ���ε��� ����ü ������ �־ ����
	CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_SBUFFERINFO);
	pConstBuffer->SetData(g_arrStructBufferInfo);
	pConstBuffer->SetPipelineTarget(m_flagPipelineTarget);
	pConstBuffer->UpdateData();

	if (eSHADER_PIPELINE_FLAG_VERTEX & m_flagPipelineTarget)
	{
		CONTEXT->VSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_HULL & m_flagPipelineTarget)
	{
		CONTEXT->HSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_DOMAIN & m_flagPipelineTarget)
	{
		CONTEXT->DSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_GEOMETRY & m_flagPipelineTarget)
	{
		CONTEXT->GSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_FLAG_PIXEL & m_flagPipelineTarget)
	{
		CONTEXT->PSSetShaderResources(m_uRegisterIdx, 1, m_SRV.GetAddressOf());
	}
}
