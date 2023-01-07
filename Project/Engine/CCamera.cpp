#include "pch.h"
#include "CCamera.h"

//ī�޶��� ��ġ�� ������ ����
#include "CTransform.h"

//ȭ�� �ػ� ������ ����
#include "CDevice.h"

//���̾ �޾ƿ��� ����
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

//���̾� ���� �޾ƿͼ� �з���
#include "CRenderComponent.h"
#include "CMaterial.h"

CCamera::CCamera():
	CComponent(eCOMPONENT_TYPE::CAMERA)
	, m_AspectRatio()
	, m_ProjectionType(ePROJ_TYPE::ORTHOGRAPHY)
	, m_CamIndex(-1)
	, m_LayerFlag(UINT32_MAX)
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
		//���� -> ��� �̰� �ѹ��� �����൵ ��
		//===========

		//1. ���� ��� ����
		m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);
		break;
	case ePROJ_TYPE::PERSPECTIVE:
		//1-1. ���� �������
		m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
		break;
	}

	//2. ������Ʈ - ī�޶� ���� �������� ����ǹǷ� ���� ������Ʈ �� �ʿ䰡 ����.
	//g_transform.MatProj = m_matProj;
}

void CCamera::SetCamIndex(UINT _Idx)
{
	m_CamIndex = (int)_Idx;
}

void CCamera::init()
{
	//�⺻ �������� ��������� ����
	SetProjType(m_ProjectionType);
}

void CCamera::finaltick()
{
	const Vec3& vCamPos = Transform()->GetRelativePos();

	//����� = ī�޶� ������ ��������� ��ü���� ������� �۾�.
	//���� ������� �۾��̹Ƿ� ������Ŀ� ��ġ�ߴ� ������ �������� �۾��� ���ָ� �ȴ�.
	//�̵��� ȸ���� ������� �ǵ����� ����� Ư�� ����� �ٽ� ���� ���·� ������ ����̶�� �� �� �ִ�.
	//(���� ��ġ) * (�̵����) * (ȸ�����) = (���� ��ġ)
	//(���� ��ġ) * (�̵��� ȸ���� �ǵ����� ���) = (���� ��ġ)
	//�̰� ������� ���ǿ� ��ġ�Ѵ�.
	// I * X = X
	// X * X^(-1) = I
	//�׷��Ƿ� ī�޶��� ��ġ�� ȸ����ŭ �� �ִ� ����� ī�޶��� ��ġ��İ� ȸ������� ������� �����ָ� �ȴٴ� ���̴�.
	//(R * T)^(-1) = T^(-1) * R^(-1)
	//������� ���ϸ� ������ �ݴ�� ��. ������ ��
	//���� ������� ��ĸ��� ��������� ����� �ٸ���. � ����� ������� �������� ���� ���� �ִ�.
	//������� ���� ���δ� �Ǻ���(��İ� Det)���� �����ϴ�.
	//���庯ȯ����� ���� ������� �� �����ϹǷ� ����� �ʿ�� ����.


	//1. ��ġ�� ����� -> �׳� ������ŭ �̵����ָ��
	//x������ a, y������ b, z������ c��ŭ �̵��ߴٰ� �ϸ�
	//x������ -a, y������ -b, z������ -c��ŭ �̵����ָ� ��.
	/*
	1 0 0 0 
	0 1 0 0 
	0 0 1 0
	-a -b -c 1
	*/
	m_matView = Matrix::CreateTranslation(-vCamPos);


	//2. ȸ��
	//ȸ���� �̵����� ������� ����� �ٸ����� ���� ���Ҽ� �ִ�.
	//�̴� ��������� ���� �����̴�.
	//��������� �� ��(�Ǵ� ��)���� ������ ����� ���Ѵ�.
	//�̷� ��������� ��ġ����� �ڽ��� ������� �Ǵ� Ư¡�� ������ �ִ�.
	//ȸ������� ��������̰�, ������� ���ȴ� �� �ٽ� ���󺹱ͽ�Ű�� ����̹Ƿ� 
	//ȸ������� �����ϸ� ���� ȸ������ �ݴ� �������� ���� �� �ְ� �ȴ�.
	const Vec3& vecRot = Transform()->GetRelativeRot();
	const XMVECTOR& vecQut = XMQuaternionRotationRollPitchYawFromVector(vecRot);
	m_matView *= Matrix::CreateFromQuaternion(vecQut).Transpose();


	//3. transform ������� ����ü�� ������Ʈ -> ����. ���߿� render�� �ϰ������� view ��İ� proj ����� ���� ����.
	//g_transform.matViewProj = m_matView;


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


void CCamera::SortObject()
{
	for (UINT32 i = 0; i < MAX_LAYER; ++i)
	{
		UINT32 mask = (UINT32)1 << i;

		if (false == (mask & m_LayerFlag))
			continue;

		//ī�޶� ����ϰ��� �ϴ� ���̾��� ������Ʈ ����Ʈ�� �޾ƿͼ�
		const list<CGameObject*>& objList = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i)->GetObjList();

		//��ȸ �����ָ鼭
		for (auto& iter : objList)
		{
			//��� ��� ������Ʈ�� �޾ƿ´�.
			CRenderComponent* Com = iter->GetRenderComponent();

			//������Ʈ�� ���ų�, ������Ʈ ������ ��¿� Ŭ������ ��ϵǾ����� ���� ��� continue
			if (
				nullptr == Com
				||
				false == Com->GetRenderReady()
				)
				continue;
			
			//���̴� �������� �޾ƿͼ�
			eSHADER_DOMAIN dom = Com->GetMaterial()->GetShader()->GetShaderDomain();

			//���� ���̴� �������� ��ϵǾ����� ���� ��� assert ó��
			assert((int)dom < (int)eSHADER_DOMAIN_END);

			//�׷��� ���� ��� push back
			m_arrvecShaderDomain[(int)dom].push_back(&(*iter));
		}
	}
}

void CCamera::render()
{
	//���� ī�޶󺰷� �������� ����ǹǷ�, ī�޶� ������ �ִ� View ��İ� Proj ����� �̸� ���� ���´�.
	g_transform.matViewProj = m_matView * m_matProj;


	for (int i = 0; i < eSHADER_DOMAIN_END; ++i)
	{
		size_t size = m_arrvecShaderDomain[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			m_arrvecShaderDomain[i][j]->render();
		}

		//������ ��ȸ ������ ���� ����ֱ�.
		m_arrvecShaderDomain[i].clear();
	}
}


