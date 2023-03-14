#pragma once
#include "CUI_Widget.h"
class CUI_Button :
    public CUI_Widget
{
    CUI_Button() = delete;
public:
    CUI_Button(const string& _strButtonName);
    virtual ~CUI_Button();

public:
    virtual bool beginUI() override;
    virtual void render_update() override;
    virtual void endUI() override {};

private:
    std::function<void(const tUIData&)> m_funcCallback;

public:
    void SetFuncCallback(std::function<void(const tUIData&)> _funcCallback) { m_funcCallback = _funcCallback; }

};

