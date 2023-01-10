#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(eCOMPONENT_TYPE _Type)
	: m_pOwner(nullptr)
	, m_Type(_Type)	
{
}

CComponent::CComponent(const CComponent& _other)
	: CEntity(_other)
	, m_Type(_other.m_Type)
	, m_pOwner(nullptr)
{
}

CComponent::~CComponent()
{
}
