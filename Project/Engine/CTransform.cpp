#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"

CTransform::CTransform()
	: CComponent(eCOMPONENT_TYPE::TRANSFORM)
	, m_vRelativeDir{}
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



	//������ ������ ����Ѵ�.
	//��� 1 - ��İ��� �����Լ��� ���
	Matrix matDir;	//�⺻ ������ķ� �ʱ�ȭ�ǹǷ� �ٷ� �����ָ� �ȴ�.
	matDir *= matRot;
	m_vRelativeDir[(int)eDIR_TYPE::RIGHT] = matDir.Right();
	m_vRelativeDir[(int)eDIR_TYPE::UP] = matDir.Up();
	m_vRelativeDir[(int)eDIR_TYPE::FRONT] = matDir.Front();

	//��� 2 - XMVector3TransformNormal�� ���
	////X
	//m_vRelativeDir[(int)eDIR_TYPE::Right] = XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), matRot);

	////Y
	//m_vRelativeDir[(int)eDIR_TYPE::Up] = XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), matRot);

	////Z
	//m_vRelativeDir[(int)eDIR_TYPE::Front] = XMVector3TransformNormal(Vec3(1.f, 0.f, 1.f), matRot);



	//m_vRelativeDir[(int)eDIR_TYPE::Front] = Vec4(1.f, 0.f, 0.f, 0.f) * matRot;

	//const Matrix& matRot = XMMatrixIdentity();
	const Matrix& matTranslation = Matrix::CreateTranslation(m_vRelativePos);

	m_matWorld = matScale * matRot * matTranslation;
	//m_matWorld = m_matWorld.Transpose();
}

void CTransform::UpdateData()
{
	//�������������� ���� �� ��ġ�Ѵ�.
	Matrix matWVP = (m_matWorld * g_transform.MatView * g_transform.MatProj).Transpose();

	// ���� ����� ������ۿ� ���� �� ���ε�
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(eCB_TYPE::TRANSFORM);
	pTransformBuffer->SetData(&matWVP, sizeof(Matrix));
	//pTransformBuffer->SetData(&m_vRelativePos, sizeof(Vec3));
	pTransformBuffer->UpdateData();
}
