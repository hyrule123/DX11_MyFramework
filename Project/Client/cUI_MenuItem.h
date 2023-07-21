#pragma once

#include "cUI_Widget.h"

class cUI_MenuItem :
    public cUI_Widget
{
private:
    cUI_MenuItem() = delete;

public:
    cUI_MenuItem(const string& _strName, tPtrData _pData = tPtrData{});
    virtual ~cUI_MenuItem();

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

    std::function<void(cUI_MenuItem*, tPtrData)> m_funcCallback;

    
public:
    void SetCheckEnable(bool _bCheckEnable) { m_bCheckEnable = _bCheckEnable; }

    bool SetEnable(bool _bEnable) { m_bEnable = _bEnable; }

    void SetItemChecked(bool _bChecked) { m_bChecked = _bChecked; }
    bool GetItemChecked() const { return m_bChecked; }

    void SetCallback(std::function<void(cUI_MenuItem*, tPtrData)> _pCallbackFunc);

};

