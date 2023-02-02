#include "pch.h"
#include "CStructBuffer.h"

#include "CConstBuffer.h"

#include "CDevice.h"


CStructBuffer::CStructBuffer(eSTRUCT_BUFFER_TYPE _Type, eSBUFFER_SHARED_CBUFFER_IDX _idx, eUAV_REGISTER_USAGE _UAVUsage)
	: m_eSBufferType(_Type)
	, m_eCBufferIdx(_idx)
	, m_uElementSize()
	, m_uElementCapacity()
	, m_flagPipelineTarget()
	, m_uRegisterIdx()
	, m_BufferDesc{}
	, m_bUAVBind()
{
	switch (m_eSBufferType)
	{
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:

		//CPU���� ����ȭ���ۿ� �ۼ��� ���̹Ƿ� AccessFlag�� write�� ����
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		//CPU���� ���۸� �ۼ��ϰ� GPU���� ���۸� �о���� ��� ���
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		//SRV�� ���ε��ǵ��� ����
		m_BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		break;
	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
		

		//CPU�� ���� �Ұ�, GPU�� �б�/����� ����
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;

		//SRV, UAV �� �� ���ε��ǵ��� ����
		m_BufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

		break;
	default:
		break;
	}

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
	m_uElementSize = _uiElementSize;
	m_uElementCapacity = _uElementCapacity;

	m_BufferDesc.StructureByteStride = m_uElementSize;
	m_BufferDesc.ByteWidth = m_uElementSize * m_uElementCapacity;

	switch (m_eSBufferType)
	{
	//�Ϲ����� GPU���� �б⸸ ������ ����ȭ ����
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		//����ȭ���� ����
		if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, nullptr, m_StructBuffer.ReleaseAndGetAddressOf())))
			assert(nullptr);

		CreateSRV();

		break;
	}
	
	//Compute Shader�� �����ؼ� ����ϴ�, GPU���� ���� ������ SBuffer
	//�� ��쿡�� CPU�� ������ ������ �Ұ����ϹǷ� ������ ���ۿ� Staging Buffer�� �߰� �ܰ�� ����ؾ� �Ѵ�.
	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
	{
		//����ȭ���� ����
		if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, nullptr, m_StructBuffer.ReleaseAndGetAddressOf())))
			assert(nullptr);

		D3D11_BUFFER_DESC Desc = m_BufferDesc;
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		Desc.Usage = D3D11_USAGE_STAGING;
		Desc.BindFlags = 0;
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		if (FAILED(DEVICE->CreateBuffer(&Desc, nullptr, m_StagingBuffer.ReleaseAndGetAddressOf())))
			assert(nullptr);

		CreateSRV();
		CreateUAV();

		break;
	}
		
	}
}

void CStructBuffer::UpdateData(void* _pData, UINT _uiCount)
{

	//���� �� �Ҵ�� �������� ���� ������ �� Ŭ ��� ���Ҵ��ϰ�, �ű⿡ �����͸� �߰�.
	if (_uiCount > m_uElementCapacity)
		Create(m_uElementSize, _uiCount * 2u);

	CDevice* pDevice = CDevice::GetInst();

	switch (m_eSBufferType)
	{
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		//Read Only�� ��� SBuffer�� �ٷ� Map/UnMap�� ���ָ� �ȴ�.
		D3D11_MAPPED_SUBRESOURCE Data = {};
		pDevice->GetDeviceContext()->Map(m_StructBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);

		memcpy(Data.pData, _pData, (size_t)(m_uElementSize * _uiCount));

		pDevice->GetDeviceContext()->Unmap(m_StructBuffer.Get(), 0);

		break;
	}

	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
	{
		//Read+Write�� ��� �߰� ���۷� ������ �� GPU�� ���۷� �����Ѵ�.

		D3D11_MAPPED_SUBRESOURCE Data = {};
		pDevice->GetDeviceContext()->Map(m_StagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &Data);

		memcpy(Data.pData, _pData, (size_t)(m_uElementSize * _uiCount));

		pDevice->GetDeviceContext()->Unmap(m_StagingBuffer.Get(), 0);

		pDevice->GetDeviceContext()->CopyResource(m_StructBuffer.Get(), m_StagingBuffer.Get());

		break;
	}

	}

	//g_arrStructBufferInfo�� �ڽ��� �ε����� �ش��ϴ� ��ġ�� �̹��� ������Ʈ�� ����ü�� ������ ����
	g_arrStructBufferInfo[(UINT)m_eCBufferIdx].uSBufferCount = _uiCount;
}

void CStructBuffer::GetData(void* _pDest)
{
	ComPtr<ID3D11DeviceContext> pContext = CONTEXT;

	switch (m_eSBufferType)
	{

	//�б� ���� Struct ������ ��쿡�� �׳� �ٷ� �����͸� �������� �ȴ�.
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		D3D11_MAPPED_SUBRESOURCE Data = {};

		pContext->Map(m_StructBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

		size_t bytesize = m_uElementSize * m_uElementCapacity;
		memcpy_s(_pDest, bytesize, Data.pData, bytesize);

		pContext->Unmap(m_StructBuffer.Get(), 0);

		break;
	}
	
	//�б�/���� ��� ������ Struct ������ ��쿡�� Staging ���۸� ���ؼ� �����´�.
	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
	{
		pContext->CopyResource(m_StagingBuffer.Get(), m_StructBuffer.Get());

		D3D11_MAPPED_SUBRESOURCE Data = {};
		pContext->Map(m_StagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

		size_t bytesize = m_uElementSize * m_uElementCapacity;
		memcpy_s(_pDest, bytesize, Data.pData, bytesize);

		pContext->Unmap(m_StagingBuffer.Get(), 0);

		break;
	}
	
	default: break;
	}
}

void CStructBuffer::BindData()
{
	//UAV�� ���ε� �Ǿ����� ��� ���ε��� ����
	if (true == m_bUAVBind)
	{
		static const UINT Offset = -1;

		ComPtr<ID3D11UnorderedAccessView> pUAV = nullptr;
		CONTEXT->CSSetUnorderedAccessViews(m_uRegisterIdx, 1, pUAV.GetAddressOf(), &Offset);
		m_bUAVBind = false;
	}

	UpdateConstBuffer();

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

void CStructBuffer::BindData_CS(int TargetReg)
{
	int target = m_uRegisterIdx;
	if (-1 != TargetReg)
		target = TargetReg;

	UpdateConstBuffer();

	static const UINT Offset = -1;
	CONTEXT->CSSetUnorderedAccessViews(TargetReg, 1, m_UAV.GetAddressOf(), &Offset);

	m_bUAVBind = true;
}

void CStructBuffer::UpdateConstBuffer()
{
	//����ü ������ ���� ������ۿ� ���ε��� ����ü ������ �־ ����
	CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_SBUFFERINFO);
	pConstBuffer->UpdateData(g_arrStructBufferInfo);
	pConstBuffer->SetPipelineTarget(m_flagPipelineTarget);
	pConstBuffer->BindData();
}

void CStructBuffer::CreateSRV()
{
	//GPU������ �б� �۾��� �� ���̹Ƿ� SRV�� ����.
	//SRVDesc�� ������ ������ �����ؼ� ����.
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.NumElements = m_uElementCapacity;

	if (FAILED(DEVICE->CreateShaderResourceView(m_StructBuffer.Get(), &SRVDesc, m_SRV.ReleaseAndGetAddressOf())))
		assert(nullptr);
}

void CStructBuffer::CreateUAV()
{
	//GPU���� �б� �� ���� �۾��� �����ؾ� �ϹǷ� UAV ���·� ����
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.NumElements = m_uElementCapacity;

	if (FAILED(DEVICE->CreateUnorderedAccessView(m_StructBuffer.Get(), &UAVDesc, m_UAV.ReleaseAndGetAddressOf())))
		assert(nullptr);
}
