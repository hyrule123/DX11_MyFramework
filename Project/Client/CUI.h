#pragma once

#include "ImGui/imgui.h"

class CUI
{
private:
	CUI() = delete;
public:
	CUI(const string& _ID);
	virtual ~CUI();

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

	void SetSize(float _width, float _height) { m_vSize = ImVec2(_width, _height); }

	void AddChildUI(CUI* _UI);


public:
	//위치 이동이나 내부 연결된 객체들의 값 조정 작업은 여기서 진행
	virtual void tick() {}
	
	//설정된 레이아웃 배치 작업은 여기서 진행.
	virtual void finaltick();

	//여기서 자세한 레이아웃을 구현. 반드시 작성해야 함.
	virtual int render_update() = 0;

private:
	void LoopChildFinaltick();
};

inline void CUI::AddChildUI(CUI* _UI)
{
	_UI->m_ParentUI = this;
	m_vecChildUI.push_back(_UI);
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

		//마지막 자식이 아닐 경우 구분선을 통해 영역을 구분해준다.
		if (i == (size - 1))
			ImGui::Separator();
	}
}
