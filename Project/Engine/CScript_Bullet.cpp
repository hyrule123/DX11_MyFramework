#include "pch.h"
#include "CScript_Bullet.h"

#include "CTransform.h"
#include "CTimeMgr.h"

void CScript_Bullet::init()
{
	GetOwner()->Transform()->SetRelativePos(m_vDefaultPos);
}

void CScript_Bullet::tick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	vPos += m_vBulletDir * m_fBulletSpeed * DELTA_TIME;

	GetOwner()->Transform()->SetRelativePos(vPos);

	m_fTimeLeft -= DELTA_TIME;

	//이벤트 매니저에 삭제 등록을 해놔야함
}

void CScript_Bullet::SetDefaultVal(const Vec3& _vPos, const Vec3& _vDir)
{
	m_vDefaultPos = _vPos;
	m_vBulletDir = _vDir;

}

CScript_Bullet::CScript_Bullet()
	: CScript(TYPE_INDEX(CScript_Bullet))
	, m_fBulletSpeed(500.f)
	, m_fTimeLeft(5.f)
{
}

CScript_Bullet::~CScript_Bullet()	
{
}
