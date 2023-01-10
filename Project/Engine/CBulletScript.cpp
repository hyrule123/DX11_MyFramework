#include "pch.h"
#include "CBulletScript.h"

#include "CTransform.h"
#include "CTimeMgr.h"

void CBulletScript::init()
{
	GetOwner()->Transform()->SetRelativePos(m_vDefaultPos);
}

void CBulletScript::tick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	vPos += m_vBulletDir * m_fBulletSpeed * DT;

	GetOwner()->Transform()->SetRelativePos(vPos);

	m_fTimeLeft -= DT;

	//이벤트 매니저에 삭제 등록을 해놔야함
}

void CBulletScript::SetDefaultVal(const Vec3& _vPos, const Vec3& _vDir)
{
	m_vDefaultPos = _vPos;
	m_vBulletDir = _vDir;

}

CBulletScript::CBulletScript()
	: CScript(TYPE_INDEX(CBulletScript))
	, m_fBulletSpeed(500.f)
	, m_fTimeLeft(5.f)
{
}

CBulletScript::~CBulletScript()	
{
}
