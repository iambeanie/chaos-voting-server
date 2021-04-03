#include <stdafx.h>

static const Hash playerGroup = GET_HASH_KEY("PLAYER");
static const Hash civGroup = GET_HASH_KEY("CIVMALE");
static const Hash femCivGroup = GET_HASH_KEY("CIVFEMALE");
Hash relationshipGroup;

static Ped CreateHostilePed(Vector3 position) {
	
	Ped ped = CreateRandomPoolPed(position.x, position.y, position.z, g_random.GetRandomFloat(0, 360));

	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
	SET_PED_HEARING_RANGE(ped, 9999.f);
	SET_PED_CONFIG_FLAG(ped, 281, true);

	SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
	SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
	SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

	SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

	SET_PED_ACCURACY(ped, 100);
	SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);

	return ped;
}

static void OnStart()
{
	Ped playerPed = PLAYER_PED_ID();

	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);

		std::vector<Ped> vehPeds;

		Hash vehModel = GET_ENTITY_MODEL(playerVeh);
		int maxSeats = GET_VEHICLE_MODEL_NUMBER_OF_SEATS(vehModel);
		for (int i = -1; i < maxSeats - 1; i++)
		{
			if (IS_VEHICLE_SEAT_FREE(playerVeh, i, false))
			{
				continue;
			}

			Ped ped = GET_PED_IN_VEHICLE_SEAT(playerVeh, i, false);

			CLEAR_PED_TASKS_IMMEDIATELY(ped);

			SET_PED_TO_RAGDOLL(ped, 5000, 5000, 0, true, true, false);

			vehPeds.push_back(ped);
		}

		Vector3 vehVel = GET_ENTITY_VELOCITY(playerVeh);

		SET_ENTITY_AS_MISSION_ENTITY(playerVeh, true, true);
		DELETE_VEHICLE(&playerVeh);

		WAIT(0);
	}

	ADD_RELATIONSHIP_GROUP("_HOSTILE_RANDOM", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, civGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, femCivGroup);

	Vector3 spawnCoords = GET_ENTITY_COORDS(playerPed, false);

	spawnCoords.z += 2000.0f;

	TeleportPlayer(spawnCoords);

	Vehicle playerHydra = CreatePoolVehicle(GET_HASH_KEY("HYDRA"), spawnCoords.x, spawnCoords.y, spawnCoords.z, GET_ENTITY_HEADING(PLAYER_PED_ID()));

	SET_PED_INTO_VEHICLE(playerPed, playerHydra, -1);

	for (int i = 0; i < 10; i++) {
		float x = g_random.GetRandomFloat(50.0f, 100.0f);

		if (g_random.GetRandomInt(0, 1) % 2 == 0) {
			x *= -1;
		}

		float y = g_random.GetRandomFloat(50.0f, 100.0f);

		if (g_random.GetRandomInt(0, 1) % 2 == 0) {
			y *= -1;
		}

		float z = g_random.GetRandomFloat(50.0f, 100.0f);

		if (g_random.GetRandomInt(0, 1) % 2 == 0) {
			z *= -1;
		}

		Vector3 enemyHydraLocation = Vector3(spawnCoords.x + x, spawnCoords.y + y, spawnCoords.z + z);

		Vehicle hostileHydra = CreatePoolVehicle(GET_HASH_KEY("HYDRA"), enemyHydraLocation.x, enemyHydraLocation.y, enemyHydraLocation.z, GET_ENTITY_HEADING(PLAYER_PED_ID()));

		Ped hostile = CreateHostilePed(enemyHydraLocation);

		SET_PED_INTO_VEHICLE(hostile, hostileHydra, -1);
	}
}

static RegisterEffect registerEffect(EFFECT_TOP_GUN, OnStart, nullptr, nullptr, EffectInfo
	{
		.Name = "Top Gun",
		.Id = "misc_top_gun"
	}
);