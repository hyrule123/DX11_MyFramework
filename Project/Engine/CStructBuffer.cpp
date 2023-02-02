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

		//CPU에서 구조화버퍼에 작성할 것이므로 AccessFlag를 write로 설정
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		//CPU에서 버퍼를 작성하고 GPU에서 버퍼를 읽어들일 경우 사용
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		//SRV만 바인딩되도록 설정
		m_BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		break;
	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
		

		//CPU는 접근 불가, GPU의 읽기/쓰기는 가능
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;

		//SRV, UAV 둘 다 바인딩되도록 설정
		m_BufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

		break;
	default:
		break;
	}

	//기타 플래그에 구조화 버퍼로 설정값이 있음.
	m_BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
}

CStructBuffer::~CStructBuffer()
{

}

void CStructBuffer::Create(UINT _uiElementSize, UINT _uElementCapacity)
{
	//상수버퍼와 마찬가지로 16바이트 단위로 정렬되어 있어야 함.
	assert(0 == _uiElementSize % 16);

	//상수버퍼와는 다르게 버퍼 재할당이 가능함. 먼저 기존 버퍼의 할당을 해제한다.(ComPtr을 통해 관리가 이루어지므로 nullptr로 바꿔주면 됨.)
	m_uElementSize = _uiElementSize;
	m_uElementCapacity = _uElementCapacity;

	m_BufferDesc.StructureByteStride = m_uElementSize;
	m_BufferDesc.ByteWidth = m_uElementSize * m_uElementCapacity;

	switch (m_eSBufferType)
	{
	//일반적인 GPU에서 읽기만 가능한 구조화 버퍼
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		//구조화버퍼 생성
		if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, nullptr, m_StructBuffer.ReleaseAndGetAddressOf())))
			assert(nullptr);

		CreateSRV();

		break;
	}
	
	//Compute Shader과 결합해서 사용하는, GPU에서 조작 가능한 SBuffer
	//이 경우에는 CPU와 데이터 전송이 불가능하므로 데이터 전송용 Staging Buffer를 중간 단계로 사용해야 한다.
	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
	{
		//구조화버퍼 생성
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

	//생성 시 할당된 갯수보다 들어온 갯수가 더 클 경우 재할당하고, 거기에 데이터를 추가.
	if (_uiCount > m_uElementCapacity)
		Create(m_uElementSize, _uiCount * 2u);

	CDevice* pDevice = CDevice::GetInst();

	switch (m_eSBufferType)
	{
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		//Read Only일 경우 SBuffer에 바로 Map/UnMap을 해주면 된다.
		D3D11_MAPPED_SUBRESOURCE Data = {};
		pDevice->GetDeviceContext()->Map(m_StructBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);

		memcpy(Data.pData, _pData, (size_t)(m_uElementSize * _uiCount));

		pDevice->GetDeviceContext()->Unmap(m_StructBuffer.Get(), 0);

		break;
	}

	case eSTRUCT_BUFFER_TYPE::READ_WRITE:
	{
		//Read+Write일 경우 중간 버퍼로 복사한 후 GPU의 버퍼로 복사한다.

		D3D11_MAPPED_SUBRESOURCE Data = {};
		pDevice->GetDeviceContext()->Map(m_StagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &Data);

		memcpy(Data.pData, _pData, (size_t)(m_uElementSize * _uiCount));

		pDevice->GetDeviceContext()->Unmap(m_StagingBuffer.Get(), 0);

		pDevice->GetDeviceContext()->CopyResource(m_StructBuffer.Get(), m_StagingBuffer.Get());

		break;
	}

	}

	//g_arrStructBufferInfo의 자신의 인덱스에 해당하는 위치에 이번에 업데이트된 구조체의 갯수를 삽입
	g_arrStructBufferInfo[(UINT)m_eCBufferIdx].uSBufferCount = _uiCount;
}

void CStructBuffer::GetData(void* _pDest)
{
	ComPtr<ID3D11DeviceContext> pContext = CONTEXT;

	switch (m_eSBufferType)
	{

	//읽기 전용 Struct 버퍼일 경우에는 그냥 바로 데이터를 가져오면 된다.
	case eSTRUCT_BUFFER_TYPE::READ_ONLY:
	{
		D3D11_MAPPED_SUBRESOURCE Data = {};

		pContext->Map(m_StructBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

		size_t bytesize = m_uElementSize * m_uElementCapacity;
		memcpy_s(_pDest, bytesize, Data.pData, bytesize);

		pContext->Unmap(m_StructBuffer.Get(), 0);

		break;
	}
	
	//읽기/쓰기 모두 가능한 Struct 버퍼일 경우에는 Staging 버퍼를 통해서 가져온다.
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
	//UAV와 바인딩 되어있을 경우 바인딩을 해제
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
	//구조체 정보를 담은 상수버퍼에 바인딩한 구조체 갯수를 넣어서 전달
	CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_SBUFFERINFO);
	pConstBuffer->UpdateData(g_arrStructBufferInfo);
	pConstBuffer->SetPipelineTarget(m_flagPipelineTarget);
	pConstBuffer->BindData();
}

void CStructBuffer::CreateSRV()
{
	//GPU에서는 읽기 작업만 할 것이므로 SRV로 생성.
	//SRVDesc에 원소의 갯수를 지정해서 생성.
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.NumElements = m_uElementCapacity;

	if (FAILED(DEVICE->CreateShaderResourceView(m_StructBuffer.Get(), &SRVDesc, m_SRV.ReleaseAndGetAddressOf())))
		assert(nullptr);
}

void CStructBuffer::CreateUAV()
{
	//GPU에서 읽기 및 쓰기 작업이 가능해야 하므로 UAV 형태로 생성
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.NumElements = m_uElementCapacity;

	if (FAILED(DEVICE->CreateUnorderedAccessView(m_StructBuffer.Get(), &UAVDesc, m_UAV.ReleaseAndGetAddressOf())))
		assert(nullptr);
}
