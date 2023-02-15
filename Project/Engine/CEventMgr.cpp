#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CGameObject.h"


CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::CreateObject(const tEvent& _event)
{
	//lParam = CGameObject Pointer
	//rParam = Layer Index
	CGameObject* Obj = reinterpret_cast<CGameObject*>(_event.lParam);

	CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(Obj, (int)_event.rParam);
}

void CEventMgr::DestroyObject(const tEvent& _event)
{
	CGameObject* _pObj = reinterpret_cast<CGameObject*>(_event.lParam);

	//�̹� ���� ��� ���¿� �� �ִ� ��� �߰��� ������� ����.
	//�ߺ� ���� ����(��۸� ������ ����)
	if (true == _pObj->GetDestroyed())
		return;

	_pObj->DestroyForEventMgr();
	m_vecReserveDestroy.push_back(_pObj);
}

void CEventMgr::AddChildObj(const tEvent& _event)
{
	CGameObject* pParent = reinterpret_cast<CGameObject*>(_event.lParam);
	CGameObject* pChild = reinterpret_cast<CGameObject*>(_event.rParam);
	
	pParent->AddChildObj(pChild);
}

void CEventMgr::tick()
{
	
	//bDestroy ������ ���ӿ�����Ʈ�� Level���� ����
	CLevelMgr::GetInst()->GetCurLevel()->RemoveDestroyed();

	//�̺�Ʈ ť�� ����Ǳ� ���� ���� �����ӿ��� ��ϵ� ������Ʈ�� ����
	//�̷��� ���� �����ӿ��� �����ϵ��� ��ϵǾ��� ������Ʈ���� ���� �������� ��.
	size_t size = m_vecReserveDestroy.size();
	for (size_t i = 0; i < size; ++i)
	{
		delete m_vecReserveDestroy[i];
	}
	m_vecReserveDestroy.clear();


	size = m_vecEvent.size();
	for (size_t i = 0; i < size; ++i)
	{
		switch (m_vecEvent[i].Type)
		{
		case eEVENT_TYPE::CREATE_OBJECT:
			CreateObject(m_vecEvent[i]);
			break;
		case eEVENT_TYPE::DELETE_OBJECT:
			DestroyObject(m_vecEvent[i]);
			break;
		case eEVENT_TYPE::ADD_CHILD:
			AddChildObj(m_vecEvent[i]);
			break;
		case eEVENT_TYPE::DELETE_RESOURCE:
			break;
		case eEVENT_TYPE::LEVEL_CHANGE:
			break;
		default:
			break;
		}
	}
	m_vecEvent.clear();
}
