#include "pch.h"
#include "SCUnitCom_Base.h"

#include "CScript_SCEntity.h"

SCUnitCom_Base::SCUnitCom_Base(CScript_SCEntity* _pOwner)
	: m_eSCUnit_ComType(_eComType)
	, m_pOwner(_pOwner)
{
}

SCUnitCom_Base::SCUnitCom_Base(const SCUnitCom_Base& _other)
	: m_eSCUnit_ComType(_other.m_eSCUnit_ComType)
	, m_pOwner()
	
{
}

SCUnitCom_Base::~SCUnitCom_Base()
{
}
