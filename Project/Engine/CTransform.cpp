#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	
}

void CTransform::UpdateData()
{
	// ��ġ���� ������ۿ� ���� �� ���ε�		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTransformBuffer->SetData(&m_vRelativePos, sizeof(Vec3));
	pTransformBuffer->UpdateData();
}
