#pragma once

#include "ImGui/imgui.h"

#include "CImGuiMgr.h"

class CUI_Widget;

//UI_Interface 필터의 항목들은 말그대로 순수가상함수로 인터페이스를 구상하는 데에만 사용함.

//1. 생성자 : UI의 동적할당 진행
//2. Init : UI간 연결 진행
//3. tick : UI에서 처리해야 하는 각종 작업 진행. 창이 떠있지 않아도 작동해야할 작업들은 여기서
//4. finaltick : 인터페이스 구상용. beginOOO ~ endOOO 등의 레이아웃 인터페이스 구상(UI_Interface 필터)
//5. render_update : 실제로 인스턴스로 생성될 클래스에서 재정의. UI의 구체적인 레이아웃 설정.

class CUI
{
private:
	CUI() = delete;
public:
	CUI(const string& _ID);
	virtual ~CUI();

public:
	//생성자에서 UI들이 전부 생성되어 등록되고 나면 init()에서 서로 간 관계(함수 포인터 등 콜백 함수)를 정의
	void initRecursive();
	//위치 이동이나 내부 연결된 객체들의 값 조정 작업은 여기서 진행
	void tickRecursive();

	virtual void init() {}
	virtual void tick() {}

	//설정된 레이아웃 배치 작업은 여기서 진행. 아래의 render_update를 반드시 호출해줘야 한다.
	//finaltick()은 위젯에 따라서 아예 다른 방식으로 정의해야할 때도 있기 때문에 virtual 형태로 둔다.
	virtual void finaltick();

	//여기서 자세한 레이아웃을 구현. 반드시 작성해야 함.
	virtual int render_update() = 0;

private:
	string			m_strName;		// 타이틀 이름, 
	string			m_strID;		// ID 이름

	ImVec2			m_vSize;		// UI 크기
	ImVec2			m_vPopupPos;	// UI 위치

	CUI*			m_ParentUI;		// 부모 UI
	vector<CUI*>	m_vecChildUI;	// 자식 UI 목록

	bool			m_Modal;		// 모달 or 모달리스
	bool			m_Active;		// UI 활성화 체크

public:
	void SetActive(bool _Active) { m_Active = _Active; }
	bool IsActive() { return m_Active; }

	void SetModal(bool _Modal) { m_Modal = _Modal; }
	bool IsModal() { return m_Modal; }

	ImVec2 GetPopupPos() { return m_vPopupPos; }
	void SetPopupPos(ImVec2 _vPos) { m_vPopupPos = _vPos; }

	const string& GetName() { return m_strName; }
	void SetName(const string& _Name) { m_strName = _Name; }

	const string& GetID() { return m_strID; }

	CUI* GetParent() const { return m_ParentUI; }
	void SetSize(float _width, float _height) { m_vSize = ImVec2(_width, _height); }
	ImVec2 GetSize() const { return m_vSize; }


	void AddChildUI(CUI* _UI);

	CUI* FindChildUIByID(const string& _ID);
	CUI* FindChildUIByName(const string& _Name);


protected:
	void LoopChildFinaltick();
};

inline void CUI::AddChildUI(CUI* _UI)
{
	_UI->m_ParentUI = this;
	m_vecChildUI.push_back(_UI);
	
	CImGuiMgr::GetInst()->CreateUI(_UI);
}



inline CUI* CUI::FindChildUIByID(const string& _ID)
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_ID == m_vecChildUI[i]->GetID())
			return m_vecChildUI[i];
	}

	return nullptr;
}

inline CUI* CUI::FindChildUIByName(const string& _Name)
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_Name == m_vecChildUI[i]->GetName())
			return m_vecChildUI[i];
	}

	return nullptr;
}



inline void CUI::LoopChildFinaltick()
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (false == m_vecChildUI[i]->IsActive())
			continue;

		//자식 UI를 자신의 UI 위쪽에 렌더링한다.
		m_vecChildUI[i]->finaltick();
	}

	ImGui::Separator();
}
