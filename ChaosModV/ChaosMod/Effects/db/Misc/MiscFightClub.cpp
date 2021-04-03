#include <stdafx.h>
#include <math.h>  

#define PI 3.14159265

static void Chimp() {
	static const Hash modelHash = GET_HASH_KEY("a_c_chimp");

	Hash relationshipGroup;
	ADD_RELATIONSHIP_GROUP("_COMPANION_CHIMP", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, relationshipGroup, GET_HASH_KEY("PLAYER"));
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, GET_HASH_KEY("PLAYER"), relationshipGroup);

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	Ped ped = CreatePoolPed(28, modelHash, playerPos.x, playerPos.y, playerPos.z, GET_ENTITY_HEADING(playerPed));
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		SET_PED_INTO_VEHICLE(ped, GET_VEHICLE_PED_IS_IN(playerPed, false), -2);
	}

	SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
	SET_PED_HEARING_RANGE(ped, 9999.f);

	SET_PED_AS_GROUP_MEMBER(ped, GET_PLAYER_GROUP(PLAYER_ID()));

	SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

	SET_PED_ACCURACY(ped, 100);
	SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);

	GIVE_WEAPON_TO_PED(ped, GET_HASH_KEY("WEAPON_PISTOL"), 9999, false, true);
	GIVE_WEAPON_TO_PED(ped, GET_HASH_KEY("WEAPON_CARBINERIFLE"), 9999, false, true);
}

static void Brad() {
	static const Hash model = GET_HASH_KEY("ig_brad");

	Hash relationshipGroup;
	ADD_RELATIONSHIP_GROUP("_COMPANION_BRAD", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, relationshipGroup, GET_HASH_KEY("PLAYER"));
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, GET_HASH_KEY("PLAYER"), relationshipGroup);

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	Ped ped = CreatePoolPed(4, model, playerPos.x, playerPos.y, playerPos.z, GET_ENTITY_HEADING(playerPed));
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		SET_PED_INTO_VEHICLE(ped, GET_VEHICLE_PED_IS_IN(playerPed, false), -2);
	}

	SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
	SET_PED_HEARING_RANGE(ped, 9999.f);

	SET_PED_AS_GROUP_MEMBER(ped, GET_PLAYER_GROUP(PLAYER_ID()));

	GIVE_WEAPON_TO_PED(ped, GET_HASH_KEY("WEAPON_MICROSMG"), 9999, true, true);

	SET_PED_ACCURACY(ped, 100);
	SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);
}

static void Chop()
{
	static const Hash modelHash = GET_HASH_KEY("a_c_chop");

	Hash relationshipGroup;
	ADD_RELATIONSHIP_GROUP("_COMPANION_CHOP", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, relationshipGroup, GET_HASH_KEY("PLAYER"));
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, GET_HASH_KEY("PLAYER"), relationshipGroup);

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	Ped ped = CreatePoolPed(28, modelHash, playerPos.x, playerPos.y, playerPos.z, GET_ENTITY_HEADING(playerPed));
	SET_PED_COMBAT_ATTRIBUTES(ped, 0, false);
	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
	SET_PED_HEARING_RANGE(ped, 9999.f);

	SET_PED_AS_GROUP_MEMBER(ped, GET_PLAYER_GROUP(PLAYER_ID()));
}

static void Ballas() {
	static const std::vector<int> ballaNames = { -198252413, 588969535, 361513884, -1492432238, -1410400252, 599294057 };

	Hash relationshipGroup;
	ADD_RELATIONSHIP_GROUP("_ENEMY_BALLAS", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, GET_HASH_KEY("PLAYER"));
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, GET_HASH_KEY("PLAYER"), relationshipGroup);

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);
	float heading = GET_ENTITY_HEADING(IS_PED_IN_ANY_VEHICLE(playerPed, false) ? GET_VEHICLE_PED_IS_IN(playerPed, false) : playerPed);
	static const Hash carModel = GET_HASH_KEY("Virgo2");

	Vehicle veh = CreatePoolVehicle(carModel, playerPos.x, playerPos.y, playerPos.z, heading);
	SET_VEHICLE_COLOURS(veh, 148, 148);
	SET_VEHICLE_ENGINE_ON(veh, true, true, false);

	WAIT(0);
	static const Hash microSmgHash = GET_HASH_KEY("WEAPON_MICROSMG");
	for (int i = 0; i < 2; i++)
	{
		Hash choosenPropHash = ballaNames.at(g_random.GetRandomInt(0, ballaNames.size() - 1));
		Ped ped = CreatePoolPed(4, choosenPropHash, playerPos.x, playerPos.y, playerPos.z, GET_ENTITY_HEADING(playerPed));
		if (i == 0)
		{
			SET_PED_INTO_VEHICLE(ped, veh, -1);
		}
		else
		{
			SET_PED_INTO_VEHICLE(ped, veh, -2);
		}
		SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
		SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
		SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
		SET_PED_HEARING_RANGE(ped, 9999.f);
		GIVE_WEAPON_TO_PED(ped, microSmgHash, 9999, true, true);
		SET_PED_ACCURACY(ped, 50);
		TASK_COMBAT_PED(ped, playerPed, 0, 16);
	}
}

static void Agent() {
	Ped playerPed = PLAYER_PED_ID();

	static const Hash model = GET_HASH_KEY("cs_milton");

	static const Hash playerGroup = GET_HASH_KEY("PLAYER");

	Hash relationshipGroup;
	ADD_RELATIONSHIP_GROUP("_HOSTILE_BOND", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);

	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	float heading = GET_ENTITY_HEADING(IS_PED_IN_ANY_VEHICLE(playerPed, false) ? GET_VEHICLE_PED_IS_IN(playerPed, false) : playerPed);

	float x_pos = sin((360 - heading) * PI / 180) * 10;
	float y_pos = cos((360 - heading) * PI / 180) * 10;

	Vehicle veh = CreatePoolVehicle(GET_HASH_KEY("JB700"), playerPos.x - x_pos, playerPos.y - y_pos, playerPos.z, heading);
	SET_VEHICLE_ENGINE_ON(veh, true, true, false);

	Vector3 vel = GET_ENTITY_VELOCITY(playerPed);
	SET_ENTITY_VELOCITY(veh, vel.x, vel.y, vel.z);

	Ped bond = CreatePoolPedInsideVehicle(veh, 4, model, -1);

	SET_PED_RELATIONSHIP_GROUP_HASH(bond, relationshipGroup);

	TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(bond, true);

	SET_PED_HEARING_RANGE(bond, 9999.f);
	SET_PED_CONFIG_FLAG(bond, 281, true);

	SET_PED_COMBAT_ATTRIBUTES(bond, 5, true);
	SET_PED_COMBAT_ATTRIBUTES(bond, 46, true);

	SET_PED_SUFFERS_CRITICAL_HITS(bond, false);

	GIVE_WEAPON_TO_PED(bond, GET_HASH_KEY("WEAPON_SWITCHBLADE"), 9999, true, true);
	GIVE_WEAPON_TO_PED(bond, GET_HASH_KEY("WEAPON_VINTAGEPISTOL"), 9999, true, true);
	GIVE_WEAPON_COMPONENT_TO_PED(bond, GET_HASH_KEY("WEAPON_VINTAGEPISTOL"), GET_HASH_KEY("COMPONENT_AT_PI_SUPP"));
	SET_PED_ACCURACY(bond, 100);
	TASK_COMBAT_PED(bond, playerPed, 0, 16);
}

static void Enemy() {
	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	static const Hash civGroup = GET_HASH_KEY("CIVMALE");
	static const Hash femCivGroup = GET_HASH_KEY("CIVFEMALE");

	Hash relationshipGroup;
	ADD_RELATIONSHIP_GROUP("_HOSTILE_RANDOM", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, civGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, femCivGroup);

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	Ped ped = CreateRandomPoolPed(playerPos.x, playerPos.y, playerPos.z, GET_ENTITY_HEADING(playerPed));
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		SET_PED_INTO_VEHICLE(ped, GET_VEHICLE_PED_IS_IN(playerPed, false), -2);
	}

	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
	SET_PED_HEARING_RANGE(ped, 9999.f);
	SET_PED_CONFIG_FLAG(ped, 281, true);

	SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
	SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
	SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

	SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

	GIVE_WEAPON_TO_PED(ped, GET_SELECTED_PED_WEAPON(playerPed), 9999, true, true);

	SET_PED_ACCURACY(ped, 100);
	SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);
}


static void OnStart()
{
	Chimp();
	Brad();
	Chop();

	Ballas();
	Agent();
	Enemy();
}


static RegisterEffect registerEffect(EFFECT_FIGHT_CLUB, OnStart, nullptr, nullptr, EffectInfo
	{
		.Name = "Fight Club",
		.Id = "misc_fight_club",
		.IsTimed = true,
		.IsShortDuration = true
	}
);