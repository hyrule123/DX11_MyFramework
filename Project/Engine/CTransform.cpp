#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"

#include "CRenderMgr.h"
#include "CMeshRender.h"

//자신이 업데이트 되면 충돌체도 업데이트 해준다.
#include "CCollider2D.h"
#include "CCollider3D.h"

CTransform::CTransform()
	: CComponent(eCOMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(1.f, 1.f, 1.f)
	, m_bInheritScale(true)
	, m_bInheritRot(true)
	, m_bSizeUpdated(true)
	, m_bLockRot()
	, m_bNeedMyUpdate(true)
	, m_bNeedParentUpdate(true)
	, m_fLongestDiagonalLen()
	//Matrix와 Vector 변수는 자체 생성자를 통해 초기화 됨.
{
}

CTransform::~CTransform()
{
}



void CTransform::finaltick()
{
	//bool 값들은 tick()에서 false로 초기화 된다.


	//여기선 고유 크기(Size)를 반영하지 않은 월드행렬을 만든다.
	//게임오브젝트 상속 관계에서 고유 크기까지 상속을 받게 되면 기하급수적으로 크기가 커짐 
	if (true == m_bNeedMyUpdate)
	{
		//자신의 트랜스폼 업데이트를 진행할 경우 - 두개 다 업데이트 해줘야함.
		UpdateMyTransform();
	}
	//부모 트랜스폼'만' 갱신되었을 경우 : 자신은 갱신할 필요 없음.
	if (true == m_bNeedParentUpdate)
	{
		UpdateParentMatrix();
	}

	//둘중에 하나라도 업데이트 되었을 경우 월드행렬을 새로 계산한다.
	if (m_bNeedMyUpdate || m_bNeedParentUpdate)
	{
		m_matWorld = m_matRelative * m_matParent;

		MATRIX matWorld = m_matSize * m_matWorld;

		//행렬의 1 ~ 3 부분을 전부 제곱한 뒤, 제곱근을 씌운다. 그러면 가장 긴 대각선의 길이가 나온다.
		//sqrt(11제곱 + 12제곱 + 13제곱 + 21제곱 + 22제곱 + 23제곱 + 31제곱 + 32제곱 + 33제곱)
		m_fLongestDiagonalLen = sqrtf(matWorld.Axis((int)eAXIS3D::X).LengthSquared() + matWorld.Axis((int)eAXIS3D::Y).LengthSquared() + matWorld.Axis((int)eAXIS3D::Z).LengthSquared());
		
		GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_MAT_WORLD, &matWorld);
	}
}



void CTransform::UpdateMyTransform()
{
	//크기행렬(CreateScale을 해주면 자동으로 동차좌표를 추가해서 행렬에 삽입해 준다.
	const Matrix& matScale = Matrix::CreateScale(m_vRelativeScale);

	//방향은 쿼터니언을 사용해서 계산.
	//회전이 잠겨있을 경우 계산하지 않음.
	Matrix matRot = Matrix::Identity;
	/*if (false == m_bLockRot)
		matRot = MATRIX::CreateFromPitchYawRoll(m_vRelativeRot.x, m_vRelativeRot.y, m_vRelativeRot.z);*/

	if (false == m_bLockRot)
	{
		matRot = MATRIX::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(m_vRelativeRot.y, m_vRelativeRot.x, m_vRelativeRot.z));
		//방금 구한 회전행렬으로 직관적 방향을 계산한다.
		//회전행렬을 따로 변수에 저장하지 않으므로 지역변수에 계산해놓은 시점에서 직관적 방향도 구해놓는다.
		//방법 1 - 행렬곱과 래핑함수를 사용
	}

	//TODO: 방향 고정일 경우 사인과 코사인을 이용해서 상대적 방향 계산하도록 만들어 놓기
	m_vRelativeDir[(UINT)eDIR_TYPE::FRONT] = matRot.Forward();
	m_vRelativeDir[(UINT)eDIR_TYPE::RIGHT] = matRot.Right();
	m_vRelativeDir[(UINT)eDIR_TYPE::UP] = matRot.Up();
	



	//이동행렬
	const Matrix& matTranslation = Matrix::CreateTranslation(m_vRelativePos);

	//자신의 월드행렬 완성(상대값)
	m_matRelative = matScale * matRot * matTranslation;
}

void CTransform::UpdateParentMatrix()
{
	m_matParent = Matrix::Identity;


	//부모 오브젝트가 있을 경우 부모의 월드행렬을 받아온다. 
	//성공 시 true가 반환되므로 이 때는 상속 과정을 시작하면 됨
	bool bWorldDirInherit = false;
	if (true == GetOwner()->GetParentWorldMatrix(m_matParent))
	{
		if (true == m_bInheritRot)
		{
			//회전 상속 + 크기 미상속 -> 크기정보 제거
			if (false == m_bInheritScale)
			{
				//정규화해서 크기정보를 제거
				m_matParent.Right(m_matParent.Right().Normalize());
				m_matParent.Up(m_matParent.Up().Normalize());
				m_matParent.Forward(m_matParent.Forward().Normalize());
			}
			//else: 둘 다 상속 받는 경우에는 작업할 것이 없음. 그냥 빠져나가면 됨

			bWorldDirInherit = true;	//이때만 월드방향을 상속받아주면 된다.
		}
		else
		{
			//회전 미상속 + 크기 상속 -> 회전정보 제거
			if (true == m_bInheritScale)
			{
				//회전정보만 상속받는 경우: 크기정보만 추출
				Vec3 Scale(m_matParent.Right().Length(), m_matParent.Up().Length(), m_matParent.Forward().Length());
				//float(4) * 12 -> 회전 파트를 모두 0으로 밀어버리고 크기만 등록
				memset(m_matParent.m, 0, 48);
				m_matParent._11 = Scale.x;
				m_matParent._22 = Scale.y;
				m_matParent._33 = Scale.z;
			}
			//회전 미상속 + 크기 미상속 -> 전부 밀고 단위행렬로
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

