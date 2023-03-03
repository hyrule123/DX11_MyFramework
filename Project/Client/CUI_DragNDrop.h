#pragma once
#include "CUI_Widget.h"




class CUI_DragNDropSender :
    public CUI_Widget
{
public:
    CUI_DragNDropSender();
    virtual ~CUI_DragNDropSender();

public:
    virtual bool beginUI() override;
    virtual void render_update() override;
    virtual void endUI() override;

private:
    ImGuiDragDropFlags m_flag;
    string m_strKeySend;
    tDataPtr m_tData;


public:
    void SetKeySend(const string& _strKeySend) { m_strKeySend = _strKeySend; }
    void SetFlag(ImGuiDragDropFlags _flag) { m_flag = _flag; }
    void AddFlag(ImGuiDragDropFlags _flag, bool _bOnOff);
};









class CUI_DragNDropReceiver :
    public CUI_Widget
{
public:
    CUI_DragNDropReceiver();
    virtual ~CUI_DragNDropReceiver();

public:
    virtual bool beginUI() override;
    virtual void render_update() override;
    virtual void endUI() override;

private:
    string m_strKeyReceive;
    tDataPtr m_tData;

public:
    void SetKeyReceive(const string& _strKeyReceive) { m_strKeyReceive = _strKeyReceive; }

    bool IsReceived() const { return (nullptr != m_tData.pData); }
    tDataPtr GetData() const { return m_tData; }
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

