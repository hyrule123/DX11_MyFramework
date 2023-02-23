#include "pch.h"
#include "CStructBuffer.h"

#include "CConstBuffer.h"

#include "CDevice.h"


CStructBuffer::CStructBuffer(eSTRUCT_BUFFER_TYPE _type, UINT _eSHADER_PIPELINE_STAGE_FLAG_SRV, eCBUFFER_SBUFFER_SHAREDATA_IDX _CBIdx, int _SRVIdx, int _UAVIdx)
	: m_eSBufferType(_type)
	, m_flagPipelineTargetSRV(_eSHADER_PIPELINE_STAGE_FLAG_SRV)
	, m_eCBufferIdx(_CBIdx)
	, m_e_t_SRVIdx(_SRVIdx)
	, m_e_u_UAVIdx(_UAVIdx)
	, m_uElementStride()
	, m_uElementCount()
	, m_uElementCapacity()
	, m_BufferDesc{}
	, m_eCurBoundView()
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

		//READ_WRITE�� ����ϰڴٴ� �� ��ǻƮ���̴��� ����ϰڴٴ� �ǹ� -> �Ǽ� ������ ���� �÷��׿� ��ǻƮ���̴� �߰�
		m_flagPipelineTargetSRV |= eSHADER_PIPELINE_STAGE::__COMPUTE;

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

void CStructBuffer::Create(UINT _uElemStride, UINT _uElemCapacity, void* _pInitialData, UINT _uElemCount)
{
	//������ۿ� ���������� 16����Ʈ ������ ���ĵǾ� �־�� ��.
	assert(0 == _uElemStride % 16);

	assert(_uElemCapacity >= _uElemCount);

	//���Ҵ� �ϱ� �� ���ε��� ���ҽ��� �ִٸ� unbind
	switch (m_eCurBoundView)
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
	m_uElementStride = _uElemStride;

	m_uElementCount = _uElemCount;
	if (eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE != m_eCBufferIdx)
		g_arrSBufferShareData[(UINT)m_eCBufferIdx].uSBufferCount = _uElemCount;

	m_uElementCapacity = _uElemCapacity;

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

			//�� ���� ���õ�. BufferDesc.ByteWidth ���� ������ ��ġ�� ���� Ȯ����.
			Data.SysMemPitch = m_uElementStride * _uElemCount;
			Data.SysMemSlicePitch = m_BufferDesc.StructureByteStride;
			pData = &Data;
			m_eCurBoundView = eCURRENT_BOUND_VIEW::SRV;
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
			m_eCurBoundView = eCURRENT_BOUND_VIEW::UAV;
		}

		//����ȭ���� ����
		if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, pData, m_StructBuffer.ReleaseAndGetAddressOf())))
			assert(nullptr);

		CreateSRV();
		CreateUAV();

		break;
	}
		
	}
}

void CStructBuffer::UploadData(void* _pData, UINT _uCount)
{
	m_uElementCount = _uCount;
	//g_arrSBufferShareData�� �ڽ��� �ε����� �ش��ϴ� ��ġ�� �̹��� ������Ʈ�� ����ü�� ������ ����
	//��� ������ ���ε��� BindData()�� �ϴ� ������ ���ش�.
	if(eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE != m_eCBufferIdx)
		g_arrSBufferShareData[(UINT)m_eCBufferIdx].uSBufferCount = _uCount;



	//���� �� �Ҵ�� �������� ���� ������ �� Ŭ ��� ���Ҵ��ϰ�, �ű⿡ �����͸� �߰�.
	//������ �� ���� ������ �� �����Ƿ� �ٷ� return ���ָ� �ɵ�
	if (_uCount > m_uElementCapacity)
	{
		//�ٽ� �����ϰ��� �Ҷ��� �ʱ� �����Ϳ� ����� ��ġ���Ѽ� ��������� �Ѵ�.
		Create(m_uElementStride, _uCount, _pData, _uCount);
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
		if (nullptr == m_StagingBuffer.Get())
			CreateStagingBuffer();
		else
		{
			D3D11_BUFFER_DESC pDesc = {};
			m_StagingBuffer->GetDesc(&pDesc);

			//�����Ǿ� �ִ� Staging Buffer�� ũ�⺸�� Ŭ ��� ���� ����
			if (pDesc.ByteWidth < m_BufferDesc.ByteWidth)
				CreateStagingBuffer();
		}

		D3D11_MAPPED_SUBRESOURCE Data = {};
		pDevice->GetDeviceContext()->Map(m_StagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &Data);

		memcpy(Data.pData, _pData, (size_t)(m_uElementStride * _uCount));

		pDevice->GetDeviceContext()->Unmap(m_StagingBuffer.Get(), 0);

		pDevice->GetDeviceContext()->CopyResource(m_StructBuffer.Get(), m_StagingBuffer.Get());

		break;
	}

	}
}

void CStructBuffer::GetData(void* _pDest, UINT _uDestByteCapacity)
{
	ComPtr<ID3D11DeviceContext> pContext = CONTEXT;

	switch (m_eSBufferType)
	{

	//�б� ���� Struct ������ ��쿡�� �׳� �ٷ� �����͸� �������� �ȴ�.
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		D3D11_MAPPED_SUBRESOURCE Data = {};

		pContext->Map(m_StructBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

		size_t bytesize = m_uElementStride * m_uElementCount;

		memcpy_s(_pDest, _uDestByteCapacity, Data.pData, bytesize);

		pContext->Unmap(m_StructBuffer.Get(), 0);

		break;
	}
	
	//�б�/���� ��� ������ Struct ������ ��쿡�� Staging ���۸� ���ؼ� �����´�.
	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
	{
		if (nullptr == m_StagingBuffer.Get())
			CreateStagingBuffer();
		else
		{
			D3D11_BUFFER_DESC pDesc = {};
			m_StagingBuffer->GetDesc(&pDesc);

			//�����Ǿ� �ִ� Staging Buffer�� ũ�⺸�� Ŭ ��� ���� ����
			if (pDesc.ByteWidth < m_BufferDesc.ByteWidth)
				CreateStagingBuffer();
		}

		pContext->CopyResource(m_StagingBuffer.Get(), m_StructBuffer.Get());

		D3D11_MAPPED_SUBRESOURCE Data = {};
		pContext->Map(m_StagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

		size_t bytesize = m_uElementStride * m_uElementCapacity;
		memcpy_s(_pDest, _uDestByteCapacity, Data.pData, bytesize);

		pContext->Unmap(m_StagingBuffer.Get(), 0);
		break;
	}
	
	default: break;
	}
}

void CStructBuffer::BindBufferSRV()
{
	//UAV�� ���ε� �Ǿ����� ��� ���ε��� ����
	if (eCURRENT_BOUND_VIEW::UAV == m_eCurBoundView)
		UnBindUAV();

	m_eCurBoundView = eCURRENT_BOUND_VIEW::SRV;

	//������� ���ε�
	BindConstBuffer(m_flagPipelineTargetSRV);

	if (eSHADER_PIPELINE_STAGE::__VERTEX & m_flagPipelineTargetSRV)
	{
		CONTEXT->VSSetShaderResources(m_e_t_SRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__HULL & m_flagPipelineTargetSRV)
	{
		CONTEXT->HSSetShaderResources(m_e_t_SRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__DOMAIN & m_flagPipelineTargetSRV)
	{
		CONTEXT->DSSetShaderResources(m_e_t_SRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__GEOMETRY & m_flagPipelineTargetSRV)
	{
		CONTEXT->GSSetShaderResources(m_e_t_SRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__PIXEL & m_flagPipelineTargetSRV)
	{
		CONTEXT->PSSetShaderResources(m_e_t_SRVIdx, 1, m_SRV.GetAddressOf());
	}

	if (eSHADER_PIPELINE_STAGE::__COMPUTE & m_flagPipelineTargetSRV)
	{
		CONTEXT->CSSetShaderResources(m_e_t_SRVIdx, 1, m_SRV.GetAddressOf());
	}
}

void CStructBuffer::BindBufferUAV()
{
	//�б� ���� �� ������ ���°� �ƴҰ�� assert
	assert(eSTRUCT_BUFFER_BIND_TYPE::READ_WRITE == m_eSBufferType);

	if (eCURRENT_BOUND_VIEW::SRV == m_eCurBoundView)
		UnBindSRV();
	m_eCurBoundView = eCURRENT_BOUND_VIEW::UAV;

	//m_flagPipelineTargetSRV |= eSHADER_PIPELINE_STAGE::__COMPUTE;
	BindConstBuffer(eSHADER_PIPELINE_STAGE::__COMPUTE);

	static const UINT Offset = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_e_u_UAVIdx, 1, m_UAV.GetAddressOf(), &Offset);
	
}

void CStructBuffer::BindConstBuffer(UINT _eSHADER_PIPELINE_FLAG)
{
	//������� ���ε� ������ ���� �ʾ��� ��� return
	if (eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE == m_eCBufferIdx)
		return;

	//����ü ������ ���� ������ۿ� ���ε��� ����ü ������ �־ ����
	//��������� �ּҴ� �ѹ� ����Ǹ� ������ �����Ƿ� static, const ���·� ����.
	static CConstBuffer* const pStructCBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_SBUFFER_SHAREDATA);
	pStructCBuffer->UploadData(g_arrSBufferShareData);
	pStructCBuffer->BindBuffer(_eSHADER_PIPELINE_FLAG);
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

	if (eSHADER_PIPELINE_STAGE::__VERTEX & m_flagPipelineTargetSRV)
	{
		CONTEXT->VSSetShaderResources(m_e_t_SRVIdx, 1, &pView);
	}

	if (eSHADER_PIPELINE_STAGE::__HULL & m_flagPipelineTargetSRV)
	{
		CONTEXT->HSSetShaderResources(m_e_t_SRVIdx, 1, &pView);
	}

	if (eSHADER_PIPELINE_STAGE::__DOMAIN & m_flagPipelineTargetSRV)
	{
		CONTEXT->DSSetShaderResources(m_e_t_SRVIdx, 1, &pView);
	}

	if (eSHADER_PIPELINE_STAGE::__GEOMETRY & m_flagPipelineTargetSRV)
	{
		CONTEXT->GSSetShaderResources(m_e_t_SRVIdx, 1, &pView);
	}

	if (eSHADER_PIPELINE_STAGE::__PIXEL & m_flagPipelineTargetSRV)
	{
		CONTEXT->PSSetShaderResources(m_e_t_SRVIdx, 1, &pView);
	}

	if (eSHADER_PIPELINE_STAGE::__COMPUTE & m_flagPipelineTargetSRV)
	{
		CONTEXT->CSSetShaderResources(m_e_t_SRVIdx, 1, &pView);
	}

	m_eCurBoundView = eCURRENT_BOUND_VIEW::NONE;
}

void CStructBuffer::UnBindUAV()
{
	static const UINT Offset = -1;
	ID3D11UnorderedAccessView* pUAV = nullptr;
	CONTEXT->CSSetUnorderedAccessViews(m_e_u_UAVIdx, 1, &pUAV, &Offset);
	m_eCurBoundView = eCURRENT_BOUND_VIEW::NONE;
}
