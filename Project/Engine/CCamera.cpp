#include "pch.h"
#include "CCamera.h"

//카메라의 위치를 얻어오기 위함
#include "CTransform.h"

//화면 해상도 얻어오기 위함
#include "CDevice.h"

CCamera::CCamera():
	CComponent(eCOMPONENT_TYPE::CAMERA)
	, m_AspectRatio()
{
}

CCamera::~CCamera()
{
}


void CCamera::init()
{
	//===========
	//투영 -> 사실 이건 한번만 구해줘도 됨
	//===========

	//1. 투영 행렬 생성
	const Vec2& Resolution = CDevice::GetInst()->GetRenderResolution();
	m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);

	//1-1. 원근 투영행렬(체험용)
	m_AspectRatio = Resolution.x / Resolution.y;
	//m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);

	//2. 업데이트
	g_transform.MatProj = m_matProj;
}

void CCamera::finaltick()
{
	const Vec3& vCamPos = Transform()->GetRelativePos();

	//뷰행렬 = 카메라 앞으로 월드행렬의 물체들을 끌어오는 작업.
	//도로 끌어오는 작업이므로 월드행렬에 배치했던 순서의 역순으로 작업을 해주면 된다.
	//1. 위치
	m_matView = Matrix::CreateTranslation(-vCamPos);

	//2. 회전
	const Vec3& vecRot = Transform()->GetRelativeRot();
	const XMVECTOR& vecQut = XMQuaternionRotationRollPitchYawFromVector(-vecRot);
	m_matView *= Matrix::CreateFromQuaternion(vecQut);

	//3. 크기
	


	//4. 업데이트
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


