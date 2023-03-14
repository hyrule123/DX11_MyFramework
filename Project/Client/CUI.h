#pragma once

#include "CUI_Structs.h"

#include <Engine/CEntity.h>

#include "ImGui/imgui.h"

#include <UtilLib_DLL/json/forwards.h>

//uDataSize == 0 -> Class Pointer
//uDataSize > 0 -> Data Start Point


//여러 개의 창이 생성될 가능성이 있는 UI의 경우
//IMGUI.ini에 저장하지 않도록 설정하고, 저장해야 할 경우 json 파일을 통해서 설정값을 저장할것

class CUI
	: public CEntity
{
public:
	CUI() = delete;
	CUI(const string& _Name);
	virtual ~CUI();
	CLONE_DISABLE(CUI)

public:
	//위치 이동이나 내부 연결된 객체들의 값 조정 작업은 여기서 진행
	void tickRecursive();
	void finaltick();


	virtual void init() {}
	virtual void tick() {}
	

	//재정의해서 각자 UI에 맞는 begin함수를 호출. bool 값이 반환되며, true가 반환되었을 때에만 endUI()가 호출됨.
	//그러므로 무조건 endUI()를 호출하고 싶을 경우에는 true를 고정으로 반환시켜줄 것 
	//bool값을 반환 여부에 따라 endUI() 호출 여부가 달라지는 IMGUI 함수(ex.ComboBox)의 경우에는
	//return 값으로 해당 함수를 호출하면 된다.
	virtual bool beginUI() = 0;

	//여기에는 클래스에서 특별하게 호출해야할 레이아웃이 있을 경우 재정의해서 사용하면 됨.
	virtual void render_update() {}

	//재정의해서 각자 UI에 맞는 end함수를 호출
	virtual void endUI() = 0;

	void SaveRecursive(Json::Value& _Node);
	void LoadRecursive(Json::Value& _Node);



private:
	string			m_strID;		//고유 ID. 중복되지 않음.
	

	CUI*			m_ParentUI;		// 부모 UI
	vector<CUI*>	m_vecChildUI;	// 자식 UI 목록
	bool			m_bActive;		// UI 활성화 체크
	bool			m_bNoChildUI;	// 자식 노드가 들어갈 수 없는 노드로 설정
	
	tUIData			m_tData;

public:
	void SetActive(bool _Active) { m_bActive = _Active; }
	bool GetActive() { return m_bActive; }
	bool* GetActivePtr() { return &m_bActive; }
	bool ToggleActive() { m_bActive = !m_bActive; return m_bActive; }

	const tUIData& GetUIData() const { return m_tData; }
	tPtrData GetPtrData() const { return m_tData.tData; }
	void SetDataPtr(tPtrData _tData) { m_tData.tData = _tData; }


	//자신의 숫자 ID넘버를 뒤에 더해서 고유 ID 생성(ID는 창에 뜨지 않음)
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
