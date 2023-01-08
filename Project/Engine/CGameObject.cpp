#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CMeshRender.h"
#include "CScriptHolder.h"

//���̾� �ű� �� ���
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

//Ʈ������ ���
#include "CTransform.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_initalized()
	, m_RenderCom()
	, m_Parent()
	, m_iLayerIdx(-1)
	, m_bFixLayer()
{
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
	m_initalized = true;

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
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::SCRIPT; ++i)
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
	if (nullptr == m_RenderCom)
		return;

	m_RenderCom->render();
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
	case eCOMPONENT_TYPE::MESHRENDER:
	case eCOMPONENT_TYPE::PARTICLESYSTEM:
	case eCOMPONENT_TYPE::TILEMAP:
	case eCOMPONENT_TYPE::LANDSCAPE:
	case eCOMPONENT_TYPE::DECAL:
		//m_RenderCom�� �ϳ� �̻��� Render ������Ʈ�� �� ���� ��� ���� �߻���Ŵ.
		assert(nullptr == m_RenderCom);
		m_RenderCom = static_cast<CRenderComponent*>(_Component);
		break;

	case eCOMPONENT_TYPE::SCRIPT:
		break;
	default:
		break;
	}

	//������ �ּҸ� ���.
	_Component->m_pOwner = this;
	m_arrCom[ComType] = _Component;

	//�̹� �۵����� ��� �ٷ� init() ȣ��
	if(m_initalized)
		_Component->init();
}

void CGameObject::AddScript(CScript* _Script)
{
	if (nullptr == _Script)
		return;

	CScriptHolder* pScriptHolder = static_cast<CScriptHolder*>(m_arrCom[(UINT)eCOMPONENT_TYPE::SCRIPT]);
	if (nullptr == pScriptHolder)
	{
		pScriptHolder = new CScriptHolder;
		AddComponent(pScriptHolder);
	}
	pScriptHolder->AddScript(_Script);
}

void CGameObject::AddChild(CGameObject* _Object)
{
	if (nullptr != (_Object->m_Parent))
		_Object->m_Parent->RemoveChild(_Object);

	_Object->m_Parent = this;
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
			_Object->m_Parent = nullptr;
			return;
		}
		++iter;
	}
}


void CGameObject::AddAllHierarchyObjects(int _iLayerIdx, list<CGameObject*>& _listObj)
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
		_listObj.push_back(this);
	}
	
	//�� ������ �ݴ��
	//m_iLayerIdx�� 1�� �ƴϰ�(&&) m_bFixLayer�� true �϶� �̹Ƿ� �� ���� ���̾� �̵��� ������ ���ϴ� ����̴�.
	
	//�ڽĵ��� ������ε� ��������� ȣ��
	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddAllHierarchyObjects(_iLayerIdx, _listObj);
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

