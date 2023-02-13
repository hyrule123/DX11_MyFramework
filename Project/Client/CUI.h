#pragma once

#include <Engine/CEntity.h>

#include "ImGui/imgui.h"

#include <UtilLib/json-forwards.h>


//���� ���� â�� ������ ���ɼ��� �ִ� UI�� ���
//IMGUI.ini�� �������� �ʵ��� �����ϰ�, �����ؾ� �� ��� json ������ ���ؼ� �������� �����Ұ�

class CUI
	: public CEntity
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

	void SaveRecursive(Json::Value& _Node);
	void LoadRecursive(Json::Value& _Node);



private:
	string			m_strName;
	bool			m_Active;		// UI Ȱ��ȭ üũ

	CUI*			m_ParentUI;		// �θ� UI
	vector<CUI*>	m_vecChildUI;	// �ڽ� UI ���
	

public:
	void SetActive(bool _Active) { m_Active = _Active; }
	bool GetActive() { return m_Active; }
	bool* GetActivePtr() { return &m_Active; }
	bool ToggleActive() { m_Active = !m_Active; return m_Active; }

	//�ڽ��� ���� ID�ѹ��� �ڿ� ���ؼ� ���� ID ����(ID�� â�� ���� ����)
	void SetName(const string& _Name) { m_strName = _Name; }
	const string& GetName() const { return m_strName; }
	void AddName(const string& _Name) { m_strName += _Name; }
	void MakeUniqueName();

	CUI* GetParent() const { return m_ParentUI; }
	void AddChildUI(CUI* _UI);
	CUI* FindChildUIByName(const string& _Name);

private:
	virtual void Save(Json::Value& _Node) {};
	virtual void Load(Json::Value& _Node) {};
};
