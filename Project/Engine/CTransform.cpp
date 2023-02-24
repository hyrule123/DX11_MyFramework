#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"

#include "CRenderMgr.h"
#include "CMeshRender.h"

//�ڽ��� ������Ʈ �Ǹ� �浹ü�� ������Ʈ ���ش�.
#include "CCollider2D.h"
#include "CCollider3D.h"

CTransform::CTransform()
	: CComponent(eCOMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(1.f, 1.f, 1.f)
	, m_bInheritScale(true)
	, m_bInheritRot(true)
	, m_bSizeUpdated(true)
	//Matrix�� Vector ������ ��ü �����ڸ� ���� �ʱ�ȭ ��.
{
}

CTransform::~CTransform()
{
}



void CTransform::finaltick()
{
	//bool ������ tick()���� false�� �ʱ�ȭ �ȴ�.


	//���⼱ ���� ũ��(Size)�� �ݿ����� ���� ��������� �����.
	//���ӿ�����Ʈ ��� ���迡�� ���� ũ����� ����� �ް� �Ǹ� ���ϱ޼������� ũ�Ⱑ Ŀ�� 
	if (true == m_bNeedMyUpdate)
	{
		//�ڽ��� Ʈ������ ������Ʈ�� ������ ��� - �ΰ� �� ������Ʈ �������.
		UpdateMyTransform();
	}
	//�θ� Ʈ������'��' ���ŵǾ��� ��� : �ڽ��� ������ �ʿ� ����.
	if (true == m_bNeedParentUpdate)
	{
		UpdateParentMatrix();
	}

	//���߿� �ϳ��� ������Ʈ �Ǿ��� ��� ��������� ���� ����Ѵ�.
	if (m_bNeedMyUpdate || m_bNeedParentUpdate)
	{
		m_matWorld = m_matRelative * m_matParent;
	}
}



void CTransform::UpdateMyTransform()
{
	//ũ�����(CreateScale�� ���ָ� �ڵ����� ������ǥ�� �߰��ؼ� ��Ŀ� ������ �ش�.
	const Matrix& matScale = Matrix::CreateScale(m_vRelativeScale);

	//������ ���ʹϾ��� ����ؼ� ���.
	//ȸ���� ������� ��� ������� ����.
	Matrix matRot = Matrix::Identity;
	/*if (false == m_bLockRot)
		matRot = MATRIX::CreateFromPitchYawRoll(m_vRelativeRot.x, m_vRelativeRot.y, m_vRelativeRot.z);*/

	if (false == m_bLockRot)
		matRot = MATRIX::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(m_vRelativeRot.y, m_vRelativeRot.x, m_vRelativeRot.z));
	

	//��� ���� ȸ��������� ������ ������ ����Ѵ�.
	//ȸ������� ���� ������ �������� �����Ƿ� ���������� ����س��� �������� ������ ���⵵ ���س��´�.
	//��� 1 - ��İ��� �����Լ��� ���
	m_vRelativeDir[(UINT)eDIR_TYPE::FRONT] = matRot.Forward();
	m_vRelativeDir[(UINT)eDIR_TYPE::RIGHT] = matRot.Right();
	m_vRelativeDir[(UINT)eDIR_TYPE::UP] = matRot.Up();

	//�̵����
	const Matrix& matTranslation = Matrix::CreateTranslation(m_vRelativePos);

	//�ڽ��� ������� �ϼ�(��밪)
	m_matRelative = matScale * matRot * matTranslation;
}

void CTransform::UpdateParentMatrix()
{
	m_matParent = Matrix::Identity;


	//�θ� ������Ʈ�� ���� ��� �θ��� ��������� �޾ƿ´�. 
	//���� �� true�� ��ȯ�ǹǷ� �� ���� ��� ������ �����ϸ� ��
	bool bWorldDirInherit = false;
	if (true == GetOwner()->GetParentWorldMatrix(m_matParent))
	{
		if (true == m_bInheritRot)
		{
			//ȸ�� ��� + ũ�� �̻�� -> ũ������ ����
			if (false == m_bInheritScale)
			{
				//����ȭ�ؼ� ũ�������� ����
				m_matParent.Right(m_matParent.Right().Normalize());
				m_matParent.Up(m_matParent.Up().Normalize());
				m_matParent.Forward(m_matParent.Forward().Normalize());
			}
			//else: �� �� ��� �޴� ��쿡�� �۾��� ���� ����. �׳� ���������� ��

			bWorldDirInherit = true;	//�̶��� ��������� ��ӹ޾��ָ� �ȴ�.
		}
		else
		{
			//ȸ�� �̻�� + ũ�� ��� -> ȸ������ ����
			if (true == m_bInheritScale)
			{
				//ȸ�������� ��ӹ޴� ���: ũ�������� ����
				Vec3 Scale(m_matParent.Right().Length(), m_matParent.Up().Length(), m_matParent.Forward().Length());
				//float(4) * 12 -> ȸ�� ��Ʈ�� ��� 0���� �о������ ũ�⸸ ���
				memset(m_matParent.m, 0, 48);
				m_matParent._11 = Scale.x;
				m_matParent._22 = Scale.y;
				m_matParent._33 = Scale.z;
			}
			//ȸ�� �̻�� + ũ�� �̻�� -> ���� �а� ������ķ�
			else
			{
				memset(m_matParent.m, 0, 48);
				m_matParent._11 = 1.f;
				m_matParent._22 = 1.f;
				m_matParent._33 = 1.f;
			}
		}
	}
}

void CTransform::UpdateData()
{
	MATRIX matWorld = m_matSize * m_matWorld;
	GetOwner()->SetScalarParam(MTRL_SCALAR_MAT_WORLD, &matWorld);
	//matWorld *= g_matViewProj;
	//GetOwner()->SetScalarParam(MTRL_SCALAR_MAT_WVP, &matWorld);
}
