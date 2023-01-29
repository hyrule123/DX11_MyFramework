#pragma once

#include "ImGui/imgui.h"

class CWidget;

class CUI
{
private:
	CUI() = delete;
public:
	CUI(const string& _ID);
	virtual ~CUI();

private:
	string			m_strName;		// Ÿ��Ʋ �̸�, 
	string			m_strID;		// ID �̸�

	ImVec2			m_vSize;		// UI ũ��
	ImVec2			m_vPopupPos;	// UI ��ġ

	CUI*			m_ParentUI;		// �θ� UI
	vector<CUI*>	m_vecChildUI;	// �ڽ� UI ���

	bool			m_Modal;		// ��� or ��޸���
	bool			m_Active;		// UI Ȱ��ȭ üũ

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


public:
	//�����ڿ��� UI���� ���� �����Ǿ� ��ϵǰ� ���� init()���� ���� �� ����(�Լ� ������ �� �ݹ� �Լ�)�� ����
	void initRecursive();
	virtual void init() {}

	//��ġ �̵��̳� ���� ����� ��ü���� �� ���� �۾��� ���⼭ ����
	void tickRecursive();
	virtual void tick() {}
	
	//������ ���̾ƿ� ��ġ �۾��� ���⼭ ����. �Ʒ��� render_update�� ȣ���Ѵ�.
	//finaltick()�� ������ ���� �ƿ� �ٸ� ������� �����ؾ��� ���� �ֱ� ������ virtual ���·� �д�.
	virtual void finaltick();

	//���⼭ �ڼ��� ���̾ƿ��� ����. �ݵ�� �ۼ��ؾ� ��.
	virtual int render_update() = 0;

private:
	void LoopChildFinaltick();
};

inline void CUI::AddChildUI(CUI* _UI)
{
	_UI->m_ParentUI = this;
	m_vecChildUI.push_back(_UI);
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

		//�ڽ� UI�� �ڽ��� UI ���ʿ� �������Ѵ�.
		m_vecChildUI[i]->finaltick();

		//������ �ڽ��� �ƴ� ��� ���м��� ���� ������ �������ش�.
		if (i == (size - 1))
			ImGui::Separator();
	}
}
