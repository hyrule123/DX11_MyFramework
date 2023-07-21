#include "pch.h"
#include "cUI_Tree.h"

cUI_Tree::cUI_Tree(const string& _strName)
    : cUI_Widget(_strName, eWIDGET_TYPE::TREE)
    , m_flagTree()
    , m_funcCallback{}
{
    m_flagTree |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_FramePadding;
}

cUI_Tree::~cUI_Tree()
{
}

bool cUI_Tree::beginUI()
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


void cUI_Tree::endUI()
{
    ImGui::TreePop();
}


void cUI_Tree::AddChildNode(const string& _strName, tPtrData _pData)
{
    cUI_Tree* pTree = new cUI_Tree(_strName);
    pTree->SetDataPtr(_pData);

    pTree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf);

    AddChildUI(pTree);
}

void cUI_Tree::AddChildNode(cUI_Tree* _pTree)
{
    _pTree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf);

    AddChildUI(_pTree);
}

void cUI_Tree::CheckCallback()
{
    if (true == ImGui::IsItemHovered())
    {
        //더블클릭을 해도 클릭이 먼저 반응하기 때문에
        //무조건 더블클릭을 먼저 감사한 후 클릭을 검사해야 함.
        if (nullptr != m_funcCallback[(int)eUI_MOUSE_STATUS::LBTN_DOUBLE_CLICKED]
            && true == ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            m_funcCallback[(int)eUI_MOUSE_STATUS::LBTN_DOUBLE_CLICKED](GetUIData());
        }

        else if (nullptr != m_funcCallback[(int)eUI_MOUSE_STATUS::LBTN_CLICKED]
            && true == ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            m_funcCallback[(int)eUI_MOUSE_STATUS::LBTN_CLICKED](GetUIData());
        }

        else if (nullptr != m_funcCallback[(int)eUI_MOUSE_STATUS::HOVERED])
        {
            m_funcCallback[(int)eUI_MOUSE_STATUS::HOVERED](GetUIData());
        }
    }
}

