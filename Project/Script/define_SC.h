#pragma once

#include <Engine/define.h>

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

	enum class eUNIT_SIZE_TYPE
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
			STRKEY_DECLARE(CONSTRUCTION_LARGE);
		}

		namespace Neutral
		{
			STRKEY_DECLARE(MINERAL_WEAR_0);
			STRKEY_DECLARE(MINERAL_WEAR_1);
			STRKEY_DECLARE(MINERAL_WEAR_2);
			STRKEY_DECLARE(MINERAL_WEAR_3);

			STRKEY_DECLARE(VESPENE_SMOKE_1);
			STRKEY_DECLARE(VESPENE_SMOKE_1_DEPLETE);
		}
	}

	//ID - strKey배열에 지정되어있지 않은 이름을 저장하는 곳
	namespace strKey_PREFAB
	{
		STRKEY MINERAL = "Mineral";
		STRKEY MAPOBJ = "MapObj";
		STRKEY VESPENE_SMOKE = "Vespene_Smoke";
	}


	namespace UNIT_INFO
	{
		namespace VespeneGeyser
		{
			constexpr float Size[2] = { 128.f, 64.f };
		}
	}

	namespace LAYER_INFO
	{
		enum idx
		{
			_00,
			TileMap,
			_03,
			GroundUnitShadow,
			_04,
			GroundUnitBack,
			Resource,
			GroundUnitMain,
			GroundUnitTop,
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
			UI,
			_26,
			_27,
			_28,
			_29,
			MouseCursor,
			Camera,
			END
		};

		constexpr const char* strLayerName[32] = {
		"", //00
		"TileMap",	//01
		"",	//02
		"GroundUnitShadow",	//03
		"",	//04
		"GroundUnitBack",	//05
		"Resource",	//06
		"GroundUnitMain",	//07
		"GroundUnitTop",	//08
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
		"UI",	//25
		"",	//26
		"",	//27
		"",	//28
		"",	//29
		"MouseCursor",	//30
		"Camera",	//31
		};
	}

	

	enum class eUNIT_ID : UINT8
	{
		TERRAN_MARINE = 0x00,
		TERRAN_GHOST = 0x01,
		TERRAN_VULTURE = 0x02,
		TERRAN_GOLIATH = 0x03,
		TOLIATH_TURRET = 0x04,
		TERRAN_SIEGE_TANK_TANK_MODE = 0x05,
		TANK_TURRET_TANK_MODE = 0x06,
		TERRAN_SCV = 0x07,
		TERRAN_WRAITH = 0x08,
		TERRAN_SCIENCE_VESSEL = 0x09,
		GUI_MONTAG_FIREBAT = 0x0A,
		TERRAN_DROPSHIP = 0x0B,
		TERRAN_BATTLECRUISER = 0x0C,
		VULTURE_SPIDER_MINE = 0x0D,
		NUCLEAR_MISSILE = 0x0E,
		TERRAN_CIVILIAN = 0x0F,
		SARAH_KERRIGAN_GHOST = 0x10,
		ALAN_SCHEZAR_GOLIATH = 0x11,
		ALAN_SCHEZAR_TURRET = 0x12,
		JIM_RAYNOR_VULTURE = 0x13,
		JIM_RAYNOR_MARINE = 0x14,
		TOM_KAZANSKY_WRAITH = 0x15,
		MAGELLAN_SCIENCE_VESSEL = 0x16,
		EDMUND_DUKE_SIEGE_TANK_TANK_MODE = 0x17,
		EDMUND_DUKE_TURRET_TANK_MODE = 0x18,
		EDMUND_DUKE_SIEGE_TANK_SIEGE_MODE = 0x19,
		EDMUND_DUKE_TURRET_SIEGE_MODE = 0x1A,
		ARCTURUS_MENGSK_BATTLECRUISER = 0x1B,
		HYPERION_BATTLECRUISER = 0x1C,
		NORAD_II_BATTLECRUISER = 0x1D,
		TERRAN_SIEGE_TANK_SIEGE_MODE = 0x1E,
		TANK_TURRET_SIEGE_MODE = 0x1F,
		TERRAN_FIREBAT = 0x20,
		SCANNER_SWEEP = 0x21,
		TERRAN_MEDIC = 0x22,
		ZERG_LARVA = 0x23,
		ZERG_EGG = 0x24,
		ZERG_ZERGLING = 0x25,
		ZERG_HYDRALISK = 0x26,
		ZERG_ULTRALISK = 0x27,
		ZERG_BROODLING = 0x28,
		ZERG_DRONE = 0x29,
		ZERG_OVERLORD = 0x2A,
		ZERG_MUTALISK = 0x2B,
		ZERG_GUARDIAN = 0x2C,
		ZERG_QUEEN = 0x2D,
		ZERG_DEFINER = 0x2E,
		ZERG_SCOURGE = 0x2F,
		TORRASQUE_ULTRALISK = 0x30,
		MATRIARCH_QUEEN = 0x31,
		INFESTED_TERRAN = 0x32,
		INFESTED_KERRIGAN = 0x33,
		UNCLEAN_ONE_DEFINER = 0x34,
		HUNTER_KILLER_HYDRALISK = 0x35,
		DEVOURING_ONE_ZERGLING = 0x36,
		KUKULZA_MUTALISK = 0x37,
		KUKULZA_GUARDIAN = 0x38,
		YGGDRASILL_OVERLORD = 0x39,
		TERRAN_VALKYRIE_FRIGATE = 0x3A,
		DEVOURER_GUARDIAN_COCOON = 0x3B,
		PROTOSS_Corsair = 0x3C,
		PROTOSS_DARK_TEMPLAR = 0x3D,
		ZERG_DEVOURER = 0x3E,
		PROTOSS_DARK_ARCHON = 0x3F,
		PROTOSS_PROBE = 0x40,
		PROTOSS_ZEALOT = 0x41,
		PROTOSS_DRAGOON = 0x42,
		PROTOSS_HIGH_TEMPLAR = 0x43,
		PROTOSS_ARCHON = 0x44,
		PROTOSS_SHUTTLE = 0x45,
		PROTOSS_SCOUT = 0x46,
		PROTOSS_ARBITER = 0x47,
		PROTOSS_CARRIER = 0x48,
		PROTOSS_INTERCEPTOR = 0x49,
		DARK_TEMPLAR_HERO = 0x4A,
		ZERATUL_DARK_TEMPLAR = 0x4B,
		TASSADAR_ZERATUL_ARCHON = 0x4C,
		FENIX_ZEALOT = 0x4D,
		FENIX_DRAGOON = 0x4E,
		TASSADAR_TEMPLAR = 0x4F,
		MOJO_SCOUT = 0x50,
		WARBRINGER_REAVER = 0x51,
		GANTHRITOR_CARRIER = 0x52,
		PROTOSS_REAVER = 0x53,
		PROTOSS_OBSERVER = 0x54,
		PROTOSS_SCARAB = 0x55,
		DANIMOTH_ARBITER = 0x56,
		ALDARIS_TEMPLAR = 0x57,
		ARTANIS_SCOUT = 0x58,
		RHYNADON = 0x59,
		BENGALAAS = 0x5A,
		CARGO_SHIP = 0x5B,
		MERCENARY_GUNSHIP = 0x5C,
		SCANTID = 0x5D,
		KAKARU = 0x5E,
		RAGNASAUR = 0x5F,
		URSADON = 0x60,
		LURKER_EGG = 0x61,
		RASZGAL_CORSAIR = 0x62,
		SAMIR_DURAN_GHOST = 0x63,
		ALEXI_STUKOV_GHOST = 0x64,
		MAP_REVEALER = 0x65,
		GERARD_DUGALLE_BATTLECRUISER = 0x66,
		ZERG_LURKER = 0x67,
		INFESTED_DURAN = 0x68,
		DISTRUPTION_WEB = 0x69,
		TERRAN_COMMAND_CENTER = 0x6A,
		TERRAN_COMSAT_STATION = 0x6B,
		TERRAN_NUCLEAR_SILO = 0x6C,
		TERRAN_SUPPLY_DEPOT = 0x6D,
		TERRAN_REFINERY = 0x6E,
		TERRAN_BARRACKS = 0x6F,
		TERRAN_ACADEMY = 0x70,
		TERRAN_FACTORY = 0x71,
		TERRAN_STARPORT = 0x72,
		TERRAN_CONTROL_TOWER = 0x73,
		TERRAN_SCIENCE_FACILITY = 0x74,
		TERRAN_COVERT_OPS = 0x75,
		TERRAN_PHYSICS_LAB = 0x76,
		STARBASE = 0x77,
		TERRAN_MACHINE_SHOP = 0x78,
		REPAIR_BAY = 0x79,
		TERRAN_ENGINEERING_BAY = 0x7A,
		TERRAN_ARMORY = 0x7B,
		TERRAN_MISSILE_TURRET = 0x7C,
		TERRAN_BUNKER = 0x7D,
		CRASHED_NORAD_II = 0x7E,
		ION_CANNON = 0x7F,
		URAJ_CRYSTAL = 0x80,
		KHALIS_CRYSTAL = 0x81,
		INFESTED_COMMAND_CENTER = 0x82,
		ZERG_HATCHERY = 0x83,
		ZERG_LAIR = 0x84,
		ZERG_HIVE = 0x85,
		ZERG_NYDUS_CANAL = 0x86,
		ZERG_HYDRALISK_DEN = 0x87,
		ZERG_DEFINER_MOUND = 0x88,
		ZERG_GREATER_SPIRE = 0x89,
		ZERG_QUEEN_NEST = 0x8A,
		ZERG_EVOLUTION_CHAMBER = 0x8B,
		ZERG_ULTRALISK_CAVERN = 0x8C,
		ZERG_SPIRE = 0x8D,
		ZERG_SPAWNING_POOL = 0x8E,
		ZERG_CREEP_COLONY = 0x8F,
		ZERG_SPORE_COLONY = 0x90,
		ZERG_BUILDING_1 = 0x91,
		ZERG_SUNKEN_COLONY = 0x92,
		OVERMIND_WITH_SHELL = 0x93,
		OVERMIND = 0x94,
		ZERG_EXTRACTOR = 0x95,
		MATURE_CHRYSALIS = 0x96,
		ZERG_CEREBRATE = 0x97,
		ZERG_CEREBRATE_DAGGOTH = 0x98,
		ZERG_BUILDING_2 = 0x99,
		PROTOSS_NEXUS = 0x9A,
		ROBOTICS_FACILITY = 0x9B,
		PROTOSS_PYLON = 0x9C,
		PROTOSS_ASSIMILATOR = 0x9D,
		PROTOSS_BUILDING_1 = 0x9E,
		PROTOSS_OBSERVATORY = 0x9F,
		PROTOSS_GATEWAY = 0xA0,
		PROTOSS_BUILDING_2 = 0xA1,
		PROTOSS_PHOTON_CANNON = 0xA2,
		PROTOSS_CITADEL_OF_ADUN = 0xA3,
		PROTOSS_CYBERNETICS_CORE = 0xA4,
		PROTOSS_TEMPLAR_ARCHIVES = 0xA5,
		PROTOSS_FORGE = 0xA6,
		PROTOSS_STARGATE = 0xA7,
		STASIS_CELL_PRISON = 0xA8,
		PROTOSS_FLEET_BEACON = 0xA9,
		PROTOSS_ARBITER_TRIBUNAL = 0xAA,
		PROTOSS_ROBOTICS_SUPPORT_BAY = 0xAB,
		PROTOSS_SHIELD_BATTERY = 0xAC,
		KHAYDARIN_CRYSTAL_FORMATION_1 = 0xAD,
		PROTOSS_TEMPLE = 0xAE,
		XEL_NAGA_TEMPLE = 0xAF,

		//미네랄
		MINERAL_FIELD_TYPE_1 = 0xB0,
		MINERAL_FIELD_TYPE_2 = 0xB1,
		MINERAL_FIELD_TYPE_3 = 0xB2,


		CAVE = 0xB3,
		CAVE_IN = 0xB4,
		CANTINA = 0xB5,
		MINING_PLATFORM = 0xB6,
		INDEPENDENT_COMMAND_CENTER = 0xB7,
		INDEPENDENT_STARPORT = 0xB8,
		INDEPENDENT_JUMP_GATE = 0xB9,
		RUINS = 0xBA,
		KHAYDARIN_CRYSTAL_FORMATION_2 = 0xBB,


		VESPENE_GEYSER = 0xBC,



		WARP_GATE = 0xBD,
		PSI_DISRUPTOR = 0xBE,
		ZERG_MARKER = 0xBF,
		TERRAN_MARKER = 0xC0,
		PROTOSS_MARKER = 0xC1,
		ZERG_BEACON = 0xC2,
		TERRAN_BEACON = 0xC3,
		PROTOSS_BEACON = 0xC4,
		ZERG_FLAG_BEACON = 0xC5,
		TERRAN_FLAG_BEACON = 0xC6,
		PROTOSS_FLAG_BEACON = 0xC7,
		POWER_GENERATOR = 0xC8,
		OVERMIND_COCOON = 0xC9,
		DARK_SWARM = 0xCA,
		FLOOR_MISSILE_TRAP = 0xCB,
		FLOOR_HATCH = 0xCC,
		LEFT_UPPER_LEVEL_DOOR = 0xCD,
		RIGHT_UPPER_LEVEL_DOOR = 0xCE,
		LEFT_PIT_DOOR = 0xCF,
		RIGHT_PIT_DOOR = 0xD0,
		FLOOR_GUN_TRAP = 0xD1,
		LEFT_WALL_MISSILE_TRAP = 0xD2,
		LEFT_WALL_FLAME_TRAP = 0xD3,
		RIGHT_WALL_MISSILE_TRAP = 0xD4,
		RIGHT_WALL_FLAME_TRAP = 0xD5,

		//====================
		START_LOCATION = 0xD6,
		//====================


		FLAG = 0xD7,
		YOUNG_CRYSALIS = 0xD8,
		PSI_EMITTER = 0xD9,
		DATA_DISC = 0xDA,
		KHAYDARIN_CRYSTAL = 0xDB,
		MINERAL_CHUNK_TYPE_1 = 0xDC,
		MINERAL_CHUNK_TYPE_2 = 0xDD,
		VESPENE_GAS_ORB_TYPE_1 = 0xDE,
		VESPENE_GAS_ORB_TYPE_2 = 0xDF,
		VESPENE_GAS_SAC_TYPE_1 = 0xE0,
		VESPENE_GAS_SAC_TYPE_2 = 0xE1,
		VESPENE_GAS_TANK_TYPE_1 = 0xE2,
		VESPENE_GAS_TANK_TYPE_2 = 0xE3,
		END
	};

	STRKEY arrStrKey[(int)eUNIT_ID::END] =
	{
		"Terran Marine",	//마린

		"TERRAN_GHOST",
		"TERRAN_VULTURE",
		"TERRAN_GOLIATH",
		"TOLIATH_TURRET",
		"TERRAN_SIEGE_TANK_TANK_MODE",
		"TANK_TURRET_TANK_MODE",
		"TERRAN_SCV",
		"TERRAN_WRAITH",
		"TERRAN_SCIENCE_VESSEL",
		"GUI_MONTAG_FIREBAT",
		"TERRAN_DROPSHIP",
		"TERRAN_BATTLECRUISER",
		"VULTURE_SPIDER_MINE",
		"NUCLEAR_MISSILE",
		"TERRAN_CIVILIAN",
		"SARAH_KERRIGAN_GHOST",
		"ALAN_SCHEZAR_GOLIATH",
		"ALAN_SCHEZAR_TURRET",
		"JIM_RAYNOR_VULTURE",
		"JIM_RAYNOR_MARINE",
		"TOM_KAZANSKY_WRAITH",
		"MAGELLAN_SCIENCE_VESSEL",
		"EDMUND_DUKE_SIEGE_TANK_TANK_MODE",
		"EDMUND_DUKE_TURRET_TANK_MODE",
		"EDMUND_DUKE_SIEGE_TANK_SIEGE_MODE",
		"EDMUND_DUKE_TURRET_SIEGE_MODE",
		"ARCTURUS_MENGSK_BATTLECRUISER",
		"HYPERION_BATTLECRUISER",
		"NORAD_II_BATTLECRUISER",
		"TERRAN_SIEGE_TANK_SIEGE_MODE",
		"TANK_TURRET_SIEGE_MODE",
		"TERRAN_FIREBAT",
		"SCANNER_SWEEP",
		"TERRAN_MEDIC",
		"ZERG_LARVA",
		"ZERG_EGG",
		"ZERG_ZERGLING",
		"ZERG_HYDRALISK",
		"ZERG_ULTRALISK",
		"ZERG_BROODLING",
		"ZERG_DRONE",
		"ZERG_OVERLORD",
		"ZERG_MUTALISK",
		"ZERG_GUARDIAN",
		"ZERG_QUEEN",
		"ZERG_DEFINER",
		"ZERG_SCOURGE",
		"TORRASQUE_ULTRALISK",
		"MATRIARCH_QUEEN",
		"INFESTED_TERRAN",
		"INFESTED_KERRIGAN",
		"UNCLEAN_ONE_DEFINER",
		"HUNTER_KILLER_HYDRALISK",
		"DEVOURING_ONE_ZERGLING",
		"KUKULZA_MUTALISK",
		"KUKULZA_GUARDIAN",
		"YGGDRASILL_OVERLORD",
		"TERRAN_VALKYRIE_FRIGATE",
		"DEVOURER_GUARDIAN_COCOON",
		"PROTOSS_Corsair",
		"PROTOSS_DARK_TEMPLAR",
		"ZERG_DEVOURER",
		"PROTOSS_DARK_ARCHON",
		"PROTOSS_PROBE",
		"PROTOSS_ZEALOT",
		"PROTOSS_DRAGOON",
		"PROTOSS_HIGH_TEMPLAR",
		"PROTOSS_ARCHON",
		"PROTOSS_SHUTTLE",
		"PROTOSS_SCOUT",
		"PROTOSS_ARBITER",
		"PROTOSS_CARRIER",
		"PROTOSS_INTERCEPTOR",
		"DARK_TEMPLAR_HERO",
		"ZERATUL_DARK_TEMPLAR",
		"TASSADAR_ZERATUL_ARCHON",
		"FENIX_ZEALOT",
		"FENIX_DRAGOON",
		"TASSADAR_TEMPLAR",
		"MOJO_SCOUT",
		"WARBRINGER_REAVER",
		"GANTHRITOR_CARRIER",
		"PROTOSS_REAVER",
		"PROTOSS_OBSERVER",
		"PROTOSS_SCARAB",
		"DANIMOTH_ARBITER",
		"ALDARIS_TEMPLAR",
		"ARTANIS_SCOUT",
		"RHYNADON",
		"BENGALAAS",
		"CARGO_SHIP",
		"MERCENARY_GUNSHIP",
		"SCANTID",
		"KAKARU",
		"RAGNASAUR",
		"URSADON",
		"LURKER_EGG",
		"RASZGAL_CORSAIR",
		"SAMIR_DURAN_GHOST",
		"ALEXI_STUKOV_GHOST",
		"MAP_REVEALER",
		"GERARD_DUGALLE_BATTLECRUISER",
		"ZERG_LURKER",
		"INFESTED_DURAN",
		"DISTRUPTION_WEB",
		"Terran Command Center",
		"TERRAN_COMSAT_STATION",
		"TERRAN_NUCLEAR_SILO",
		"TERRAN_SUPPLY_DEPOT",
		"TERRAN_REFINERY",
		"TERRAN_BARRACKS",
		"TERRAN_ACADEMY",
		"TERRAN_FACTORY",
		"TERRAN_STARPORT",
		"TERRAN_CONTROL_TOWER",
		"TERRAN_SCIENCE_FACILITY",
		"TERRAN_COVERT_OPS",
		"TERRAN_PHYSICS_LAB",
		"STARBASE",
		"TERRAN_MACHINE_SHOP",
		"REPAIR_BAY",
		"TERRAN_ENGINEERING_BAY",
		"TERRAN_ARMORY",
		"TERRAN_MISSILE_TURRET",
		"TERRAN_BUNKER",
		"CRASHED_NORAD_II",
		"ION_CANNON",
		"URAJ_CRYSTAL",
		"KHALIS_CRYSTAL",
		"INFESTED_COMMAND_CENTER",
		"ZERG_HATCHERY",
		"ZERG_LAIR",
		"ZERG_HIVE",
		"ZERG_NYDUS_CANAL",
		"ZERG_HYDRALISK_DEN",
		"ZERG_DEFINER_MOUND",
		"ZERG_GREATER_SPIRE",
		"ZERG_QUEEN_NEST",
		"ZERG_EVOLUTION_CHAMBER",
		"ZERG_ULTRALISK_CAVERN",
		"ZERG_SPIRE",
		"ZERG_SPAWNING_POOL",
		"ZERG_CREEP_COLONY",
		"ZERG_SPORE_COLONY",
		"ZERG_BUILDING_1",
		"ZERG_SUNKEN_COLONY",
		"OVERMIND_WITH_SHELL",
		"OVERMIND",
		"ZERG_EXTRACTOR",
		"MATURE_CHRYSALIS",
		"ZERG_CEREBRATE",
		"ZERG_CEREBRATE_DAGGOTH",
		"ZERG_BUILDING_2",
		"PROTOSS_NEXUS",
		"ROBOTICS_FACILITY",
		"PROTOSS_PYLON",
		"PROTOSS_ASSIMILATOR",
		"PROTOSS_BUILDING_1",
		"PROTOSS_OBSERVATORY",
		"PROTOSS_GATEWAY",
		"PROTOSS_BUILDING_2",
		"PROTOSS_PHOTON_CANNON",
		"PROTOSS_CITADEL_OF_ADUN",
		"PROTOSS_CYBERNETICS_CORE",
		"PROTOSS_TEMPLAR_ARCHIVES",
		"PROTOSS_FORGE",
		"PROTOSS_STARGATE",
		"STASIS_CELL_PRISON",
		"PROTOSS_FLEET_BEACON",
		"PROTOSS_ARBITER_TRIBUNAL",
		"PROTOSS_ROBOTICS_SUPPORT_BAY",
		"PROTOSS_SHIELD_BATTERY",
		"KHAYDARIN_CRYSTAL_FORMATION_1",
		"PROTOSS_TEMPLE",
		"XEL_NAGA_TEMPLE",

		"Mineral",	//세 개는 모두 같은 게임오브젝트를 사용(내부 변수를 다르게 해줘야함)
		"Mineral",
		"Mineral",

		"CAVE",
		"CAVE_IN",
		"CANTINA",
		"MINING_PLATFORM",
		"INDEPENDENT_COMMAND_CENTER",
		"INDEPENDENT_STARPORT",
		"INDEPENDENT_JUMP_GATE",
		"RUINS",
		"KHAYDARIN_CRYSTAL_FORMATION_2",

		"VESPENE_GEYSER",

		"WARP_GATE",
		"PSI_DISRUPTOR",
		"ZERG_MARKER",
		"TERRAN_MARKER",
		"PROTOSS_MARKER",
		"ZERG_BEACON",
		"TERRAN_BEACON",
		"PROTOSS_BEACON",
		"ZERG_FLAG_BEACON",
		"TERRAN_FLAG_BEACON",
		"PROTOSS_FLAG_BEACON",
		"POWER_GENERATOR",
		"OVERMIND_COCOON",
		"DARK_SWARM",
		"FLOOR_MISSILE_TRAP",
		"FLOOR_HATCH",
		"LEFT_UPPER_LEVEL_DOOR",
		"RIGHT_UPPER_LEVEL_DOOR",
		"LEFT_PIT_DOOR",
		"RIGHT_PIT_DOOR",
		"FLOOR_GUN_TRAP",
		"LEFT_WALL_MISSILE_TRAP",
		"LEFT_WALL_FLAME_TRAP",
		"RIGHT_WALL_MISSILE_TRAP",
		"RIGHT_WALL_FLAME_TRAP",

		"START_LOCATION",

		"FLAG",
		"YOUNG_CRYSALIS",
		"PSI_EMITTER",
		"DATA_DISC",
		"KHAYDARIN_CRYSTAL",
		"MINERAL_CHUNK_TYPE_1",
		"MINERAL_CHUNK_TYPE_2",
		"VESPENE_GAS_ORB_TYPE_1",
		"VESPENE_GAS_ORB_TYPE_2",
		"VESPENE_GAS_SAC_TYPE_1",
		"VESPENE_GAS_SAC_TYPE_2",
		"VESPENE_GAS_TANK_TYPE_1",
		"VESPENE_GAS_TANK_TYPE_2"
	};


	inline const char* GetUnitName(SC::eUNIT_ID _eID)
	{
		return SC::arrStrKey[(int)_eID];
	}
};




