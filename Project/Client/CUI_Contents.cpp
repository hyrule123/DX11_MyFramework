#include "pch.h"


#include "CUI_Contents.h"


#include "CUI_Tree.h"

CUI_Contents::CUI_Contents()
	: CUI_BasicWindow("Contents")
{
}

CUI_Contents::~CUI_Contents()
{
}

void CUI_Contents::init()
{
	CUI_Tree* Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);

	CUI_Tree* Tree2 = new CUI_Tree("Tree2");
	Tree->AddChildUI(Tree2);

	CUI_Tree* Tree3 = new CUI_Tree("Tree3");
	Tree2->AddChildUI(Tree3);

	Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);

	Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);

	Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);

	Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);

	Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);

	Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);

	Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);

	Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);

	Tree = new CUI_Tree("Tree1");
	AddChildUI(Tree);
}
