#pragma once

#include <string_view>

//FSM
enum class eSTATE_SCGroundUnit
{
    IDLE,
    MOVE,
    ATTACK,
    END
};

//지상유닛의 FSM에 대응되는 애니메이션 이름을 저장
constexpr std::string_view strKeyAnim2D_SCGroundUnit[(int)eSTATE_SCGroundUnit::END] =
{
    "IDLE",
    "MOVE",
    "ATTACK"
};
