#pragma once

#include "EffectGroups.h"
#include "EffectExecutionType.h"

#include <unordered_map>

enum EffectType
{
	EFFECT_PLAYER_SUICIDE,
	EFFECT_PLUS_2_STARS,
	EFFECT_5_STARS,
	EFFECT_NEVER_WANTED,
	EFFECT_STRIP_WEAPONS,
	EFFECT_HEAL,
	EFFECT_IGNITE,
	EFFECT_ANGRY_JESUS,
	EFFECT_SPAWN_IMPOTENTRAGE,
	EFFECT_ANGRY_JESUS2,
	EFFECT_IGNITE_PEDS,
	EFFECT_EXPLODE_VEHS,
	EFFECT_PLAYER_LAUNCH,
	EFFECT_VEHS_LAUNCH,
	EFFECT_PLAYER_VEH_LOCK,
	EFFECT_NOTHING,
	EFFECT_KILL_ENGINE,
	EFFECT_TIME_MORNING,
	EFFECT_TIME_DAY,
	EFFECT_TIME_EVENING,
	EFFECT_TIME_NIGHT,
	EFFECT_WEATHER_EXTRASUNNY,
	EFFECT_WEATHER_THUNDER,
	EFFECT_WEATHER_FOGGY,
	EFFECT_WEATHER_NEUTRAL,
	EFFECT_WEATHER_XMAS,
	EFFECT_TP_LSAIRPORT,
	EFFECT_TP_MAZETOWER,
	EFFECT_TP_FORTZANCUDO,
	EFFECT_TP_MOUNTCHILLIAD,
	EFFECT_TP_SKYFALL,
	EFFECT_TP_RANDOM,
	EFFECT_TP_MISSION,
	EFFECT_TP_FAKE,
	EFFECT_NO_PHONE,
	EFFECT_SET_INTO_CLOSEST_VEH,
	EFFECT_PEDS_EXIT_VEH,
	EFFECT_GAMESPEED_X05,
	EFFECT_GAMESPEED_X02,
	EFFECT_GAMESPEED_LAG,
	EFFECT_PEDS_RIOT,
	EFFECT_RED_VEHS,
	EFFECT_BLUE_VEHS,
	EFFECT_GREEN_VEHS,
	EFFECT_CHROME_VEHS,
	EFFECT_PINK_VEHS,
	EFFECT_RAINBOW_VEHS,
	EFFECT_FORCED_FP,
	EFFECT_SLIPPERY_VEHS,
	EFFECT_NO_GRAV_VEHS,
	EFFECT_PLAYER_INVINCIBLE,
	EFFECT_2XENGINE_VEHS,
	EFFECT_10XENGINE_VEHS,
	EFFECT_05XENGINE_VEHS,
	EFFECT_SPAWN_TANK,
	EFFECT_SPAWN_ADDER,
	EFFECT_SPAWN_DUMP,
	EFFECT_SPAWN_MONSTER,
	EFFECT_SPAWN_BMX,
	EFFECT_SPAWN_TUG,
	EFFECT_SPAWN_CARGO,
	EFFECT_SPAWN_BUS,
	EFFECT_SPAWN_BLIMP,
	EFFECT_SPAWN_BUZZARD,
	EFFECT_SPAWN_FAGGIO,
	EFFECT_SPAWN_RUINER3,
	EFFECT_SPAWN_BALETRAILER,
	EFFECT_SPAWN_ROMERO,
	EFFECT_SPAWN_RANDOM,
	EFFECT_NO_VEHS,
	EFFECT_EXPLODE_CUR_VEH,
	EFFECT_PEDS_INVISIBLE,
	EFFECT_VEHS_INVISIBLE,
	EFFECT_NO_RADAR,
	EFFECT_NO_HUD,
	EFFECT_SUPER_RUN,
	EFFECT_PLAYER_RAGDOLL,
	EFFECT_PEDS_RAGDOLL,
	EFFECT_PEDS_RAGDOLL_ON_TOUCH,
	EFFECT_PLAYER_POOR,
	EFFECT_PEDS_FOLLOW_PLAYER,
	EFFECT_PLAYER_DRUNK,
	EFFECT_PEDS_OHKO,
	EFFECT_SCREEN_BLOOM,
	EFFECT_SCREEN_LSD,
	EFFECT_SCREEN_RENDERDIST,
	EFFECT_SCREEN_FOG,
	EFFECT_SCREEN_BRIGHT,
	EFFECT_SCREEN_MEXICO,
	EFFECT_SCREEN_FULLBRIGHT,
	EFFECT_SCREEN_BUBBLEVISION,
	EFFECT_PEDS_BLIND,
	EFFECT_HONK_BOOSTING,
	EFFECT_MIN_DAMAGE,
	EFFECT_PEDS_FROZEN,
	EFFECT_LOW_GRAV,
	EFFECT_VERY_LOW_GRAV,
	EFFECT_INSANE_GRAV,
	EFFECT_INVERT_GRAV,
	EFFECT_VEH_REPAIR,
	EFFECT_VEH_POP_TIRES,
	EFFECT_ALL_VEH_POP_TIRES,
	EFFECT_NO_SPECIAL_ABILITY,
	EFFECT_IN_THE_HOOD,
	EFFECT_FORCED_CINEMATIC,
	EFFECT_PEDS_FLEE,
	EFFECT_BREAK_VEH_DOORS,
	EFFECT_ZOMBIES,
	EFFECT_METEOR_RAIN,
	EFFECT_BLACKOUT,
	EFFECT_QUICK_TIME,
	EFFECT_MOV_NO_UD,
	EFFECT_MOV_NO_LR,
	EFFECT_PLAYER_AUTOPILOT,
	EFFECT_EVERYONE_RPG,
	EFFECT_EVERYONE_TAZER,
	EFFECT_EVERYONE_MINIGUN,
	EFFECT_EVERYONE_UPNATOMIZER,
	EFFECT_EVERYONE_RANDOMWEP,
	EFFECT_EVERYONE_RAILGUN,
	EFFECT_EVERYONE_BATTLEAXE,
	EFFECT_NO_SPRINT_JUMP,
	EFFECT_EVERYONE_INVINCIBLE,
	EFFECT_VEHS_INVINCIBLE,
	EFFECT_PLAYER_SHOT_RAGDOLL,
	EFFECT_JUMPY_VEHS,
	EFFECT_LOCK_VEHS,
	EFFECT_TOTAL_CHAOS,
	EFFECT_NO_RAGDOLL,
	EFFECT_VEHS_HORN,
	EFFECT_TP_WAYPOINT,
	EFFECT_PEDS_SAY_HI,
	EFFECT_PEDS_INSULT,
	EFFECT_EXPLOSIVE_COMBAT,
	EFFECT_GIVE_ALL_WEPS,
	EFFECT_PEDS_AIMBOT,
	EFFECT_SPAWN_COMPANION_CHOP,
	EFFECT_SPAWN_COMPANION_CHIMP,
	EFFECT_SPAWN_COMPANION_BRAD,
	EFFECT_SPAWN_COMPANION_RANDOM,
	EFFECT_SPAWN_BALLA_SQUAD,
	EFFECT_PLAYER_NIGHTVISION,
	EFFECT_PLAYER_HEATVISION,
	EFFECT_PLAYER_MONEYDROPS,
	EFFECT_VEH_TPRANDOMPEDS,
	EFFECT_PEDS_REVIVE,
	EFFECT_TP_TO_STORE,
	EFFECT_SNOW,
	EFFECT_WHALE_RAIN,
	EFFECT_VEH_MAX_UPGRADES,
	EFFECT_VEH_RANDOM_UPGRADES,
	EFFECT_INTENSE_MUSIC,
	EFFECT_PEDS_DRIVEBY,
	EFFECT_PLAYER_RANDOMCLOTHES,
	EFFECT_PEDS_RAINBOWWEPS,
	EFFECT_GTAO_TRAFFIC,
	EFFECT_SPAWN_IE_SULTAN,
	EFFECT_PLAYER_SETINTORANDVEH,
	EFFECT_VEHS_FULLACCEL,
	EFFECT_SPAWN_UFO,
	EFFECT_SPAWN_FERRISWHEEL,
	EFFECT_EXPLOSIVE_PEDS,
	EFFECT_INVERTVELOCITY,
	EFFECT_PLAYER_TPEVERYTHING,
	EFFECT_WEATHER_RANDOMWEATHER,
	EFFECT_LOW_POLY,
	EFFECT_NEARBY_PEDS_OBLITERATE,
	EFFECT_VEHS_TRIGGER_ALARM,
	EFFECT_VEH_SET_RANDOM_SEAT,
	EFFECT_VEH_SET_TOPSPEED_30MPH,
	EFFECT_JESUS_TAKE_THE_WHEEL,
	EFFECT_VEH_POP_TIRE_LOOP,
	EFFECT_ANGRY_ALIEN,
	EFFECT_ANGRY_JIMMY,
	EFFECT_OHKO_VEHICLES,
	EFFECT_VEH_SPAM_DOORS,
	EFFECT_VEH_SPEED_MINIMUM,
	EFFECT_MISC_CREDITS,
	EFFECT_MISC_EARTHQUAKE,
	EFFECT_TP_FRONT,
	EFFECT_SPAWN_FAN_CATS,
	EFFECT_PEDS_COPS,
	EFFECT_ROT_ALL_VEHS,
	EFFECT_LAUNCH_ALL_PEDS,
	EFFECT_PEDS_ATTACK_PLAYER,
	EFFECT_CLONE_PLAYER,
	EFFECT_SLIDY_PEDS,
	EFFECT_SPAWN_DANCING_APES,
	EFFECT_ONE_BULLET_WEP,
	EFFECT_PHONES,
	EFFECT_MIDAS_TOUCH,
	EFFECT_SPAWN_RANDOM_HOSTILE,
	EFFECT_PEDS_PORTAL_GUN,
	EFFECT_MISC_FIREWORKS,
	EFFECT_VEH_DESPAWN,
	EFFECT_PEDS_SCOOTERBROTHERS,
	EFFECT_PEDS_INTORANDOMVEHS,
	EFFECT_VEH_FLYING_CAR,
	EFFECT_PLAYER_HEAVY_RECOIL,
	EFFECT_PEDS_CAT_GUNS,
	EFFECT_PLAYER_FORCEFIELD,
	EFFECT_MISC_OIL_LEAKS,
	EFFECT_PEDS_GUNSMOKE,
	EFFECT_PLAYER_KEEP_RUNNING,
	EFFECT_GAMESPEED_SUPERHOT,
	EFFECT_VEH_WEAPONS,
	EFFECT_MISC_AIRSTRIKE,
	EFFECT_PEDS_MINIONS,
	EFFECT_PEDS_MERCENARIES,
	EFFECT_LOOSE_TRIGGER,
	EFFECT_PLAYER_KICKFLIP,
	EFFECT_MISC_FLAMETHROWER,
	EFFECT_MISC_DVDSCREENSAVER,
	EFFECT_PLAYER_FAKEDEATH,
	EFFECT_VEH_BEYBLADE,
	EFFECT_KILLER_CLOWNS,
	EFFECT_JAMES_BOND,
	EFFECT_PLAYER_POOF,
	EFFECT_PLAYER_SIMEONSAYS,
	EFFECT_VEH_LOCKCAMERA,
	EFFECT_VEH_REPLACEVEHICLE,
	EFFECT_PLAYER_TIRED,
	EFFECT_SCREEN_LS_NOIRE,
	EFFECT_MISC_SUPER_STUNT,
	EFFECT_SCREEN_NEED_GLASSES,
	EFFECT_FLIP_CAMERA,
	EFFECT_PLAYER_WALK_ON_WATER,
	EFFECT_RAPID_FIRE,
	EFFECT_PLAYER_ON_DEMAND_CARTOON,
	EFFECT_PEDS_DRIVE_BACKWARDS,
	EFFECT_VEH_RANDTRAFFIC,
	EFFECT_MISC_RAMPJAM,
	EFFECT_MISC_VEHICLE_RAIN,
	EFFECT_MISC_CRASH,
	EFFECT_PLAYER_GRAVITY,
	EFFECT_VEH_BOUNCY,
	EFFECT_PEDS_STOP_AND_STARE,
	EFFECT_PEDS_FLIP,
	EFFECT_PLAYER_PACIFIST,
	EFFECT_PEDS_BUSBOIS,
	EFFECT_PLAYER_DEAD_EYE,
	EFFECT_PLAYER_QUAKE_FOV,
	EFFECT_PLAYER_HACKING,
	EFFECT_PEDS_NAILGUNS,
	EFFECT_VEH_BRAKEBOOST,
	EFFECT_PLAYER_BEES,
	EFFECT_PLAYER_VR,
	EFFECT_MISC_PORTRAIT_MODE,
	EFFECT_HIGH_PITCH,
	EFFECT_NO_SKY,
	EFFECT_PLAYER_GTA_2,
	EFFECT_PEDS_KIFFLOM,
	EFFECT_META_TIMER_SPEED_X0_5,
	EFFECT_META_TIMER_SPEED_X2,
	EFFECT_META_TIMER_SPEED_X5,
	EFFECT_META_EFFECT_DURATION_X2,
	EFFECT_META_EFFECT_DURATION_X0_5,
	EFFECT_META_HIDE_CHAOS_UI,
	EFFECT_META_ADDITIONAL_EFFECTS,
	EFFECT_VEHS_CRUMBLE,
	EFFECT_MISC_FPS_LIMIT,
	EFFECT_META_NO_CHAOS,
	EFFECT_PEDS_ROASTING,
	EFFECT_PLAYER_BINOCULARS,
	EFFECT_VEHS_CRUISE_CONTROL,
	EFFECT_PLAYER_AIMBOT,
	EFFECT_PEDS_SLIPPERY_PEDS,
	EFFECT_PEDS_SPAWN_BIKER,
	EFFECT_DEATH_ROLL,
	EFFECT_COOKIE_DOUGH,
	EFFECT_GET_UP,
	EFFECT_WORST_AVENGER,
	EFFECT_CONFLICTED_DRIVER,
	EFFECT_NOT_FAIR,
	EFFECT_HONK_HONK,
	EFFECT_TWIRLING,
	EFFECT_NO_TRACTION,
	EFFECT_ADORING_FANS,
	EFFECT_CULLING,
	EFFECT_FIERY_GAZE,
	EFFECT_PANIC,
	EFFECT_FIGHT_CLUB,
	EFFECT_DO_NOT_BELONG,
	EFFECT_TWENTY_TWENTY,
	EFFECT_MMM_MONKE,
	EFFECT_CLOWN_ATTACK,
	EFFECT_MR_IMPORTANT,
	EFFECT_RAIN_FIRE,
	EFFECT_THE_MESSIAH,
	EFFECT_MOON_QUAKE,
	EFFECT_TOOK_TOO_MUCH,
	EFFECT_GOTTA_GO_FAST,
	EFFECT_IS_IT_OVER_YET,
	EFFECT_ESCAPE,
	EFFECT_MAYBE_ESCAPE,
	EFFECT_REALLY_BAD_NIGHT,
	EFFECT_NO_ESCAPE,
	EFFECT_NEED_A_RIDE,
	EFFECT_BAD_COP_NO_DONUT,
	EFFECT_ICE_TO_MEET_YOU,
	EFFECT_JUNKER,
	EFFECT_TOPSY_TURVY,
	EFFECT_PLAGUE,
	EFFECT_CHIPMUNKS,
	EFFECT_WHERE_YOU_END_UP,
	EFFECT_MONEY_MAN,
	EFFECT_FILTHY_CONSOLE_PEASANT,
	EFFECT_NO_PEACE_AND_QUIET,
	EFFECT_DRUGS_ARE_BAD,
	EFFECT_WOLFMAN,
	EFFECT_MARDI_GRAS,
	EFFECT_BODIES_HIT_THE_FLOOR,
	EFFECT_EPIC_BENDER,
	EFFECT_TOP_GUN,
	EFFECT_BUMPER_CARS,
	EFFECT_SANIC,
	EFFECT_PEDS_SPAWN_SPACE_RANGER,
	EFFECT_THE_LEMONING,
	_EFFECT_ENUM_MAX
};

struct EffectInfo
{
	const char* Name;
	const char* Id;
	bool IsTimed = false;
	bool IsShortDuration = false;
	std::vector<EffectType> IncompatibleWith;
	EffectGroupType EffectGroupType = EffectGroupType::NONE;
	EffectExecutionType ExecutionType = EffectExecutionType::DEFAULT;
};

inline std::unordered_map<EffectType, EffectInfo> g_effectsMap;