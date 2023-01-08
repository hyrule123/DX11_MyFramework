#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"

CTransform::CTransform()
	: CComponent(eCOMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(1.f, 1.f, 1.f)
	, m_bInheritScale(true)
	, m_bInheritRot(true)
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	//���⼱ ���� ũ��(Size)�� �ݿ����� ���� ��������� �����.
	//���ӿ�����Ʈ ��� ���迡�� ���� ũ����� ����� �ް� �Ǹ� ���ϱ޼������� ũ�Ⱑ Ŀ�� 
	 
	
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

	//�ڽ��� ������� �ϼ�
	m_matWorld = matScale * matRot * matTranslation;

	//�θ��� ������� ���
	//�켱 ��������� �ϳ� ���� ��
	static Matrix matInherit = XMMatrixIdentity();

	//�θ��� ��������� �޾ƿ´�. ���� �� true�� ��ȯ�ǹǷ� �� ���� ��� ������ �����ϸ� ��
	if (GetOwner()->GetParentWorldMatrix(matInherit))
	{
		do
		{
			//�� �� ��ӹ޴� ��� �״�� ��������
			if (true == m_bInheritScale && true == m_bInheritRot)
				break;
			
			//�Ѵ� ��� �ȹ޴� ���
			if (false == m_bInheritScale && false == m_bInheritRot)
			{
				//ȸ��, ũ�� ��� ��ӹ��� ������� ��� �����ϰ� �̵������� ����
				matInherit = Matrix::CreateTranslation(matInherit.Translation());
				break;
			}

			//ũ�⸦ ���Ѵ�.
			Vec3 Scale(matInherit.Right().Length(), matInherit.Up().Length(), matInherit.Forward().Length());

			//�� �� �ϳ��� ��ӹ޴� ��� - ũ�⸦ ��ӹ��� �ʴ°��
			//��� ���͸� ũ��� ������
			if (false == m_bInheritScale)
			{
				for (int i = 0; i < 3; ++i)
				{
					matInherit.m[0][i] /= Scale.x;
					matInherit.m[1][i] /= Scale.y;
					matInherit.m[2][i] /= Scale.z;
				}
			}
			//ȸ���� ��ӹ��� �ʴ°�� - ȸ���� ����
			else if (false == m_bInheritScale)
			{
				matInherit.Right(	Vec3(Scale.x, 0.f, 0.f));
				matInherit.Up(		Vec3(0.f, Scale.y, 0.f));
				matInherit.Forward(	Vec3(0.f, 0.f, Scale.z));
			}
		} while (false);

		//�θ��� ȸ������� ���Ѵ�.
		m_matWorld *= matInherit;
	}
}

void CTransform::UpdateData()
{
	//�ڽ��� ����� ������ WVP ����� ����� ������۷� ������Ʈ �Ѵ�.
	const Matrix& matSize = Matrix::CreateScale(m_vSize);

	//�������������� ���� �� ��ġ�Ѵ�.(HLSL�� Column-Major Matrix, XMMATRIX������ Row-Major Matrix�� ��� ��)
	Matrix matWVP = (matSize * m_matWorld * g_transform.matViewProj).Transpose();

	//���� ����� ������ۿ� ���� �� ���ε�
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(eCB_TYPE::TRANSFORM);
	pTransformBuffer->SetData(&matWVP, sizeof(Matrix));
	pTransformBuffer->UpdateData();
}
