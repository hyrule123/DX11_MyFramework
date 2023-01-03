#include "pch.h"
#include "CCamera.h"

//카메라의 위치를 얻어오기 위함
#include "CTransform.h"

//화면 해상도 얻어오기 위함
#include "CDevice.h"

CCamera::CCamera():
	CComponent(eCOMPONENT_TYPE::CAMERA)
	, m_AspectRatio()
	, m_ProjectionType(ePROJ_TYPE::ORTHOGRAPHY)
{
	SetProjType(m_ProjectionType);
}

CCamera::~CCamera()
{
}


void CCamera::SetProjType(ePROJ_TYPE _Type)
{
	m_ProjectionType = _Type;

	const Vec2& Resolution = CDevice::GetInst()->GetRenderResolution();
	m_AspectRatio = Resolution.x / Resolution.y;

	switch (m_ProjectionType)
	{
	case ePROJ_TYPE::ORTHOGRAPHY:
		//===========
		//투영 -> 사실 이건 한번만 구해줘도 됨
		//===========

		//1. 투영 행렬 생성
		m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);
		break;
	case ePROJ_TYPE::PERSPECTIVE:
		//1-1. 원근 투영행렬
		m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
		break;
	}

	//2. 업데이트
	g_transform.MatProj = m_matProj;
}

void CCamera::init()
{
	//기본 설정으로 투영행렬을 생성
	SetProjType(m_ProjectionType);
}

void CCamera::finaltick()
{
	const Vec3& vCamPos = Transform()->GetRelativePos();

	//뷰행렬 = 카메라 앞으로 월드행렬의 물체들을 끌어오는 작업.
	//도로 끌어오는 작업이므로 월드행렬에 배치했던 순서의 역순으로 작업을 해주면 된다.
	//이동과 회전을 원래대로 되돌리는 행렬은 특정 행렬을 다시 원래 상태로 돌리는 행렬이라고 볼 수 있다.
	//(로컬 위치) * (이동행렬) * (회전행렬) = (월드 위치)
	//(월드 위치) * (이동과 회전을 되돌리는 행렬) = (로컬 위치)
	//이건 역행렬의 정의와 일치한다.
	// I * X = X
	// X * X^(-1) = I
	//그러므로 카메라의 위치와 회전만큼 뺴 주는 행렬은 카메라의 위치행렬과 회전행렬의 역행렬을 곱해주면 된다는 뜻이다.
	//(R * T)^(-1) = T^(-1) * R^(-1)
	//역행렬을 곱하면 순서가 반대로 됨. 주의할 것
	//또한 역행렬은 행렬마다 만들어지는 방식이 다르다. 어떤 행렬은 역행렬이 존재하지 않을 수도 있다.
	//역행렬의 존재 여부는 판별식(행렬곱 Det)으로 가능하다.
	//월드변환행렬의 경우는 역행렬이 다 존재하므로 사용할 필요는 없음.


	//1. 위치의 역행렬 -> 그냥 음수만큼 이동해주면됨
	//x축으로 a, y축으로 b, z축으로 c만큼 이동했다고 하면
	//x축으로 -a, y축으로 -b, z축으로 -c만큼 이동해주면 됨.
	/*
	1 0 0 0 
	0 1 0 0 
	0 0 1 0
	-a -b -c 1
	*/
	m_matView = Matrix::CreateTranslation(-vCamPos);


	//2. 회전
	//회전은 이동과는 역행렬의 모습은 다르지만 쉽게 구할수 있다.
	//이는 직교행렬의 성질 덕분이다.
	//직교행렬은 각 행(또는 열)끼리 수직인 행렬을 말한다.
	//이런 직교행렬은 전치행렬이 자신의 역행렬이 되는 특징을 가지고 있다.
	//회전행렬은 직교행렬이고, 역행렬은 돌렸던 걸 다시 원상복귀시키는 행렬이므로 
	//회전행렬을 직교하면 현재 회전각의 반대 방향으로 돌릴 수 있게 된다.
	const Vec3& vecRot = Transform()->GetRelativeRot();
	const XMVECTOR& vecQut = XMQuaternionRotationRollPitchYawFromVector(vecRot);
	m_matView *= Matrix::CreateFromQuaternion(vecQut).Transpose();


	//3. transform 상수버퍼 구조체에 업데이트
	g_transform.MatView = m_matView;


	////===========
	////투영 -> 사실 이건 한번만 구해줘도 됨 -> init()으로 이동함
	////===========
	//
	////1. 투영 행렬 생성
	//const Vec2& Resolution = CDevice::GetInst()->GetRenderResolution();
	////m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);

	////1-1. 원근 투영행렬(체험용)
	//m_AspectRatio = Resolution.x / Resolution.y;
	//m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
	//
	////2. 업데이트
	//g_transform.MatProj = m_matProj;
	
}


