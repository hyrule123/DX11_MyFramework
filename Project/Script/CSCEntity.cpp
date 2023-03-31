#include "pch.h"
#include "CSCEntity.h"

CSCEntity::CSCEntity(std::type_index _typeIdx, UINT _eNumState)
	: CFStateMgr(_typeIdx, _eNumState)
{
}

CSCEntity::~CSCEntity()
{
}
