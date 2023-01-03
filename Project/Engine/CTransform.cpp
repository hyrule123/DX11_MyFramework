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

	m_matWorld = matScale * matRot * matTranslation;
}

void CTransform::UpdateData()
{
	//월드뷰투영행렬을 곱한 후 전치한다.(HLSL은 Column-Major Matrix, XMMATRIX에서는 Row-Major Matrix를 사용 중)
	Matrix matWVP = (m_matWorld * g_transform.MatView * g_transform.MatProj).Transpose();

	//위의 행렬을 상수버퍼에 전달 및 바인딩
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(eCB_TYPE::TRANSFORM);
	pTransformBuffer->SetData(&matWVP, sizeof(Matrix));
	pTransformBuffer->UpdateData();
}
