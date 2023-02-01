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

    tMenuItem() = delete;
    tMenuItem(const string& _sLabel)
        : Label(_sLabel)
        , bCheckEnable(false)
        , bChecked(false)
        , bEnable(true)
        , Callback()
    {}
};

class CUI_MenuItemGroup :
    public CUI
{
public:
    CUI_MenuItemGroup();
    virtual ~CUI_MenuItemGroup();

public:
    virtual void finaltick() override;
    virtual int render_update() override;

private:
    vector<tMenuItem> m_vecItem;
    
public:
    void AddItem(const tMenuItem& _Item);
    bool SetCallback(const string& _sItemName, std::function<void()> _pCallbackFunc);
    bool SetEnable(const string& _sItemName, bool _bEnable);
    bool GetItemSelected(const string& _sItemName);

private:
    tMenuItem* FindItem(const string& _sItemName);
};

