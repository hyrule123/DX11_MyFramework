#pragma once

#include <string_view>


enum class eSCUNIT_UNIT_RACE
{
    NEUTRAL,
    TERRAN,
    PROTOSS,
    ZERG
};

enum class eSCUNIT_SIZE
{
    SMALL,
    MEDIUM,
    BIG,
};

//두 상태를 동시에 가질 수 있으므로 
enum class eSCUNIT_ARMOR_TYPE
{
    isBionic = BITMASK(0),
    isMechanic = BITMASK(1),
};


enum class eSCUNIT_ATTACK_TYPE
{
    NORMAL,
    EXPLOSIVE,
    CONCUSSIVE,
    SPELL
};

enum class eSCUNIT_WEAPON_FLAG
{
    GROUND = BITMASK(0),
    AIR = BITMASK(1),
};

enum class eSCUNIT_MOVE_TYPE
{
    GROUND,
    HOVER,
    AIR
};

namespace FSM_SCUnit
{
    enum eSTATE : UINT
    {
        IDLE,
        MOVE,
        ATTACK,
        DEATH,
        END
    };

    namespace strKey_Anim
    {
        STRKEY_DECLARE(IDLE);
        STRKEY_DECLARE(MOVE);
        STRKEY_DECLARE(ATTACK_BEGIN_END);
        STRKEY_DECLARE(ATTACK);
        STRKEY_DECLARE(DEATH);
    }
    
}

namespace UNIT_INFO
{

}
