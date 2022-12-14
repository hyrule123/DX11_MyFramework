#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM),
	m_vRelativeScale(1.f, 1.f, 1.f)
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	//ũ�����(CreateScale�� ���ָ� �ڵ����� ������ǥ�� �߰��ؼ� ��Ŀ� ������ �ش�.
	const Matrix& matScale = Matrix::CreateScale(m_vRelativeScale);


	//Matrix matRot = Matrix::CreateRotationX(m_vRelativeRot.x);
	//matRot *= Matrix::CreateRotationY(m_vRelativeRot.y);
	//matRot *= Matrix::CreateRotationZ(m_vRelativeRot.z);

	const DirectX::XMVECTOR& XmVec = XMQuaternionRotationRollPitchYaw(m_vRelativeRot.x, m_vRelativeRot.y, m_vRelativeRot.z);

	const Matrix& matRot = Matrix::CreateFromQuaternion(XmVec);

	const Matrix& matTranslation = Matrix::CreateTranslation(m_vRelativePos);

	m_matWorld = (matScale * matRot * matTranslation).Transpose();
	//m_matWorld = m_matWorld.Transpose();
}

void CTransform::UpdateData()
{
	// ��ġ���� ������ۿ� ���� �� ���ε�		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTransformBuffer->SetData(&m_matWorld, sizeof(Matrix));
	//pTransformBuffer->SetData(&m_vRelativePos, sizeof(Vec3));
	pTransformBuffer->UpdateData();
}
