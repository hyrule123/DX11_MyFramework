#pragma once
#include "cUI_Widget.h"
class cUI_Button :
    public cUI_Widget
{
    cUI_Button() = delete;
public:
    cUI_Button(const string& _strButtonName);// , ImVec2 _v2Size = ImVec2(0.f, 0.f));
    virtual ~cUI_Button();

public:
    virtual bool beginUI() override;
    virtual void render_update() override;
    virtual void endUI() override {};

private:
    std::function<void(const tUIData&)> m_funcCallback;
    ImVec2 m_vSize;

public:
    void SetFuncCallback(std::function<void(const tUIData&)> _funcCallback) { m_funcCallback = _funcCallback; }
    void SetSize(ImVec2 _vSize) { m_vSize = _vSize; }
};

