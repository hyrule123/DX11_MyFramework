#pragma once

#include "CUI_Structs.h"

#include <Engine/CEntity.h>

#include "ImGui/imgui.h"

#include <UtilLib_DLL/json-forwards.h>

//uDataSize == 0 -> Class Pointer
//uDataSize > 0 -> Data Start Point


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
	//��ġ �̵��̳� ���� ����� ��ü���� �� ���� �۾��� ���⼭ ����
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
	string			m_strID;		//���� ID. �ߺ����� ����.
	

	CUI*			m_ParentUI;		// �θ� UI
	vector<CUI*>	m_vecChildUI;	// �ڽ� UI ���
	bool			m_bActive;		// UI Ȱ��ȭ üũ
	bool			m_bNoChildUI;	// �ڽ� ��尡 �� �� ���� ���� ����
	

public:
	void SetActive(bool _Active) { m_bActive = _Active; }
	bool GetActive() { return m_bActive; }
	bool* GetActivePtr() { return &m_bActive; }
	bool ToggleActive() { m_bActive = !m_bActive; return m_bActive; }

	//�ڽ��� ���� ID�ѹ��� �ڿ� ���ؼ� ���� ID ����(ID�� â�� ���� ����)
	void SetStrID(const string& _strID) { m_strID = _strID; }
	const string& GetStrID() const { return m_strID; }
	void AddStrID(const string& _strID) { m_strID += _strID; }
	void MakeUniqueID();
	void MakeUniqueID(const string& _strUniqueIDSuffix);

	CUI* GetParent() const { return m_ParentUI; }

	void AddChildUI(CUI* _UI);
	CUI* FindChildUIByName(const string& _Name);
	size_t GetChildNum() const { return m_vecChildUI.size(); }

	void ClearChildUI();

	const vector<CUI*>& GetVecChildUI() { return m_vecChildUI; }

private:
	virtual void Save(Json::Value& _Save) {};
	virtual void Load(Json::Value& _Load) {};
};
