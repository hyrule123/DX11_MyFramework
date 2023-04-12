#pragma once

#include <string_view>

enum class eSCUNIT_UNIT_RACE
{
    NEUTRAL,
    TERRAN,
    PROTOSS,
    ZERG
};

enum class eSCUNIT_UNIT_SIZE_TYPE
{
    Small,
    Midium,
    Big,
};

//두 상태를 동시에 가질 수 있으므로 
enum class eSCUNIT_ARMOR_TYPE
{
    isBionic = BITMASK(0),
    isMechanic = BITMASK(1),
};


enum class eSCUNIT_ATTACK_TYPE
{
    Normal,
    Explosive,
    Concussive,
    Spell
};

enum class eSCUNIT_WEAPON_FLAG
{
    isGroundAttackable = BITMASK(0),
    isAirAttackable = BITMASK(1),
};

enum class eSCUNIT_MOVE_TYPE
{
    Ground,
    Hover,
    Air
};

namespace FSM_SCGroundUnit
{
    enum eSTATE
    {
        IDLE,
        MOVE,
        ATTACK_BEGIN,
        ATTACK,
        ATTACK_END,
        DEATH,
        END
    };

    constexpr std::string_view strKey_Anim[END] = {
        "IDLE",
        "MOVE",
        "ATTACK_BEGIN",
        "ATTACK",
        "ATTACK_END",
        "DEATH"
    };
}

namespace UNIT_INFO
{

}
