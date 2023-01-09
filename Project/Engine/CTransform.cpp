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
	//여기선 고유 크기(Size)를 반영하지 않은 월드행렬을 만든다.
	//게임오브젝트 상속 관계에서 고유 크기까지 상속을 받게 되면 기하급수적으로 크기가 커짐 
	
	
	//크기행렬(CreateScale을 해주면 자동으로 동차좌표를 추가해서 행렬에 삽입해 준다.
	const Matrix& matScale = Matrix::CreateScale(m_vRelativeScale);

	//방향은 쿼터니언을 사용해서 계산.
	const Matrix& matRot = Matrix::CreateFromPitchYawRoll(m_vRelativeRot.x, m_vRelativeRot.y, m_vRelativeRot.z);

	//직관적 방향을 계산한다.
	//회전행렬을 따로 변수에 저장하지 않으므로 지역변수에 계산해놓은 시점에서 직관적 방향도 구해놓는다.
	//방법 1 - 행렬곱과 래핑함수를 사용
	m_vRelativeDir[eDIR_TYPE::eDIR_FRONT] = matRot.Forward();
	m_vRelativeDir[eDIR_TYPE::eDIR_RIGHT] = matRot.Right();
	m_vRelativeDir[eDIR_TYPE::eDIR_UP] = matRot.Up();

	const Matrix& matTranslation = Matrix::CreateTranslation(m_vRelativePos);

	//자신의 월드행렬 완성
	m_matWorld = matScale * matRot * matTranslation;


	//우선 단위행렬을 하나 만든 뒤
	static Matrix matInherit;
	//부모의 월드행렬을 받아온다. 성공 시 true가 반환되므로 이 때는 상속 과정을 시작하면 됨
	if (true == GetOwner()->GetParentWorldMatrix(matInherit))
	{
		do
		{
			//둘 다 상속받는 경우 그대로 빠져나감(부모의 월드 매트릭스를 그대로 사용)
			if (true == m_bInheritScale && true == m_bInheritRot)
				break;
			//둘다 상속 안받는 경우
			else if (false == m_bInheritScale && false == m_bInheritRot)
			{
				//회전/크기 정보 부분을 밀어버리고 1.f를 넣어줌
				//float 4byte * 12(3열까지) = 48
				memset(matInherit.m, 0, 48);
				matInherit._11 = 1.f;
				matInherit._22 = 1.f;
				matInherit._33 = 1.f;

				//회전, 크기 모두 상속받지 않을경우 모두 제거하고 이동정보만 남김
				//matInherit = Matrix::CreateTranslation(matInherit.Translation());
				break;
			}

			//크기를 구한다.(Right = x, Up = y, Forward = z)
			Vec3 Scale(matInherit.Right().Length(), matInherit.Up().Length(), matInherit.Forward().Length());

			//둘 중 하나만 상속받는 경우 - 크기를 상속받지 않는경우
			//회전 정보만 남겨줌 -> 모든 벡터를 크기로 나눠줌
			if (false == m_bInheritScale)
			{
				for (int i = 0; i < 3; ++i)
				{
					Scale = Vec3(1.f, 1.f, 1.f) / Scale;
					matInherit.m[0][i] *= Scale.x;
					matInherit.m[1][i] *= Scale.y;
					matInherit.m[2][i] *= Scale.z;
				}
			}
			//회전을 상속받지 않는경우 - 크기만 남김
			else if (false == m_bInheritRot)
			{
				//float(4) * 12 -> 회전 파트를 모두 0으로 밀어버리고 크기만 등록
				memset(matInherit.m, 0, 48);
				matInherit._11 = Scale.x;
				matInherit._22 = Scale.y;
				matInherit._33 = Scale.z;
			}
		} while (false);

		//부모의 회전행렬을 곱한다.
		m_matWorld *= matInherit;
	}
}

void CTransform::UpdateData()
{
	//자신의 사이즈를 적용한 WVP 행렬을 만들어 상수버퍼로 업데이트 한다.
	const Matrix& matSize = Matrix::CreateScale(m_vSize);

	//월드뷰투영행렬을 곱한 후 전치한다.(HLSL은 Column-Major Matrix, XMMATRIX에서는 Row-Major Matrix를 사용 중)
	Matrix matWVP = (matSize * m_matWorld * g_transform.matViewProj).Transpose();

	//위의 행렬을 상수버퍼에 전달 및 바인딩
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(eCB_TYPE::TRANSFORM);
	pTransformBuffer->SetData(&matWVP, sizeof(Matrix));
	pTransformBuffer->UpdateData();
}
