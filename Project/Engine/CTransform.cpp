#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"

#include "CRenderMgr.h"
#include "CMeshRender.h"

CTransform::CTransform()
	: CComponent(eCOMPONENT_TYPE::eCOMPONENT_TRANSFORM)
	, m_vRelativeScale(1.f, 1.f, 1.f)
	, m_bInheritScale(true)
	, m_bInheritRot(true)
	, m_vSize(100.f, 100.f, 1.f)
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

	//��� ���� ȸ��������� ������ ������ ����Ѵ�.
	//ȸ������� ���� ������ �������� �����Ƿ� ���������� ����س��� �������� ������ ���⵵ ���س��´�.
	//��� 1 - ��İ��� �����Լ��� ���
	m_vRelativeDir[eDIR_TYPE::eDIR_FRONT] = matRot.Forward();
	m_vRelativeDir[eDIR_TYPE::eDIR_RIGHT] = matRot.Right();
	m_vRelativeDir[eDIR_TYPE::eDIR_UP] = matRot.Up();

	//�̵����
	const Matrix& matTranslation = Matrix::CreateTranslation(m_vRelativePos);

	//�ڽ��� ������� �ϼ�
	m_matWorld = matScale * matRot * matTranslation;


	//�켱 ��������� �ϳ� ���� ��
	static Matrix matInherit;
	//�θ� ������Ʈ�� ���� ��� �θ��� ��������� �޾ƿ´�. 
	//���� �� true�� ��ȯ�ǹǷ� �� ���� ��� ������ �����ϸ� ��
	bool bWorldDirInherit = false;
	if (true == GetOwner()->GetParentWorldMatrix(matInherit))
	{
		if (true == m_bInheritRot)
		{
			//ȸ�� ��� + ũ�� �̻�� -> ũ������ ����
			if (false == m_bInheritScale)
			{
				for (int i = 0; i < 3; ++i)
				{
					//����ȭ�ؼ� ũ�������� ����
					matInherit.Right(matInherit.Right().Normalize());
					matInherit.Up(matInherit.Up().Normalize());
					matInherit.Forward(matInherit.Forward().Normalize());
					

					//Vec3 Scale(matInherit.Right().Length(), matInherit.Up().Length(), matInherit.Forward().Length());
					//Scale = Vec3(1.f, 1.f, 1.f) / Scale;
					//matInherit.m[0][i] *= Scale.x;
					//matInherit.m[1][i] *= Scale.y;
					//matInherit.m[2][i] *= Scale.z;
				}
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
				Vec3 Scale(matInherit.Right().Length(), matInherit.Up().Length(), matInherit.Forward().Length());
				//float(4) * 12 -> ȸ�� ��Ʈ�� ��� 0���� �о������ ũ�⸸ ���
				memset(matInherit.m, 0, 48);
				matInherit._11 = Scale.x;
				matInherit._22 = Scale.y;
				matInherit._33 = Scale.z;
			}
			//ȸ�� �̻�� + ũ�� �̻�� -> ���� �а� ������ķ�
			else
			{
				memset(matInherit.m, 0, 48);
				matInherit._11 = 1.f;
				matInherit._22 = 1.f;
				matInherit._33 = 1.f;
			}
		}

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
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_TRANSFORM);
	pTransformBuffer->SetData(&matWVP, sizeof(Matrix));
	pTransformBuffer->UpdateData();

	//����� ������ ������ ��û
	tDebugShapeInfo dbgInfo = {};
	dbgInfo.eShape = eSHAPE_CIRCLE;
	dbgInfo.fLifeSpan = 0.f;
	dbgInfo.matWVP = matWVP;
	dbgInfo.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(dbgInfo);
}
