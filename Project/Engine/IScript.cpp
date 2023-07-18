#include "pch.h"
#include "IScript.h"
#include "cTransform.h"
#include "cCom_Renderer_Basic.h"
#include "cTimeMgr.h"
#include "cKeyMgr.h"


IScript::IScript()
	: IComponent(eCOMPONENT_TYPE::SCRIPTS)
{
}

IScript::IScript(const IScript& _other)
	: IComponent(_other)
{
}

IScript::~IScript()
{
}
