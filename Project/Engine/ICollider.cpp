#include "pch.h"
#include "ICollider.h"

#include "cScriptHolder.h"
#include "IScript.h"

#include "cTransform.h"

#include "jsoncpp.h"
#include "strKey_Default.h"

ICollider::ICollider(eCOMPONENT_TYPE _ComType, eDIMENSION_TYPE _eDim)
	: IComponent(_ComType)
	, m_eCollDimension(_eDim)
	, m_iCollisionCount()
	, m_v3Size(Vec3(100.f))
	, m_bFollowTransformSize(true)
	, m_bCollPosUpdated(0x02)
	, m_bCollSizeUpdated(0x02)
{
}

ICollider::~ICollider()
{
}

bool ICollider::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == IComponent::SaveJson(_pJson))
		return false;

	Json::Value& jVal = *_pJson;

	{
		string strKey = RES_INFO::PREFAB::COMPONENT::COLLIDER::JSON_KEY::m_v3OffsetPos;
		jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		jVal[strKey].append(Pack_float_int(m_v3OffsetPos.x).i);
		jVal[strKey].append(Pack_float_int(m_v3OffsetPos.y).i);
		jVal[strKey].append(Pack_float_int(m_v3OffsetPos.z).i);
	}

	{
		jVal[RES_INFO::PREFAB::COMPONENT::COLLIDER::JSON_KEY::m_bFollowTransformSize] = m_bFollowTransformSize;
	}

	//고유한 충돌체 사이즈를 별도로 사용하는 경우 사이즈를 별도로 저장한다.
	//트랜스폼의 사이즈를 따라가는 경우 사이즈를 저장하지 않음(어차피 계산됨)
	if (false == m_bFollowTransformSize)
	{
		string strKey = RES_INFO::PREFAB::COMPONENT::COLLIDER::JSON_KEY::m_v3Size;
		jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		jVal[strKey].append(Pack_float_int(m_v3Size.x).i);
		jVal[strKey].append(Pack_float_int(m_v3Size.y).i);
		jVal[strKey].append(Pack_float_int(m_v3Size.z).i);
	}


	return true;
}

bool ICollider::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == IComponent::LoadJson(_pJson))
		return false;

	Json::Value& jVal = *_pJson;

	try
	{
		{
			string strKey = RES_INFO::PREFAB::COMPONENT::COLLIDER::JSON_KEY::m_v3OffsetPos;
			if (jVal.isMember(strKey) && jVal[strKey].isArray() && (jVal[strKey].size() == 3))
			{
				m_v3OffsetPos.x = Pack_float_int(jVal[strKey][0].asInt()).f;
				m_v3OffsetPos.y = Pack_float_int(jVal[strKey][1].asInt()).f;
				m_v3OffsetPos.z = Pack_float_int(jVal[strKey][2].asInt()).f;
			}
			else
			{
				strKey += " is Missing!!";
				throw(std::runtime_error(strKey));
			}
		}

		{
			string strKey = RES_INFO::PREFAB::COMPONENT::COLLIDER::JSON_KEY::m_bFollowTransformSize;
			if (jVal.isMember(strKey))
				m_bFollowTransformSize = jVal[strKey].asBool();
			else
				throw(std::runtime_error(strKey + " is Missing!!"));
		}

		//사이즈의 경우 고유 사이즈를 사용하도록 설정되어 있을 경우에만 불러온다.
		if(false == m_bFollowTransformSize)
		{
			string strKey = RES_INFO::PREFAB::COMPONENT::COLLIDER::JSON_KEY::m_v3Size;
			if (jVal.isMember(strKey) && jVal[strKey].isArray() && (jVal[strKey].size() == 3))
			{
				m_v3Size.x = Pack_float_int(jVal[strKey][0].asInt()).f;
				m_v3Size.y = Pack_float_int(jVal[strKey][1].asInt()).f;
				m_v3Size.z = Pack_float_int(jVal[strKey][2].asInt()).f;
			}
			else
				throw(std::runtime_error(strKey + " is Missing!!"));
		}
	}
	catch (const std::runtime_error& _err)
	{
		ERROR_MESSAGE(_err.what());
		assert(false);
		return false;
	}



	return true;
}


//자신과 상대방의 begincollision 호출
void ICollider::BeginCollision(ICollider* _other, const Vec3& _v3HitPoint)
{
	++m_iCollisionCount;
	_other->AddCollisionCount();

	cScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->BeginColiision(_other, _v3HitPoint);
	

	pSH = _other->GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->BeginColiision(this, _v3HitPoint);
}

void ICollider::OnCollision(ICollider* _other, const Vec3& _v3HitPoint)
{
	cScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->OnCollision(_other, _v3HitPoint);


	pSH = _other->GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->OnCollision(this, _v3HitPoint);
}

void ICollider::EndCollision(ICollider* _other)
{
	--m_iCollisionCount;
	_other->SubCollisionCount();

	cScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->EndCollision(_other);


	pSH = _other->GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->EndCollision(this);
}

void ICollider::finaltick()
{
	m_bCollPosUpdated >>= 1;
	m_bCollSizeUpdated >>= 1;


	//트랜스폼의 위치정보가 변경되었을 경우 중심점을 새로 계산(위치는 무조건 cTransform을 따라감.)
	if (true == Transform().IsUpdated() || isCollPosUpdated())
	{
		m_bCollPosUpdated |= 0x01;

		//자신의 중심 위치를 구한다.
		const Vec3& WorldPos = Transform().GetWorldPos();

		m_v3CenterPos = WorldPos + m_v3OffsetPos;
	}

	//트랜스폼의 사이즈를 따라가고, 트랜스폼의 사이즈가 업데이트 되었을 경우 자신의 사이즈를 업데이트
	if (m_bFollowTransformSize && Transform().GetSizeUpdated())
	{
		m_bCollSizeUpdated |= 0x01;
		m_v3Size = Transform().GetWorldSize();
	}

	//충돌체를 업데이트 해야 하는 경우 UpdateCollider() 함수 호출.
	if (isCollPosUpdated() || isCollSizeUpdated())
		UpdateCollider();
}


