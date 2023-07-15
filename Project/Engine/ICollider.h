#pragma once
#include "IComponent.h"


class ICollider :
    public IComponent
{
private:
    ICollider() = delete;
public:
    ICollider(eCOMPONENT_TYPE _ComType, eDIMENSION_TYPE _eDim);
    virtual ~ICollider();

    virtual bool SaveJson(Json::Value* _pJson) override;
    virtual bool LoadJson(Json::Value* _pJson) override;


public:
    //충돌체 finaltick()은 transform::finaltick()이후 호출되므로 transform의 값을 마음껏 가져다 사용해도 문제 없음.
    virtual void finaltick();

    //고유 충돌체의 정보와 AABB 사각형 정보(m_RectInfo)를 업데이트 해야함.
    virtual void UpdatecCollider() = 0;

private:
    const eDIMENSION_TYPE       m_eCollDimension;
    int                         m_iCollisionCount;  // 현재 충돌중인 충돌체의 갯수를 저장

    //Position은 무조건 트랜스폼을 따라감. 이 값은 cCom_Transform의 위치값에 추가로 Offset을 줌.
    Vec3                        m_v3OffsetPos;

    //Offset을 통해 계산된 실제 중심점 위치
    Vec3                        m_v3CenterPos;

    //Size는 선택적으로 독립적인 사이즈를 사용 가능
    Vec3                        m_v3Size;
    //트랜스폼의 사이즈를 따라갈것인지 / 독립적인 사이즈를 사용할것인지 여부
    bool                        m_bFollowTransformSize;

    //충돌체 업데이트가 필요할 경우 하위 클래스에서 이 값을 변경해주면 됨.
    //이 값은 매 프레임 >>1씩 비트시프트가 진행됨
    UINT8                       m_bCollPosUpdated;
    UINT8                       m_bCollSizeUpdated;

public:
    eDIMENSION_TYPE GetDimensionType() const { return m_eCollDimension; }
    
    void                   SetOffsetPos(const Vec3& _v3Offset) { m_v3OffsetPos = _v3Offset; m_bCollPosUpdated |= 0x02; }
    const Vec3&            GetOffsetPos()      const { return m_v3OffsetPos; }

    const Vec3& GetCenterPos()      const { return m_v3CenterPos; }

    //사이즈를 직접 설정하는순간 cCom_Transform과 독립적인 사이즈를 사용
    //이 함수의 호출 타이밍을 알 수가 없으므로 최소 2프레임동안 업데이트 플래그가 유지되도록 설정해준다.
    void SetCollSize(const Vec3& _v3Size) { m_v3Size = _v3Size; m_bFollowTransformSize = false; m_bCollSizeUpdated |= 0x02; }
    void SetCollSize(const Vec2& _v2Size) { m_v3Size = _v2Size; m_bFollowTransformSize = false; m_bCollSizeUpdated |= 0x02; }

    const Vec3& GetCollSize()    const { return m_v3Size; }


    void SetFollowTransformSize(bool _bFollow) { m_bFollowTransformSize = _bFollow; }
    bool isFollowTransformSize() const { return m_bFollowTransformSize; }

    bool isCollPosUpdated() const { return (0x00 != m_bCollPosUpdated); }
    bool isCollSizeUpdated() const { return (0x00 != m_bCollSizeUpdated); }



public:
    void BeginCollision(ICollider* _other, const Vec3& _v3HitPoint);
    void OnCollision(ICollider* _other, const Vec3& _v3HitPoint);
    void EndCollision(ICollider* _other);
    void            AddCollisionCount() { ++m_iCollisionCount; }
    void            SubCollisionCount() { --m_iCollisionCount; }
    int             GetCollisionCount() const { return m_iCollisionCount; }

    int             GetLayerIndex() { return GetOwner()->GetLayer(); }


    //nullptr이 반환될 수 있으므로 주의할것
    cScriptHolder* ScriptHolder() { return GetOwner()->ScriptHolder(); }

};
