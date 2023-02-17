#include "pch.h"
#include "CUI_Tree.h"

CUI_Tree::CUI_Tree(const string& _strName, DWORD_PTR _Value)
    : CUI_Widget(_strName, eWIDGET_TYPE::TREE)
    , m_flagTree()
    , m_pData(_Value)
    , m_funcCallback{}
{
    m_flagTree |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_FramePadding;
}

CUI_Tree::~CUI_Tree()
{
}

bool CUI_Tree::beginUI()
{
    ImGuiTreeNodeFlags Flag = 0;

    //자식 없을 경우(리프노드 일경우)
    if (0 == GetChildNum())
    {
        Flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
    }
    //자식 있을 경우
    else
    {
        Flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
    }
        
    Flag |= m_flagTree;

    bool IsOpen = ImGui::TreeNodeEx(GetStrID().c_str(), Flag);

    //열려있던 닫혀있던 일단 콜백함수 체크 및 호출
    CheckCallback();

    return IsOpen;
}


void CUI_Tree::endUI()
{
    ImGui::TreePop();
}


void CUI_Tree::AddChildNode(const string& _strName, DWORD_PTR _Value)
{
    CUI_Tree* pTree = new CUI_Tree(_strName, _Value);

    pTree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf);

    AddChildUI(pTree);
}

void CUI_Tree::AddChildNode(CUI_Tree* _pTree)
{
    _pTree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf);

    AddChildUI(_pTree);
}

void CUI_Tree::CheckCallback()
{
    if (true == ImGui::IsItemHovered())
    {
        //더블클릭을 해도 클릭이 먼저 반응하기 때문에
        //무조건 더블클릭을 먼저 감사한 후 클릭을 검사해야 함.
        if (nullptr != m_funcCallback[(int)eUI_MOUSE_STATUS::LBTN_DOUBLE_CLICKED]
            && true == ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            m_funcCallback[(int)eUI_MOUSE_STATUS::LBTN_DOUBLE_CLICKED](this, m_pData);
        }

        else if (nullptr != m_funcCallback[(int)eUI_MOUSE_STATUS::LBTN_CLICKED]
            && true == ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            m_funcCallback[(int)eUI_MOUSE_STATUS::LBTN_CLICKED](this, m_pData);
        }

        else if (nullptr != m_funcCallback[(int)eUI_MOUSE_STATUS::HOVERED])
        {
            m_funcCallback[(int)eUI_MOUSE_STATUS::HOVERED](this, m_pData);
        }
    }
}

