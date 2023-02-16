#include "pch.h"
#include "CUI_Tree.h"

CUI_Tree::CUI_Tree(const string& _strUniqueName, DWORD_PTR _Value)
    : CUI_Widget("Tree", eWIDGET_TYPE::TREE)
    , m_flagTree()
    , m_Value(_Value)
    , m_funcCallback()
{
    MakeUniqueID(_strUniqueName);
    m_flagTree |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_FramePadding;
}

CUI_Tree::~CUI_Tree()
{
}

bool CUI_Tree::beginUI()
{
    ImGuiTreeNodeFlags Flag = 0;

    //ÀÚ½Ä 
    if (0 == GetChildNum())
    {
        Flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
    }
    else
    {
        Flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
    }
        
    Flag |= m_flagTree;

    return ImGui::TreeNodeEx(GetStrID().c_str(), Flag);
}

void CUI_Tree::endUI()
{
    ImGui::TreePop();
}

void CUI_Tree::AddChildNode(const string& _strName, DWORD_PTR _Value)
{
    CUI_Tree* pTree = new CUI_Tree(_strName, _Value);

    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
    pTree->AddTreeFlag(flag);

    AddChildUI(pTree);
}

void CUI_Tree::SetFuncCallback(std::function<void(CUI_Tree*, DWORD_PTR)> _FuncCallback)
{
}
