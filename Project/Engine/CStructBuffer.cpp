#include "pch.h"
#include "CStructBuffer.h"

#include "CConstBuffer.h"

#include "CDevice.h"


CStructBuffer::CStructBuffer(eSTRUCT_BUFFER_TYPE _type, UINT8 _eSHADER_PIPELINE_STAGE_FLAG, eSBUFFER_SHARED_CBUFFER_IDX _CBIdx, eSRV_REGISTER_IDX _SRVIdx, eUAV_REGISTER_IDX _UAVIdx)
	: m_eSBufferType(_type)
	, m_flagPipelineTarget(_eSHADER_PIPELINE_STAGE_FLAG)
	, m_eCBufferIdx(_CBIdx)
	, m_eSRVIdx(_SRVIdx)
	, m_eUAVIdx(_UAVIdx)
	, m_uElementStride()
	, m_uElementCapacity()
	, m_BufferDesc{}
	, m_eCurrentBoundView()
{
	switch (m_eSBufferType)
	{
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:

		//CPU���� ����ȭ���۷� �ۼ��� �� �־�� �ϹǷ� AccessFlag�� write�� ����
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		//CPU���� ���۸� �ۼ��ϰ� GPU���� ���۸� �о���� ��� ���
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		//SRV�� ���ε��ǵ��� ����
		m_BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		break;
	case eSTRUCT_BUFFER_TYPE::READ_WRITE:

		//CPU�� ���� �Ұ�, 
		m_BufferDesc.CPUAccessFlags = 0;

		//GPU�� �б�/����� ����
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

void CStructBuffer::Create(UINT _uiElementSize, UINT _uElementCapacity, void* _pInitialData, UINT _uElemCount)
{
	//������ۿ� ���������� 16����Ʈ ������ ���ĵǾ� �־�� ��.
	assert(0 == _uiElementSize % 16);

	assert(_uElementCapacity >= _uElemCount);

	//���Ҵ� �ϱ� �� ���ε��� ���ҽ��� �ִٸ� unbind
	switch (m_eCurrentBoundView)
	{
	case eCURRENT_BOUND_VIEW::SRV:
		UnBindSRV();
		break;
	case eCURRENT_BOUND_VIEW::UAV:
		UnBindUAV();
		break;
	default:
		break;
	}

	//������ۿʹ� �ٸ��� ���� ���Ҵ��� ������. ���� ���� ������ �Ҵ��� �����Ѵ�.(ComPtr�� ���� ������ �̷�����Ƿ� nullptr�� �ٲ��ָ� ��.)
	m_uElementStride = _uiElementSize;
	m_uElementCapacity = _uElementCapacity;

	m_BufferDesc.StructureByteStride = m_uElementStride;
	m_BufferDesc.ByteWidth = m_uElementStride * m_uElementCapacity;

	switch (m_eSBufferType)
	{
	//�Ϲ����� GPU���� �б⸸ ������ ����ȭ ����
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		//�ʱ갪 ���ε� �� ���ε�
		D3D11_SUBRESOURCE_DATA* pData = nullptr;
		D3D11_SUBRESOURCE_DATA Data = {};
		if (nullptr != _pInitialData)
		{
			Data.pSysMem = _pInitialData;
			Data.SysMemPitch = m_uElementStride * _uElemCount;
			Data.SysMemSlicePitch = m_BufferDesc.StructureByteStride;
			pData = &Data;
			m_eCurrentBoundView = eCURRENT_BOUND_VIEW::SRV;
		}
		

		//����ȭ���� ����. ReleaseAndGetAddressOf() �Լ��� ���ؼ� ���� ����ȭ ���۰� �ִٸ� ���������� ����
		if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, pData, m_StructBuffer.ReleaseAndGetAddressOf())))
			assert(nullptr);

		//Read Only�� ��� �ٷ� SRV ����
		CreateSRV();

		break;
	}
	
	//Compute Shader�� �����ؼ� ����ϴ�, GPU���� ���� ������ SBuffer
	//�� ��쿡�� CPU�� ������ ������ �Ұ����ϹǷ� ������ ���ۿ� Staging Buffer�� �߰� �ܰ�� ����ؾ� �Ѵ�.
	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
	{
		//�ʱ갪 ����
		D3D11_SUBRESOURCE_DATA* pData = nullptr;
		D3D11_SUBRESOURCE_DATA Data = {};
		if (nullptr != _pInitialData)
		{
			Data.pSysMem = _pInitialData;
			Data.SysMemPitch = m_uElementStride * _uElemCount;
			Data.SysMemSlicePitch = m_BufferDesc.StructureByteStride;
			pData = &Data;
			m_eCurrentBoundView = eCURRENT_BOUND_VIEW::UAV;
		}

		//����ȭ���� ����
		if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, pData, m_StructBuffer.ReleaseAndGetAddressOf())))
			assert(nullptr);

		CreateStagingBuffer();
		CreateSRV();
		CreateUAV();

		break;
	}
		
	}
}

void CStructBuffer::UploadData(void* _pData, UINT _uCount)
{
	//g_arrStructBufferInfo�� �ڽ��� �ε����� �ش��ϴ� ��ġ�� �̹��� ������Ʈ�� ����ü�� ������ ����
	g_arrStructBufferInfo[(UINT)m_eCBufferIdx].uSBufferCount = _uCount;

	//���� �� �Ҵ�� �������� ���� ������ �� Ŭ ��� ���Ҵ��ϰ�, �ű⿡ �����͸� �߰�.
	//������ �� ���� ������ �� �����Ƿ� �ٷ� return ���ָ� �ɵ�
	if (_uCount > m_uElementCapacity)
	{
		Create(m_uElementStride, g_arrStructBufferInfo[(UINT)m_eCBufferIdx].uSBufferCount, _pData, _uCount);
		return;
	}


	CDevice* pDevice = CDevice::GetInst();
	switch (m_eSBufferType)
	{
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		//Read Only�� ��� SBuffer�� �ٷ� Map/UnMap�� ���ָ� �ȴ�.
		D3D11_MAPPED_SUBRESOURCE Data = {};
		pDevice->GetDeviceContext()->Map(m_StructBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);

		memcpy(Data.pData, _pData, (size_t)(m_uElementStride * _uCount));

		pDevice->GetDeviceContext()->Unmap(m_StructBuffer.Get(), 0);

		break;
	}

	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
	{
		D3D11_MAPPED_SUBRESOURCE Data = {};
		pDevice->GetDeviceContext()->Map(m_StagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &Data);

		memcpy(Data.pData, _pData, (size_t)(m_uElementStride * _uCount));

		pDevice->GetDeviceContext()->Unmap(m_StagingBuffer.Get(), 0);

		pDevice->GetDeviceContext()->CopyResource(m_StructBuffer.Get(), m_StagingBuffer.Get());

		break;
	}

	}
}

void CStructBuffer::GetData(void* _pDest, UINT _uDestCapacity)
{
	ComPtr<ID3D11DeviceContext> pContext = CONTEXT;

	switch (m_eSBufferType)
	{

	//�б� ���� Struct ������ ��쿡�� �׳� �ٷ� �����͸� �������� �ȴ�.
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		D3D11_MAPPED_SUBRESOURCE Data = {};

		pContext->Map(m_StructBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

		size_t bytesize = m_uElementStride * m_uElementCapacity;

		memcpy_s(_pDest, _uDestCapacity, Data.pData, bytesize);

		pContext->Unmap(m_StructBuffer.Get(), 0);

		break;
	}
	
	//�б�/���� ��� ������ Struct ������ ��쿡�� Staging ���۸� ���ؼ� �����´�.
	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
	{
		pContext->CopyResource(m_StagingBuffer.Get(), m_StructBuffer.Get());

		D3D11_MAPPED_SUBRESOURCE Data = {};
		pContext->Map(m_StagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

		size_t bytesize = m_uElementStride * m_uElementCapacity;
		memcpy_s(_pDest, bytesize, Data.pData, bytesize);

		pContext->Unmap(m_StagingBuffer.Get(), 0);
		break;
	}
	
	default: break;
	}
}

void CStructBuffer::BindBufferSRV()
{
	//UAV�� ���ε� �Ǿ����� ��� ���ε��� ����
	if (eCURRENT_BOUND_VIEW::UAV == m_eCurrentBoundView)
		UnBindUAV();

	m_eCurrentBoundView = eCURRENT_BOUND_VIEW::SRV;

	//������� ���ε�
	BindConstBuffer();

	if (eSHADER_PIPELINE_STAGE::__VERTEX & m_flagPipelineTarget)
	{
		CONTEXT->VSSetShaderResources((UINT)m_eSRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__HULL & m_flagPipelineTarget)
	{
		CONTEXT->HSSetShaderResources((UINT)m_eSRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__DOMAIN & m_flagPipelineTarget)
	{
		CONTEXT->DSSetShaderResources((UINT)m_eSRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__GEOMETRY & m_flagPipelineTarget)
	{
		CONTEXT->GSSetShaderResources((UINT)m_eSRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__PIXEL & m_flagPipelineTarget)
	{
		CONTEXT->PSSetShaderResources((UINT)m_eSRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (__COMPUTE & m_flagPipelineTarget)
	{
		CONTEXT->CSSetShaderResources((UINT)m_eSRVIdx, 1, m_SRV.GetAddressOf());
	}
}

void CStructBuffer::BindBufferUAV()
{
	//�б� ���� �� ������ ���°� �ƴҰ�� assert
	assert(eSTRUCT_BUFFER_BIND_TYPE::READ_WRITE == m_eSBufferType);

	if (eCURRENT_BOUND_VIEW::SRV == m_eCurrentBoundView)
		UnBindSRV();
	m_eCurrentBoundView = eCURRENT_BOUND_VIEW::UAV;

	m_flagPipelineTarget |= __COMPUTE;
	BindConstBuffer();

	static const UINT Offset = -1;
	CONTEXT->CSSetUnorderedAccessViews((UINT)m_eUAVIdx, 1, m_UAV.GetAddressOf(), &Offset);
	
}

void CStructBuffer::BindConstBuffer()
{
	//����ü ������ ���� ������ۿ� ���ε��� ����ü ������ �־ ����
	//��������� �ּҴ� �ѹ� ����Ǹ� ������ �����Ƿ� static, const ���·� ����.
	static CConstBuffer* const pStructCBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_SBUFFERINFO);
	pStructCBuffer->UploadData(g_arrStructBufferInfo);
	pStructCBuffer->SetPipelineTarget(m_flagPipelineTarget);
	pStructCBuffer->BindBuffer();
}

void CStructBuffer::CreateStagingBuffer()
{
	//CPU ��ſ� ������¡ ���� ����
	D3D11_BUFFER_DESC Desc = m_BufferDesc;
	Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	Desc.Usage = D3D11_USAGE_STAGING;
	Desc.BindFlags = 0;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&Desc, nullptr, m_StagingBuffer.ReleaseAndGetAddressOf())))
		assert(nullptr);
}

void CStructBuffer::CreateSRV()
{
	//SRV ����
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

void CStructBuffer::UnBindSRV()
{
	ID3D11ShaderResourceView* pView = nullptr;

	if (eSHADER_PIPELINE_STAGE::__VERTEX & m_flagPipelineTarget)
	{
		CONTEXT->VSSetShaderResources((UINT)m_eSRVIdx, 1, &pView);
	}

	if (eSHADER_PIPELINE_STAGE::__HULL & m_flagPipelineTarget)
	{
		CONTEXT->HSSetShaderResources((UINT)m_eSRVIdx, 1, &pView);
	}

	if (eSHADER_PIPELINE_STAGE::__DOMAIN & m_flagPipelineTarget)
	{
		CONTEXT->DSSetShaderResources((UINT)m_eSRVIdx, 1, &pView);
	}

	if (eSHADER_PIPELINE_STAGE::__GEOMETRY & m_flagPipelineTarget)
	{
		CONTEXT->GSSetShaderResources((UINT)m_eSRVIdx, 1, &pView);
	}

	if (eSHADER_PIPELINE_STAGE::__PIXEL & m_flagPipelineTarget)
	{
		CONTEXT->PSSetShaderResources((UINT)m_eSRVIdx, 1, &pView);
	}

	if (__COMPUTE & m_flagPipelineTarget)
	{
		CONTEXT->CSSetShaderResources((UINT)m_eSRVIdx, 1, &pView);
	}

	m_eCurrentBoundView = eCURRENT_BOUND_VIEW::NONE;
}

void CStructBuffer::UnBindUAV()
{
	static const UINT Offset = -1;
	ID3D11UnorderedAccessView* pUAV = nullptr;
	CONTEXT->CSSetUnorderedAccessViews((UINT)m_eUAVIdx, 1, &pUAV, &Offset);
	m_eCurrentBoundView = eCURRENT_BOUND_VIEW::NONE;
}
