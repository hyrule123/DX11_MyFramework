#include "pch.h"
#include "CUI_Tree.h"

CUI_Tree::CUI_Tree(const string& _strName, tDataPtr _pData)
    : CUI_Widget(_strName, eWIDGET_TYPE::TREE)
    , m_flagTree()
    , m_pData(_pData)
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

    //�ڽ� ���� ���(������� �ϰ��)
    if (0 == GetChildNum())
    {
        Flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
    }
    //�ڽ� ���� ���
    else
    {
        Flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
    }
        
    Flag |= m_flagTree;

    bool IsOpen = ImGui::TreeNodeEx(GetStrID().c_str(), Flag);

    //�����ִ� �����ִ� �ϴ� �ݹ��Լ� üũ �� ȣ��
    CheckCallback();

    return IsOpen;
}


void CUI_Tree::endUI()
{
    ImGui::TreePop();
}


void CUI_Tree::AddChildNode(const string& _strName, tDataPtr _pData)
{
    CUI_Tree* pTree = new CUI_Tree(_strName, _pData);

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
        //����Ŭ���� �ص� Ŭ���� ���� �����ϱ� ������
        //������ ����Ŭ���� ���� ������ �� Ŭ���� �˻��ؾ� ��.
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

