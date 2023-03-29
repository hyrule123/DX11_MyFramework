#pragma once
#include "CCollider2D.h"
class CCollider2D_Rect :
    public CCollider2D
{
public:
    CCollider2D_Rect();
    CCollider2D_Rect(eCOLLIDER_TYPE_2D _Type);
    virtual ~CCollider2D_Rect();
    CLONE(CCollider2D_Rect)

public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

public:
    virtual void UpdateCollider() override;
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

private:
    //가로세로 길이와 CCollider 클래스의 v3Offset을 활용하여 계산한다.
    enum XY { x, y };
    Vec2 m_v2RectSize;

public:
    void SetSize(Vec2 _v2RectSize) { m_v2RectSize = _v2RectSize; }

    //https://m.ygosu.com/board/st/268087/?page=1 참고
    //위 사이트에 있는 데이터를 반영해서 현재 프로젝트 충돌체 설정에 맞게 XY 길이 + 오프셋으로 변환하는 함수
    //사이즈와 오프셋값으로 변환(V4값이 L-R-T-B임에 주의할 것)
    //나중에 클라이언트 쪽으로 옮길 것
    void SetSCBuildingSize(UINT _uNumMegatileX, UINT _uNumMegatileY, const Vec4& _v4LRTBOffset);
     


};

