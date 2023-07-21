#pragma once

#include "cUI.h"

class cUI_Widget;

class cUI_BasicWindow
	: public cUI
{
private:
	cUI_BasicWindow() = delete;
public:
	cUI_BasicWindow(const string& _Name);
	virtual ~cUI_BasicWindow();

public:
	virtual void init() {}
	virtual void Tick() {}

	//재정의해서 각자 UI에 맞는 begin함수를 호출. bool 값이 반환되며, true가 반환되었을 때에만 endUI()가 호출됨.
	//그러므로 무조건 endUI()를 호출하고 싶을 경우에는 true를 고정으로 반환시켜줄 것 
	//bool값을 반환 여부에 따라 endUI() 호출 여부가 달라지는 IMGUI 함수(ex.ComboBox)의 경우에는
	//return 값으로 해당 함수를 호출하면 된다.
	virtual bool beginUI() override;

	//여기에는 클래스에서 특별하게 호출해야할 레이아웃이 있을 경우 재정의해서 사용하면 됨.
	virtual void render_update() override {}

	//재정의해서 각자 UI에 맞는 end함수를 호출
	virtual void endUI() override;

	//설정된 레이아웃 배치 작업은 여기서 진행. 아래의 render_update를 반드시 호출해줘야 한다.
	//FinalTick()은 위젯에 따라서 아예 다른 방식으로 정의해야할 때도 있기 때문에 virtual 형태로 둔다.
	virtual void Save(Json::Value& _Save) override;
	virtual void Load(Json::Value& _Load) override;

private:
	ImGuiWindowFlags m_WindowFlag;

	//Child
	bool			m_bBorder;
	ImVec2			m_vSize;		// UI 크기

public:
	void SetImGuiWindowFlags(ImGuiWindowFlags _uWindowFlag) { m_WindowFlag = _uWindowFlag; }
	void AddImGuiWindowFlags(ImGuiWindowFlags _uWindowFlag) { m_WindowFlag |= _uWindowFlag; }
	ImGuiWindowFlags GetImGuiWindowFlag() const { return m_WindowFlag; }

	void SetBorder(bool _b) { m_bBorder = _b; }

	void SetSize(float _width, float _height) { m_vSize = ImVec2(_width, _height); }
	ImVec2 GetSize() const { return m_vSize; }
	float GetWidth() const { return m_vSize.x; }
	float GetHeight() const { return m_vSize.y; }
	bool SizeSet() const { return (0.f == m_vSize.x) && (0.f == m_vSize.y); }
};
