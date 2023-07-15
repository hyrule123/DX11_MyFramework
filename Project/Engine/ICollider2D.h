#pragma once
#include "ICollider.h"

#include <UtilLib_DLL/json/forwards.h>

class cCom_Coll2D_Rect;
class cCom_Coll2D_Circle;
class cCom_Coll2D_OBB;
class cCom_Coll2D_Point;


//기본 : Rect 검사를 진행
//추가적인 정밀 충돌체가 필요할 경우 상속 클래스들을 사용하면 됨.
class ICollider2D :
    public ICollider
{
private:
    ICollider2D() = delete;
public:
    ICollider2D(eCOLLIDER_TYPE_2D _eColType);
    virtual ~ICollider2D();

public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

public:
    virtual void finaltick() final;
    virtual void UpdatecCollider() override;

    //기본 설정으로 transform에서 받아온 변의 길이로 간이 충돌체를 계산함.
    //더 간단한 충돌체는 이걸 재정의
    virtual void UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos) = 0;
    virtual void DebugRender() {};
    virtual void cleanup() override {};



private:
    const eCOLLIDER_TYPE_2D      m_eColType;

    //공간분할에서 검사된 자신이 속한 그리드 인덱스 번호
    vector<UINT>           m_vecGridIdxInfo;

    //공간분할용 간이 충돌체 정보(직사각형 형태 - 기본 세팅은 transform에서 정사각형 형태)
    //기본 설
    Vec4                   m_v4SimpleCollLBRTPos;
    
    //간이충돌체 계산은 각자 충돌체에서 따로 구현해줄것.(점충돌체 때문에)

    



public:
    eCOLLIDER_TYPE_2D  GetcColliderType()   const { return m_eColType; }

    //Swap을 사용하므로 기존 데이터는 사용할수 없어짐에 주의할것
    void SetvecGridIdx(vector<UINT>& _vecIdx) { std::swap(m_vecGridIdxInfo, _vecIdx); }

    //Vec2 GetCenterPos() const { return m_v2CenterPos; }
    const Vec4& GetSimplecCollider() const { return m_v4SimpleCollLBRTPos; }
};

