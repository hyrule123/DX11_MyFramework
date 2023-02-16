#pragma once

#include "CUI_Widget.h"

class CUI_Tree :
    public CUI_Widget
{
private:
    CUI_Tree() = delete;
public:
    CUI_Tree(const string& _strUniqueName, DWORD_PTR _Value = (DWORD_PTR)0);
    virtual ~CUI_Tree();

public:
    virtual bool beginUI() override;

    virtual void render_update()

    virtual void endUI() override;

private:
    ImGuiTreeNodeFlags m_flagTree;

    DWORD_PTR          m_Value;

    std::function<void(CUI_Tree*, DWORD_PTR)> m_funcCallback;

public:
    void SetTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree = _ImGuiTreeNodeFlags; }
    void AddTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree |= _ImGuiTreeNodeFlags; }

    void AddChildNode(const string& _strName, DWORD_PTR _Value);

    void SetFuncCallback(std::function<void(CUI_Tree*, DWORD_PTR)> _FuncCallback) { m_funcCallback = _FuncCallback; }
    
};

