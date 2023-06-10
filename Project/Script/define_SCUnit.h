#pragma once

#include <string_view>


//두 상태를 동시에 가질 수 있으므로 
namespace SC
{
	enum class eRACE
	{
		NEUTRAL,
		TERRAN,
		PROTOSS,
		ZERG
	};

	enum class eUNIT_SIZE
	{
		SMALL,
		MEDIUM,
		BIG,
	};



	enum class eUNIT_ARMOR_TYPE
	{
		isBionic = BITMASK(0),
		isMechanic = BITMASK(1),
	};


	enum class eUNIT_ATTACK_TYPE
	{
		NORMAL,
		EXPLOSIVE,
		CONCUSSIVE,
		SPELL
	};

	enum class eUNIT_WEAPON_FLAG
	{
		GROUND = BITMASK(0),
		AIR = BITMASK(1),
	};

	enum class eUNIT_MOVE_TYPE
	{
		GROUND,
		HOVER,
		AIR
	};

	namespace FSM
	{
		enum eSTATE : UINT
		{
			IDLE,

			MOVE,

			ATTACK,

			DEATH,

			IN_CONSTRUCTION,

			BUILD_UNIT,

			LIFTOFF,

			END
		};

		namespace strKey_Anim
		{
			STRKEY_DECLARE(IDLE);

			STRKEY_DECLARE(MOVE);

			STRKEY_DECLARE(ATTACK_BEGIN_END);
			STRKEY_DECLARE(ATTACK);

			STRKEY_DECLARE(DEATH);

			STRKEY_DECLARE(IN_CONSTRUCTION);
			STRKEY_DECLARE(BUILD_UNIT);

			STRKEY_DECLARE(LIFTOFF);
		}
	}


	namespace strKey_Anim
	{
		namespace Terran
		{
			STRKEY(CONSTRUCTION_LARGE) = "Construction_Large";

		}
	}


	namespace UNIT_INFO
	{

	}

	namespace INGAME_LAYER_INFO
	{
		enum idx
		{
			_00,
			TileMap,
			_03,
			GroundUnitShadow,
			_04,
			GroundUnitBack,
			_06,
			GroundUnitMain,
			_08,
			GroundUnitEffects,
			_10,
			GroundUnitAttack,
			_12,
			AirUnitShadow,
			_14,
			AirUnitBack,
			_16,
			AirUnitMain,
			_18,
			AirUnitBooster,
			_20,
			AirUnitAttack,
			_22,
			_23,
			_24,
			_25,
			_26,
			_27,
			_28,
			_29,
			MouseCursor,
			_31,
			END
		};

		constexpr inline const char* strLayerName[32] = {
		"", //00
		"TileMap",	//01
		"",	//02
		"GroundUnitShadow",	//03
		"",	//04
		"GroundUnitBack",	//05
		"",	//06
		"GroundUnitMain",	//07
		"",	//08
		"GroundUnitEffects",	//09
		"",	//10
		"GroundUnitAttack",	//11
		"",	//12
		"AirUnitShadow",	//13
		"",	//14
		"AirUnitBack",	//15
		"",	//16
		"AirUnitMain",	//17
		"",	//18
		"AirUnitBooster",	//19
		"",	//20
		"AirUnitAttack",	//21
		"",	//22
		"",	//23
		"",	//24
		"",	//25
		"",	//26
		"",	//27
		"",	//28
		"",	//29
		"MouseCursor",	//30
		"",	//31
		};
	}
}



