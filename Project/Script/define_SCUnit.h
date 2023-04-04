#pragma once

#include <string_view>

enum class eSCUNIT_UNIT_TYPE
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

#define FSM(_NameSpace, ...) \
namespace FSM_##_NameSpace\
{\
    enum eSTATE : UINT {\
        __VA_ARGS__\
        , END\
    };\
\
    constexpr std::string_view strKeyAnim[(int)eSTATE::END] = {\
        #__VA_ARGS__\
    };\
}


FSM(SCGroundUnit,
    IDLE,
    MOVE,
    ATTACK_BEGIN,
    ATTACK,
    ATTACK_END,
    DEATH
);

FSM(MARINE,
    IDLE,
    MOVE,
    ATTACK_BEGIN,
    ATTACK,
    ATTACK_END,
    DEATH
    );
