#pragma once

#include "CUI_Widget.h"

struct tMenuItem
{
    string Label;
    string KeyBind;
    bool bCheckEnable;
    bool bChecked;
    bool bEnable;
    std::function<void()> Callback;

    tMenuItem()
        : bCheckEnable()
        , bChecked()
        , bEnable(true)
        , Callback()
    {}
};

class CUI_MenuItemGroup :
    public CUI_BasicWindow
{
public:
    CUI_MenuItemGroup();
    virtual ~CUI_MenuItemGroup();

public:
    virtual bool beginUI() override;
    virtual void render_update() override;
    virtual void endUI() override;

private:
    vector<tMenuItem> m_vecItem;
    
public:
    void AddItem(const tMenuItem& _Item);
    bool SetCallback(const string& _sItemName, std::function<void()> _pCallbackFunc);
    bool SetEnable(const string& _sItemName, bool _bEnable);
    bool SetItemSelected(const string& _sItemName, bool _bChecked);
    bool GetItemSelected(const string& _sItemName);

private:
    tMenuItem* FindItem(const string& _sItemName);
};

