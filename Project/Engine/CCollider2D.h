#pragma once
#include "CCollider.h"

#include <UtilLib_DLL/json/forwards.h>

class CCollider2D_Rect;
class CCollider2D_Circle;
class CCollider2D_OBB;
class CCollider2D_Point;


//기본 : Rect 검사를 진행
//추가적인 정밀 충돌체가 필요할 경우 상속 클래스들을 사용하면 됨.
class CCollider2D :
    public CCollider
{
private:
    CCollider2D() = delete;
public:
    CCollider2D(eCOLLIDER_TYPE_2D _eColType);
    virtual ~CCollider2D();

public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

public:
    virtual void finaltick() final;
    virtual void UpdateCollider() = 0;

    //기본 설정으로 transform에서 받아온 변의 길이로 간이 충돌체를 계산함.
    //더 간단한 충돌체는 이걸 재정의
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos);
    virtual void DebugRender() {};
    virtual void cleanup() override {};



private:
    eCOLLIDER_TYPE_2D      m_eColType;

    //공간분할에서 검사된 자신이 속한 그리드 인덱스 번호
    vector<UINT>           m_vecGridIdxInfo;

    //공간분할용 간이 충돌체 정보(직사각형 형태 - 기본 세팅은 transform에서 정사각형 형태)
    //기본 설
    Vec4                   m_vSimpleCollLBRTPos;
    
    //간이충돌체 계산은 각자 충돌체에서 따로 구현해줄것.(점충돌체 때문에)

    //오프셋 위치를 반영한 위치(2D)
    Vec2                   m_vCenterPos;


public:
    eCOLLIDER_TYPE_2D  GetColliderType()   const { return m_eColType; }
    void SetvecGridIdx(vector<UINT>& _vecIdx) { std::swap(m_vecGridIdxInfo, _vecIdx); }

    Vec2 GetCenterPos() const { return m_vCenterPos; }
    const Vec4& GetSimpleCollider() const { return m_vSimpleCollLBRTPos; }
};

