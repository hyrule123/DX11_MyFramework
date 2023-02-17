#include "pch.h"
#include "CUI_Outliner.h"


#include <Engine/CEventMgr.h>

CUI_Outliner::CUI_Outliner()
	: CUI_BasicWindow("Outliner")
{

}

CUI_Outliner::~CUI_Outliner()
{

}

void CUI_Outliner::init()
{
	


}

void CUI_Outliner::tick()
{
	if (true == CEventMgr::GetInst()->IsLevelChanged())
	{
		UpdateObjectTree();
	}
}

void CUI_Outliner::UpdateObjectTree()
{


}
