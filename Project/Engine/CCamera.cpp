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

//ī�޶� ���
#include "CRenderMgr.h"

CCamera::CCamera():
	CComponent(eCOMPONENT_TYPE::eCOMPONENT_CAMERA)
	, m_AspectRatio()
	, m_ProjectionType(ePROJ_TYPE::ORTHOGRAPHY)
	, m_CamIndex(-1)
	, m_LayerFlag(UINT32_MAX)
{
	SetProjType(m_ProjectionType);
}

CCamera::CCamera(const CCamera& _other)
	: CComponent(_other)
	, m_AspectRatio(_other.m_AspectRatio)
	, m_ProjectionType(_other.m_ProjectionType)
	, m_matView()	// finaltick()���� �� tick���� ��� ��
	, m_matProj(_other.m_matProj)
	, m_CamIndex(-1)
	, m_arrvecShaderDomain{}
	, m_LayerFlag(_other.m_LayerFlag)
{
}

CCamera::~CCamera()
{
}


void CCamera::SetProjType(ePROJ_TYPE _Type)
{
	m_ProjectionType = _Type;

	m_CamResolution = CDevice::GetInst()->GetRenderResolution();
	m_AspectRatio = m_CamResolution.x / m_CamResolution.y;

	switch (m_ProjectionType)
	{
	case ePROJ_TYPE::ORTHOGRAPHY:
		//===========
		//���� -> ��� �̰� �ѹ��� �����൵ ��
		//===========

		//1. ���� ��� ����
		m_matProj = XMMatrixOrthographicLH(m_CamResolution.x, m_CamResolution.y, 1.f, 10000.f);
		break;
	case ePROJ_TYPE::PERSPECTIVE:
		//1-1. ���� �������
		m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
		break;
	}

	//2. ������Ʈ - ī�޶� ���� �������� ����ǹǷ� ���� ������Ʈ �� �ʿ䰡 ����.
	//g_transform.MatProj = m_matProj;
}

void CCamera::SetCamIndex(eCAMERA_INDEX _Idx)
{
	assert(_Idx <= eCAMIDX_END);

	m_CamIndex = _Idx;
	CRenderMgr::GetInst()->RegisterCamera(this, _Idx);
}



void CCamera::Zoom2D(float _fScale)
{
	if (ePROJ_TYPE::ORTHOGRAPHY != m_ProjectionType)
		return;

	m_CamResolution *= _fScale;

	//���μ��θ� ���� ������ Ȯ��/����ϹǷ� AspecRatio�� ������ ����.
	m_matProj = XMMatrixOrthographicLH(m_CamResolution.x, m_CamResolution.y, 1.f, 10000.f);
}


void CCamera::init()
{
	//�⺻ �������� ��������� ����
	SetProjType(m_ProjectionType);
}

void CCamera::finaltick()
{
	Vec3 vCamPos = Transform()->GetWorldPos();

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

	//�ڽŵ� ȭ�鿡 ������ �ϱ� ���ؼ� z������ �ణ �Ÿ��� �� �̵�
	vCamPos.z -= 10.f;
	m_matView = Matrix::CreateTranslation(-vCamPos);


	//2. ȸ��
	//ȸ���� �̵����� ������� ����� �ٸ����� ���� ���Ҽ� �ִ�.
	//�̴� ��������� ���� �����̴�.
	//��������� �� ��(�Ǵ� ��)���� ������ ����� ���Ѵ�.
	//�̷� ��������� ��ġ����� �ڽ��� ������� �Ǵ� Ư¡�� ������ �ִ�.
	//ȸ������� ��������̰�, ������� ���ȴ� �� �ٽ� ���󺹱ͽ�Ű�� ����̹Ƿ� 
	//ȸ������� �����ϸ� ���� ȸ������ �ݴ� �������� ���� �� �ְ� �ȴ�.
	//const Vec3& vecRot = Transform()->GetRelativeRot();
	//const XMVECTOR& vecQut = XMQuaternionRotationRollPitchYawFromVector(vecRot);
	//Matrix tempmat = Matrix::CreateFromQuaternion(vecQut);
	//m_matView *= tempmat.Transpose();

	const Matrix& matRot = Transform()->GetWorldRotMat();
	m_matView *= matRot.Transpose();

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
		const vector<CGameObject*>& objList = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i)->GetObjList();
		
		size_t size = objList.size();
		for (size_t i = 0; i < size; ++i)
		{
			//��� ��� ������Ʈ�� �޾ƿ´�.
			CRenderComponent* Com = objList[i]->GetRenderComponent();

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
			m_arrvecShaderDomain[(int)dom].push_back(objList[i]);
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


