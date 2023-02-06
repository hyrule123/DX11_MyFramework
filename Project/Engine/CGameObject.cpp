#include "pch.h"
#include "CGameObject.h"


#include "components.h"


//���̾� �ű� �� ���
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

//Ʈ������ ���
#include "CTransform.h"

//�����ֱ� ����
#include "CTimeMgr.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_bInitialized()
	, m_RenderCom()
	, m_Parent()
	, m_iLayerIdx(-1)
	, m_bFixLayer()
	, m_bDestroy()
	, m_fLifeSpan(FLT_MAX_NEG)
{
}

CGameObject::CGameObject(const CGameObject& _other)
	: CEntity(_other)
	, m_iLayerIdx(_other.m_iLayerIdx)
	, m_bFixLayer(_other.m_bFixLayer)
	, m_bInitialized(_other.m_bInitialized)
	, m_bDestroy()
	, m_fLifeSpan(FLT_MAX_NEG)
{
	//1. ������Ʈ ��� ����
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _other.m_arrCom[i])
		{
			m_arrCom[i] = _other.m_arrCom[i]->Clone();
			m_arrCom[i]->SetOwner(this);

			//1-1. ������ ������Ʈ �� ��� m_RenderCom�� ����
			if ((UINT)g_RenderComIdxStart <= i && i < (UINT)g_RenderComIdxEnd)
			{
				assert(nullptr == m_RenderCom);
				m_RenderCom = static_cast<CRenderComponent*>(m_arrCom[i]);
			}
		}
	}

	//2. �ڳ� ������Ʈ ����
	size_t size = _other.m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddChild(_other.m_vecChild[i]->Clone());
	}

}

CGameObject::~CGameObject()
{
	/*for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			delete m_arrCom[i];
	}*/

	Safe_Del_Array(m_arrCom);
}

void CGameObject::init()
{
	m_bInitialized = true;

	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->init();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->init();
	}

}

void CGameObject::tick()
{
	//�ڽ��� �ı� ��� ������ ��� �ڽŰ� ��� �ڽĵ鿡 ���� tick�� ó������ ����
	if (true == m_bDestroy)
		return;


	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	//�ڽ��� �ı� ��� ������ ��� �ڽŰ� ��� �ڽĵ鿡 ���� tick�� ó������ ����
	if (true == m_bDestroy)
	{
		//��ũ��Ʈ�� ������ ������Ʈ�鿡 ���� finaltick()�� ȣ���Ѵ�.
		cleanup();
		return;
	}
	else if (FLT_MAX_NEG != m_fLifeSpan)
	{
		m_fLifeSpan -= DELTA_TIME;
		if (m_fLifeSpan < 0.f)
		{
			DestroyObject(this);
			return;
		}
	}
		

	//��ũ��Ʈ�� ������ ������Ʈ�鿡 ���� finaltick()�� ȣ���Ѵ�.
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->finaltick();
	}
}

void CGameObject::render()
{
	//���� ��� ������ ��� �������� ���� ����.
	if (nullptr == m_RenderCom || true == m_bDestroy)
		return;

	m_RenderCom->render();
}

void CGameObject::cleanup()
{
	//������ ������Ʈ ����
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->cleanup();
	}

	//�ڽ� ������Ʈ�鵵 ��� cleanup
	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->cleanup();
	}
	
	//���� �ڽ��� �θ� ������Ʈ�� �����ϰ�, �ش� ������Ʈ�� ���� ��� ���°� �ƴ� ���
	//�غθ� ������Ʈ�� ���� ��� ������ ��쿡�� �θ� ������Ʈ�� ������ �̹� ������ �������� ������ �� ���̹Ƿ�
	//���� �ڽ��� �ּҸ� �����ش޶�� ��û���� �ʾƵ� �ȴ�.
	if (nullptr != m_Parent && false == m_Parent->GetDestroyed())
	{
		m_Parent->RemoveChild(this);
		m_Parent = nullptr;
	}
}

void CGameObject::AddComponent(CComponent* _Component)
{
	UINT ComType = (UINT)_Component->GetType();
	assert(nullptr == m_arrCom[ComType]);

	switch ((eCOMPONENT_TYPE)ComType)
	{
	case eCOMPONENT_TYPE::TRANSFORM:
	case eCOMPONENT_TYPE::COLLIDER2D:
	case eCOMPONENT_TYPE::COLLIDER3D:
	case eCOMPONENT_TYPE::ANIMATOR2D:
	case eCOMPONENT_TYPE::ANIMATOR3D:
	case eCOMPONENT_TYPE::LIGHT2D:
	case eCOMPONENT_TYPE::LIGHT3D:
	case eCOMPONENT_TYPE::CAMERA:
		break;

	//Render Components
	case eCOMPONENT_TYPE::MESH_RENDER:
	case eCOMPONENT_TYPE::PARTICLE_SYSTEM:
	case eCOMPONENT_TYPE::TILEMAP:
	case eCOMPONENT_TYPE::LANDSCAPE:
	case eCOMPONENT_TYPE::DECAL:
		//m_RenderCom�� �ϳ� �̻��� Render ������Ʈ�� �� ���� ��� ���� �߻���Ŵ.
		assert(nullptr == m_RenderCom);
		m_RenderCom = static_cast<CRenderComponent*>(_Component);
		break;

	case eCOMPONENT_TYPE::SCRIPT_HOLDER:
		break;
	default:
		break;
	}

	//������ �ּҸ� ���.
	_Component->m_pOwner = this;
	m_arrCom[ComType] = _Component;

	//�̹� �۵����� ��� �ٷ� init() ȣ��
	if(m_bInitialized)
		_Component->init();
}

void CGameObject::AddScript(CScript* _Script)
{
	if (nullptr == _Script)
		return;

	CScriptHolder* pScriptHolder = static_cast<CScriptHolder*>(m_arrCom[(UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER]);

	if (nullptr == pScriptHolder)
	{
		pScriptHolder = new CScriptHolder;
		AddComponent(pScriptHolder);
	}
	pScriptHolder->AddScript(_Script);
}

void CGameObject::AddChild(CGameObject* _Object)
{
	if (nullptr != (_Object->GetParent()))
		_Object->GetParent()->RemoveChild(_Object);

	_Object->SetParent(this);
	m_vecChild.push_back(_Object);
}

void CGameObject::RemoveChild(CGameObject* _Object)
{
	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();
	while (iter != iterEnd)
	{
		if (_Object == (*iter))
		{
			m_vecChild.erase(iter);
			return;
		}
		++iter;
	}
}


void CGameObject::AddAllHierarchyObjects(int _iLayerIdx, vector<CGameObject*>& _vecObj)
{
	//�ε����� ���� �������� �ʾ��� ��� -> ������ �ε��� ������ �ѹ� �������
	//���̾ �������� �ʾ��� ��� -> �θ� ������Ʈ�� ���̾ ����
	//���� �ϳ��� �ɸ��� �ϴ� �Űܾ���
	if (-1 == m_iLayerIdx || false == m_bFixLayer)
	{
		//���� ����� ���̾ ��ϵǾ� �־��ٸ� ���� ���̾�� �ڽ��� �����ؾ� �Ѵ�.
		if (m_iLayerIdx > 0)
		{
			CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx)->RemoveGameObject(this);
		}

		//�����ϰ� ���ο� ���̾ ���
		m_iLayerIdx = _iLayerIdx;
		_vecObj.push_back(this);
	}
	
	//�� ������ �ݴ��
	//m_iLayerIdx�� 1�� �ƴϰ�(&&) m_bFixLayer�� true �϶� �̹Ƿ� �� ���� ���̾� �̵��� ������ ���ϴ� ����̴�.
	
	//�ڽĵ��� ������ε� ��������� ȣ��
	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddAllHierarchyObjects(_iLayerIdx, _vecObj);
	}
}

void CGameObject::SetParentTransformUpdated()
{
	if (nullptr != Transform())
		Transform()->SetParentUpdate();

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetParentTransformUpdated();
	}
}

bool CGameObject::GetParentWorldMatrix(Matrix& _mat)
{
	//�θ� ������Ʈ�� ���ų� Ʈ�������� ���� ��� false ��ȯ
	if (nullptr == m_Parent || nullptr == m_Parent->Transform())
		return false;

	//���� ��� ���ڿ� �״�� ����, true ��ȯ.
	_mat = m_Parent->Transform()->GetWorldMat();
	return true;
}


void CGameObject::SetChildTransformToUpdate()
{
	if (nullptr != Transform())
		Transform()->SetParentUpdate();

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetParentTransformUpdated();
	}
}