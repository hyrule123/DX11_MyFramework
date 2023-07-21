#pragma once
#include "cUI_Widget.h"




class CUI_DragNDropSender :
    public cUI_Widget
{
public:
    CUI_DragNDropSender(const string& _strKeySend);
    virtual ~CUI_DragNDropSender();

public:
    virtual bool beginUI() override;
    virtual void render_update() override;
    virtual void endUI() override;

private:
    ImGuiDragDropFlags m_flag;
    string m_strKeySend;

public:
    void SetKeySend(const string& _strKeySend) { m_strKeySend = _strKeySend; }
    void SetFlag(ImGuiDragDropFlags _flag) { m_flag = _flag; }
    void AddFlag(ImGuiDragDropFlags _flag, bool _bOnOff);
};









class cUI_DragNDropReceiver :
    public cUI_Widget
{
public:
    cUI_DragNDropReceiver(const string& _strKeyReceive);
    virtual ~cUI_DragNDropReceiver();

public:
    virtual bool beginUI() override;
    virtual void render_update() override;
    virtual void endUI() override;

private:
    string m_strKeyReceive;

public:
    void SetKeyReceive(const string& _strKeyReceive) { m_strKeyReceive = _strKeyReceive; }
};










inline void CUI_DragNDropSender::AddFlag(ImGuiDragDropFlags _flag, bool _bOnOff)
{
    if (true == _bOnOff)
    {
        m_flag |= _flag;
    }
    else
    {
        m_flag &= ~_flag;
    }
}

