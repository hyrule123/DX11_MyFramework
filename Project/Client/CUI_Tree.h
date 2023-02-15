#pragma once

#include "CUI_Widget.h"

class CUI_Tree :
    public CUI_Widget
{
private:
    CUI_Tree() = delete;
public:
    CUI_Tree(const string& _strUniqueName);
    virtual ~CUI_Tree();

public:
    virtual bool beginUI() override;
    virtual void endUI() override;

private:
    ImGuiTreeNodeFlags m_flagTree;

public:
    void SetTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree = _ImGuiTreeNodeFlags; }
    void AddTreeFlag(ImGuiTreeNodeFlags _ImGuiTreeNodeFlags) { m_flagTree |= _ImGuiTreeNodeFlags; }
    
    void AddChildNode(const string& _strName);


};

