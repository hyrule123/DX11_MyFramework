#include "pch.h"
#include "Component.h"

#include "jsoncpp.h"
#include "strKey_Default.h"

Component::Component(eCOMPONENT_TYPE _Type)
	: m_pOwner(nullptr)
	, m_eComponentType(_Type)
	, m_bIsDisabled()
{
}

Component::Component(const Component& _other)
	: Entity(_other)
	, m_eComponentType(_other.m_eComponentType)
	, m_pOwner(nullptr)
	, m_bIsDisabled()
{
}

Component::~Component()
{
}


bool Component::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == Entity::LoadJson(_pJson))
		return false;

	(*_pJson)[string(RES_INFO::PREFAB::COMPONENT::JSON_KEY::eComponentType)] = (int)m_eComponentType;

	return true;
}

bool Component::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == Entity::LoadJson(_pJson))
		return false;

	string strKey = string(RES_INFO::PREFAB::COMPONENT::JSON_KEY::eComponentType);
	if (_pJson->isMember(strKey))
	{
		//불러온 컴포넌트 파일 정보가 현재 클래스의 컴포넌트 타일 정보와 동일한지 확인한다.
		if ((int)m_eComponentType == (*_pJson)[string(RES_INFO::PREFAB::COMPONENT::JSON_KEY::eComponentType)].asInt())
			return true;
	}

	return false;
}


