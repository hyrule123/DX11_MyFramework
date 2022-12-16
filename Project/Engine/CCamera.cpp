#include "pch.h"
#include "CCamera.h"

//ī�޶��� ��ġ�� ������ ����
#include "CTransform.h"

//ȭ�� �ػ� ������ ����
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
	//���� -> ��� �̰� �ѹ��� �����൵ ��
	//===========

	//1. ���� ��� ����
	const Vec2& Resolution = CDevice::GetInst()->GetRenderResolution();
	m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);

	//1-1. ���� �������(ü���)
	m_AspectRatio = Resolution.x / Resolution.y;
	//m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);

	//2. ������Ʈ
	g_transform.MatProj = m_matProj;
}

void CCamera::finaltick()
{
	const Vec3& vCamPos = Transform()->GetRelativePos();

	//����� = ī�޶� ������ ��������� ��ü���� ������� �۾�.
	//���� ������� �۾��̹Ƿ� ������Ŀ� ��ġ�ߴ� ������ �������� �۾��� ���ָ� �ȴ�.
	//1. ��ġ
	m_matView = Matrix::CreateTranslation(-vCamPos);

	//2. ȸ��
	const Vec3& vecRot = Transform()->GetRelativeRot();
	const XMVECTOR& vecQut = XMQuaternionRotationRollPitchYawFromVector(-vecRot);
	m_matView *= Matrix::CreateFromQuaternion(vecQut);

	//3. ũ��
	


	//4. ������Ʈ
	g_transform.MatView = m_matView;


	////===========
	////���� -> ��� �̰� �ѹ��� �����൵ �� -> init()���� �̵���
	////===========
	//
	////1. ���� ��� ����
	//const Vec2& Resolution = CDevice::GetInst()->GetRenderResolution();
	////m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);

	////1-1. ���� �������(ü���)
	//m_AspectRatio = Resolution.x / Resolution.y;
	//m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
	//
	////2. ������Ʈ
	//g_transform.MatProj = m_matProj;
	
}


