#include "pch.h"


#include "CUI_Contents.h"


#include "CUI_Tree.h"

#include <Engine/CResMgr.h>

CUI_Contents::CUI_Contents()
	: CUI_BasicWindow("Contents")
{
}

CUI_Contents::~CUI_Contents()
{
}

void CUI_Contents::init()
{
	CUI_Tree* Tree = new CUI_Tree("Texture");
	Tree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);
	AddChildUI(Tree);
	auto TexMap = CResMgr::GetInst()->GetResMap(eRES_TYPE::TEXTURE);
	for (auto iter : TexMap)
	{
		Tree->AddChildNode(iter.first, (DWORD_PTR)(iter.second.Get()));
	}

}
