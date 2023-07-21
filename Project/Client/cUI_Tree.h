#pragma once

#include "cUI_Widget.h"


class cUI_Tree :
    public cUI_Widget
{
private:
    cUI_Tree() = delete;
public:
    cUI_Tree(const string& _strName);
    virtual ~cUI_Tree();

public:

    virtual bool beginUI() override;

    virtual void endUI() override;

private:
    ImGuiTreeNodeFlags m_flagTree;

    std::function<void(const tUIData&)> m_funcCallback[(int)eUI_MOUSE_STATUS::END];

public:
    void SetTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree = _ImGuiTreeNodeFlags; }
    void AddTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree |= _ImGuiTreeNodeFlags; }
    void RemoveTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree &= ~(_ImGuiTreeNodeFlags); }

    void SetSelected(bool _bIsSet);

    void AddChildNode(const string& _strName, tPtrData _pData);
    void AddChildNode(cUI_Tree* _pTree);

    void SetFuncCallback(eUI_MOUSE_STATUS _eMouseStatus, std::function<void(const tUIData&)> _FuncCallback) { m_funcCallback[(int)_eMouseStatus] = _FuncCallback; }
    
private:
    void CheckCallback();
};

inline void cUI_Tree::SetSelected(bool _bIsSet)
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
