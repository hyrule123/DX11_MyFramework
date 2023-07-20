#include "pch.h"
#include "cLevel.h"



#include "cGameObject.h"

//에디터 모드/게임모드 카메라 전환용
#include "cRenderMgr.h"

cLevel::cLevel()
	: m_arrLayer{}
	, m_CurState()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].SetLayerIdx(i);
	}
}

cLevel::~cLevel()
{
}


void cLevel::Tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].Tick();
	}
}

void cLevel::FinalTick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].FinalTick();
	}
}


void cLevel::AddNewGameObj(cGameObject* _Object)
{
	AddNewGameObj_Recursive(_Object);
	_Object->Init();
}

void cLevel::RemoveDestroyed()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].RemoveDestroyed();
	}
}

void cLevel::AddNewGameObj_Recursive(cGameObject* _pObj)
{
	//레이어 번호를 지정하지 않았을 경우 부모의 레이어를 따라간다.
	if (0 > _pObj->GetLayer())
	{
		cGameObject* pParent = _pObj->GetParent();
		assert(nullptr != pParent && 0 <= pParent->GetLayer() && pParent->GetLayer() < MAX_LAYER);
		_pObj->SetLayer(pParent->GetLayer());
	}

	//레이어에 삽입
	m_arrLayer[_pObj->GetLayer()].AddGameObject(_pObj);

	//자식 오브젝트들에 대해서 재귀적으로 수행
	const vector<cGameObject*>& vecObj = _pObj->GetvecChilds();
	size_t size = vecObj.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddNewGameObj_Recursive(vecObj[i]);
	}
}

void cLevel::SetLayerName(int _iLayer, const string_view _sLayerName)
{
	assert(0 <= _iLayer && _iLayer < MAX_LAYER);

	m_arrLayer[_iLayer].SetKey(_sLayerName);
}

const string_view cLevel::GetLayerName(int _iLayer)
{
	assert(0 <= _iLayer && _iLayer < MAX_LAYER);

	return m_arrLayer[_iLayer].GetKey();
}

int cLevel::GetLayerIdxByName(const string_view _sLayerName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (_sLayerName == m_arrLayer[i].GetKey())
			return i;
	}

	//이름으로 못찾았을 경우 -1을 리턴
	return -1;
}

cGameObject* cLevel::FindObjectByName(const string_view _Name)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<cGameObject*>& vecObj = m_arrLayer[i].GetvecObj();

		size_t size = vecObj.size();
		for (size_t j = 0; j < size; ++j)
		{
			if (_Name == vecObj[j]->GetKey())
			{
				if (true == vecObj[j]->IsDestroyed())
					continue;
				return vecObj[j];
			}
				
		}
	}

	return nullptr;
}

void cLevel::FindObjectALLByName(const string_view _Name, vector<cGameObject*>& _out)
{
	_out.clear();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<cGameObject*>& vecObj = m_arrLayer[i].GetvecObj();

		size_t size = vecObj.size();
		for (size_t j = 0; j < size; ++j)
		{
			if (true == vecObj[j]->IsDestroyed())
				continue;

			if (_Name == vecObj[j]->GetKey())
				_out.push_back(vecObj[j]);
		}
	}
}

void cLevel::SetState(eLEVEL_STATE _eState)
{
	m_CurState = _eState;

	if (eLEVEL_STATE::PLAY == m_CurState)
	{
		cRenderMgr::GetInst()->ChangeRenderCam(true);
	}
	else
	{
		cRenderMgr::GetInst()->ChangeRenderCam(false);
	}
}

