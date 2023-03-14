#pragma once

#include "CUI_Widget.h"


class CUI_Tree :
    public CUI_Widget
{
private:
    CUI_Tree() = delete;
public:
    CUI_Tree(const string& _strName);
    virtual ~CUI_Tree();

public:

    virtual bool beginUI() override;

    virtual void endUI() override;

private:
    ImGuiTreeNodeFlags m_flagTree;

    std::function<void(tUIData)> m_funcCallback[(int)eUI_MOUSE_STATUS::END];

public:
    void SetTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree = _ImGuiTreeNodeFlags; }
    void AddTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree |= _ImGuiTreeNodeFlags; }
    void RemoveTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree &= ~(_ImGuiTreeNodeFlags); }

    void SetSelected(bool _bIsSet);

    void AddChildNode(const string& _strName, tPtrData _pData);
    void AddChildNode(CUI_Tree* _pTree);

    void SetFuncCallback(eUI_MOUSE_STATUS _eMouseStatus, std::function<void(tUIData)> _FuncCallback) { m_funcCallback[(int)_eMouseStatus] = _FuncCallback; }
    
private:
    void CheckCallback();
};

inline void CUI_Tree::SetSelected(bool _bIsSet)
{
    if (true == _bIsSet)
    {
        m_flagTree |= ImGuiTreeNodeFlags_Selected;
    }
    else
    {
        m_flagTree &= ~ImGuiTreeNodeFlags_Selected;
    }
}
