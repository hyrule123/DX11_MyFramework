#pragma once

class CCollider;
class SC_Func
{
public:
    //https://m.ygosu.com/board/st/268087/?page=1 참고
//위 사이트에 있는 데이터를 반영해서 현재 프로젝트 충돌체 설정에 맞게 XY 길이 + 오프셋으로 변환하는 함수
//사이즈와 오프셋값으로 변환(V4값이 L-R-T-B임에 주의할 것)
//나중에 클라이언트 쪽으로 옮길 것
    static void SetSCBuildingSize(CCollider* _pCol, int _uNumMegatileX, int _uNumMegatileY, const Vec4& _v4LRTBOffset);

};

