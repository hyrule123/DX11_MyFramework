#include "pch.h"
#include "CUI_Tree.h"

CUI_Tree::CUI_Tree(const string& _strUniqueName)
    : CUI_Widget("Tree", eWIDGET_TYPE::TREE)
    , m_flagTree()
{
    MakeUniqueID(_strUniqueName);
}

CUI_Tree::~CUI_Tree()
{
}

bool CUI_Tree::beginUI()
{
    return ImGui::TreeNodeEx(GetStrID().c_str(), m_flagTree);
}

void CUI_Tree::endUI()
{
    ImGui::TreePop();
}

void CUI_Tree::AddChildNode(const string& _strName)
{
    CUI_Tree* pTree = new CUI_Tree(_strName);

    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;

    pTree->SetImGuiWindowFlags(flag);

    AddChildUI(pTree);
}
