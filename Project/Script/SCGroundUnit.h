#pragma once

#include <string_view>

namespace SCGroundUnit
{
    enum class eSTATE
    {
        IDLE,
        MOVE,
        ATTACK,
        END
    };

    //지상유닛의 FSM에 대응되는 애니메이션 이름을 저장
    constexpr std::string_view strKeyAnim2D[(int)eSTATE::END] =
    {
        "IDLE",
        "MOVE",
        "ATTACK"
    };
}



