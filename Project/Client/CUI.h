#pragma once

#include <Engine/CEntity.h>

#include "ImGui/imgui.h"


class CUI :
    public CEntity
{
public:
	CUI() = delete;
	CUI(const string& _Name);
	virtual ~CUI();
	CLONE_DISABLE(CUI)

public:
	//��ġ �̵��̳� ���� s����� ��ü���� �� ���� �۾��� ���⼭ ����
	void tickRecursive();
	void finaltick();


	virtual void init() {}
	virtual void tick() {}
	

	//�������ؼ� ���� UI�� �´� begin�Լ��� ȣ��. bool ���� ��ȯ�Ǹ�, true�� ��ȯ�Ǿ��� ������ endUI()�� ȣ���.
	//�׷��Ƿ� ������ endUI()�� ȣ���ϰ� ���� ��쿡�� true�� �������� ��ȯ������ �� 
	//bool���� ��ȯ ���ο� ���� endUI() ȣ�� ���ΰ� �޶����� IMGUI �Լ�(ex.ComboBox)�� ��쿡��
	//return ������ �ش� �Լ��� ȣ���ϸ� �ȴ�.
	virtual bool beginUI() = 0;

	//���⿡�� Ŭ�������� Ư���ϰ� ȣ���ؾ��� ���̾ƿ��� ���� ��� �������ؼ� ����ϸ� ��.
	virtual void render_update() {}

	//�������ؼ� ���� UI�� �´� end�Լ��� ȣ��
	virtual void endUI() = 0;


private:
	string			m_strID;
	bool			m_Active;		// UI Ȱ��ȭ üũ

	CUI*			m_ParentUI;		// �θ� UI
	vector<CUI*>	m_vecChildUI;	// �ڽ� UI ���
	

public:
	void SetActive(bool _Active) { m_Active = _Active; }
	bool GetActive() { return m_Active; }
	bool* GetActivePtr() { return &m_Active; }
	bool ToggleActive() { m_Active = !m_Active; return m_Active; }

	//�ڽ��� ���� ID�ѹ��� �ڿ� ���ؼ� ���� ID ����(ID�� â�� ���� ����)
	void SetstrID(const string& _Name); 
	const string& GetstrID() const { return m_strID; }

	CUI* GetParent() const { return m_ParentUI; }
	void AddChildUI(CUI* _UI);
	CUI* FindChildUIByName(const string& _Name);
};

inline void CUI::SetstrID(const string& _Name)
{
	CEntity::SetName(_Name); 
	m_strID = _Name;
	m_strID += "##";
	m_strID += std::to_string(GetID());
}
