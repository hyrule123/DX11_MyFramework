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

	//방금 구한 회전행렬으로 직관적 방향을 계산한다.
	//회전행렬을 따로 변수에 저장하지 않으므로 지역변수에 계산해놓은 시점에서 직관적 방향도 구해놓는다.
	//방법 1 - 행렬곱과 래핑함수를 사용
	m_vRelativeDir[eDIR_TYPE::eDIR_FRONT] = matRot.Forward();
	m_vRelativeDir[eDIR_TYPE::eDIR_RIGHT] = matRot.Right();
	m_vRelativeDir[eDIR_TYPE::eDIR_UP] = matRot.Up();

	//이동행렬
	const Matrix& matTranslation = Matrix::CreateTranslation(m_vRelativePos);

	//자신의 월드행렬 완성
	m_matWorld = matScale * matRot * matTranslation;


	//우선 단위행렬을 하나 만든 뒤
	static Matrix matInherit;
	//부모 오브젝트가 있을 경우 부모의 월드행렬을 받아온다. 
	//성공 시 true가 반환되므로 이 때는 상속 과정을 시작하면 됨
	bool bWorldDirInherit = false;
	if (true == GetOwner()->GetParentWorldMatrix(matInherit))
	{
		if (true == m_bInheritRot)
		{
			//회전 상속 + 크기 미상속 -> 크기정보 제거
			if (false == m_bInheritScale)
			{
				for (int i = 0; i < 3; ++i)
				{
					//정규화해서 크기정보를 제거
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
			//else: 둘 다 상속 받는 경우에는 작업할 것이 없음. 그냥 빠져나가면 됨

			bWorldDirInherit = true;	//이때만 월드방향을 상속받아주면 된다.
		}
		else
		{
			//회전 미상속 + 크기 상속 -> 회전정보 제거
			if (true == m_bInheritScale)
			{
				//회전정보만 상속받는 경우: 크기정보만 추출
				Vec3 Scale(matInherit.Right().Length(), matInherit.Up().Length(), matInherit.Forward().Length());
				//float(4) * 12 -> 회전 파트를 모두 0으로 밀어버리고 크기만 등록
				memset(matInherit.m, 0, 48);
				matInherit._11 = Scale.x;
				matInherit._22 = Scale.y;
				matInherit._33 = Scale.z;
			}
			//회전 미상속 + 크기 미상속 -> 전부 밀고 단위행렬로
			else
			{
				memset(matInherit.m, 0, 48);
				matInherit._11 = 1.f;
				matInherit._22 = 1.f;
				matInherit._33 = 1.f;
			}
		}

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
