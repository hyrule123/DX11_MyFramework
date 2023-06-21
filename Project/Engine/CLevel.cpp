#include "pch.h"
#include "CLevel.h"



#include "CGameObject.h"

//에디터 모드/게임모드 카메라 전환용
#include "CRenderMgr.h"

CLevel::CLevel()
	: m_arrLayer{}
	, m_CurState()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].SetLayerIdx(i);
	}
}

CLevel::~CLevel()
{
}


void CLevel::tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].tick();
	}
}

void CLevel::finaltick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].finaltick();
	}
}


void CLevel::AddNewGameObj(CGameObject* _Object)
{
	AddNewGameObj_Recursive(_Object);
	_Object->init();
}

void CLevel::RemoveDestroyed()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].RemoveDestroyed();
	}
}

void CLevel::AddNewGameObj_Recursive(CGameObject* _pObj)
{
	//레이어 번호를 지정하지 않았을 경우 부모의 레이어를 따라간다.
	if (0 > _pObj->GetLayer())
	{
		CGameObject* pParent = _pObj->GetParent();
		assert(nullptr != pParent && 0 <= pParent->GetLayer() && pParent->GetLayer() < MAX_LAYER);
		_pObj->SetLayer(pParent->GetLayer());
	}

	//레이어에 삽입
	m_arrLayer[_pObj->GetLayer()].AddGameObject(_pObj);

	//자식 오브젝트들에 대해서 재귀적으로 수행
	const vector<CGameObject*>& vecObj = _pObj->GetvecChilds();
	size_t size = vecObj.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddNewGameObj_Recursive(vecObj[i]);
	}
}

void CLevel::SetLayerName(int _iLayer, const string_view _sLayerName)
{
	assert(0 <= _iLayer && _iLayer < MAX_LAYER);

	m_arrLayer[_iLayer].SetName(_sLayerName);
}

const string_view CLevel::GetLayerName(int _iLayer)
{
	assert(0 <= _iLayer && _iLayer < MAX_LAYER);

	return m_arrLayer[_iLayer].GetName();
}

int CLevel::GetLayerIdxByName(const string_view _sLayerName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (_sLayerName == m_arrLayer[i].GetName())
			return i;
	}

	//이름으로 못찾았을 경우 -1을 리턴
	return -1;
}

CGameObject* CLevel::FindObjectByName(const string_view _Name)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObj = m_arrLayer[i].GetvecObj();

		size_t size = vecObj.size();
		for (size_t j = 0; j < size; ++j)
		{
			if (_Name == vecObj[j]->GetName())
			{
				if (true == vecObj[j]->IsDestroyed())
					continue;
				return vecObj[j];
			}
				
		}
	}

	return nullptr;
}

void CLevel::FindObjectALLByName(const string_view _Name, vector<CGameObject*>& _out)
{
	_out.clear();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObj = m_arrLayer[i].GetvecObj();

		size_t size = vecObj.size();
		for (size_t j = 0; j < size; ++j)
		{
			if (true == vecObj[j]->IsDestroyed())
				continue;

			if (_Name == vecObj[j]->GetName())
				_out.push_back(vecObj[j]);
		}
	}
}

void CLevel::SetState(eLEVEL_STATE _eState)
{
	m_CurState = _eState;

	if (eLEVEL_STATE::PLAY == m_CurState)
	{
		CRenderMgr::GetInst()->ChangeRenderCam(true);
	}
	else
	{
		CRenderMgr::GetInst()->ChangeRenderCam(false);
	}
}

