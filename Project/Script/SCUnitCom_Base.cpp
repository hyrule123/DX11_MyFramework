#include "pch.h"
#include "SCUnitCom_Base.h"

#include "CSC_Entity.h"

SCUnitCom_Base::SCUnitCom_Base(eSCUnit_ComType _eComType, CSC_Entity* _pOwner)
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
