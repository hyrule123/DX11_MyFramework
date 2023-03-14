#pragma once

#include "CUI_Widget.h"

class CUI_MenuItem :
    public CUI_Widget
{
private:
    CUI_MenuItem() = delete;

public:
    CUI_MenuItem(const string& _strName, tPtrData _pData = tPtrData{});
    virtual ~CUI_MenuItem();

public:
    virtual bool beginUI() override;
    virtual void render_update() override;
    virtual void endUI() override {};

private:
    string m_strShortcutKeyBind;

    bool m_bCheckEnable;
    bool m_bChecked;
    bool m_bEnable;

    tPtrData m_pData;

    std::function<void(CUI_MenuItem*, tPtrData)> m_funcCallback;

    
public:
    void SetCheckEnable(bool _bCheckEnable) { m_bCheckEnable = _bCheckEnable; }

    bool SetEnable(bool _bEnable) { m_bEnable = _bEnable; }

    void SetItemChecked(bool _bChecked) { m_bChecked = _bChecked; }
    bool GetItemChecked() const { return m_bChecked; }

    void SetCallback(std::function<void(CUI_MenuItem*, tPtrData)> _pCallbackFunc);

};

