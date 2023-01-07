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

	//������ ���ʹϾ��� ����ؼ� ���.
	const Matrix& matRot = Matrix::CreateFromPitchYawRoll(m_vRelativeRot.x, m_vRelativeRot.y, m_vRelativeRot.z);

	//������ ������ ����Ѵ�.
	//ȸ������� ���� ������ �������� �����Ƿ� ���������� ����س��� �������� ������ ���⵵ ���س��´�.
	//��� 1 - ��İ��� �����Լ��� ���
	m_vRelativeDir[eDIR_TYPE::eDIR_FRONT] = matRot.Forward();
	m_vRelativeDir[eDIR_TYPE::eDIR_RIGHT] = matRot.Right();
	m_vRelativeDir[eDIR_TYPE::eDIR_UP] = matRot.Up();


	const Matrix& matTranslation = Matrix::CreateTranslation(m_vRelativePos);

	m_matWorld = matScale * matRot * matTranslation;
}

void CTransform::UpdateData()
{
	//�������������� ���� �� ��ġ�Ѵ�.(HLSL�� Column-Major Matrix, XMMATRIX������ Row-Major Matrix�� ��� ��)
	Matrix matWVP = (m_matWorld * g_transform.matViewProj).Transpose();

	//���� ����� ������ۿ� ���� �� ���ε�
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(eCB_TYPE::TRANSFORM);
	pTransformBuffer->SetData(&matWVP, sizeof(Matrix));
	pTransformBuffer->UpdateData();
}
