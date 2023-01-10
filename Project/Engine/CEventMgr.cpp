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

void CEventMgr::tick()
{
	while (false == m_queueEvent.empty())
	{
		const tEvent& event = m_queueEvent.front();
		
		switch (event.Type)
		{
		case eEVENT_TYPE::CREATE_OBJECT:
		{
			//wParam = CGameObject Pointer
			//lParam = Layer Index
			CGameObject* Obj = reinterpret_cast<CGameObject*>(event.wParam);

			CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(Obj, (int)event.lParam);

			break;
		}



			
		case eEVENT_TYPE::DELETE_OBJECT:
			break;
		case eEVENT_TYPE::ADD_CHILD:
			break;
		case eEVENT_TYPE::DELETE_RESOURCE:
			break;
		case eEVENT_TYPE::LEVEL_CHANGE:
			break;
		default:
			break;
		}


		m_queueEvent.pop();
	}

	



}
