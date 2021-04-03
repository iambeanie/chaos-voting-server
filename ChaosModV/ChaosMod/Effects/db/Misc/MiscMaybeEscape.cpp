#include <stdafx.h>
#include <math.h>  

#define PI 3.14159265

static void OnStopInvincible()
{
	SET_PLAYER_INVINCIBLE(PLAYER_ID(), false);
}

static void OnTickInvincible()
{
	SET_PLAYER_INVINCIBLE(PLAYER_ID(), true);
}

static void OnTickOneBullet()
{
	for (Ped ped : GetAllPeds())
	{
		if (IS_PED_ARMED(ped, 7)) // 7 = anything but fists
		{
			Hash weaponHash;
			GET_CURRENT_PED_WEAPON(ped, &weaponHash, 1);
			int ammo;
			GET_AMMO_IN_CLIP(ped, weaponHash, &ammo);
			if (ammo > 1)
			{
				int diff = ammo - 1;
				ADD_AMMO_TO_PED(ped, weaponHash, diff);
				SET_AMMO_IN_CLIP(ped, weaponHash, 1);
			}
		}
	}
}

static void OnStartRandomEnemy()
{
	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	static const Hash civGroup = GET_HASH_KEY("CIVMALE");
	static const Hash femCivGroup = GET_HASH_KEY("CIVFEMALE");

	Hash relationshipGroupRandom;
	ADD_RELATIONSHIP_GROUP("_HOSTILE_RANDOM", &relationshipGroupRandom);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupRandom, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupRandom, civGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupRandom, femCivGroup);

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	Ped ped = CreateRandomPoolPed(playerPos.x, playerPos.y, playerPos.z, GET_ENTITY_HEADING(playerPed));
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		SET_PED_INTO_VEHICLE(ped, GET_VEHICLE_PED_IS_IN(playerPed, false), -2);
	}

	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroupRandom);
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

static void OnStartExtremeJesus()
{
	static constexpr Hash modelHash = -835930287;

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	static const Hash oppressorHash = GET_HASH_KEY("OPPRESSOR2");
	LoadModel(oppressorHash);

	float heading = GET_ENTITY_HEADING(IS_PED_IN_ANY_VEHICLE(playerPed, false) ? GET_VEHICLE_PED_IS_IN(playerPed, false) : playerPed);

	Vehicle veh = CreatePoolVehicle(oppressorHash, playerPos.x, playerPos.y, playerPos.z, heading);
	SET_VEHICLE_ENGINE_ON(veh, true, true, false);
	SET_VEHICLE_MOD_KIT(veh, 0);

	for (int i = 0; i < 50; i++)
	{
		int max = GET_NUM_VEHICLE_MODS(veh, i);
		SET_VEHICLE_MOD(veh, i, max > 0 ? max - 1 : 0, false);
	}

	SET_ENTITY_PROOFS(veh, false, true, true, false, false, false, false, false);
	SET_MODEL_AS_NO_LONGER_NEEDED(oppressorHash);

	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	static const Hash civGroup = GET_HASH_KEY("CIVMALE");
	static const Hash femCivGroup = GET_HASH_KEY("CIVFEMALE");

	Hash relationshipGroupBetterJesus;
	ADD_RELATIONSHIP_GROUP("_HOSTILE_JESUS2", &relationshipGroupBetterJesus);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupBetterJesus, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupBetterJesus, civGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupBetterJesus, femCivGroup);

	Ped ped = CreatePoolPedInsideVehicle(veh, 4, modelHash, -1);
	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroupBetterJesus);
	SET_PED_HEARING_RANGE(ped, 9999.f);
	SET_PED_CONFIG_FLAG(ped, 281, true);

	SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
	SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

	SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

	SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 1);
	SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
	SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
	SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

	GIVE_WEAPON_TO_PED(ped, GET_HASH_KEY("WEAPON_RAILGUN"), 9999, true, true);
	TASK_COMBAT_PED(ped, playerPed, 0, 16);

	SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);
}

static void OnStartJesus()
{
	static constexpr Hash modelHash = -835930287;

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	static const Hash civGroup = GET_HASH_KEY("CIVMALE");
	static const Hash femCivGroup = GET_HASH_KEY("CIVFEMALE");

	Hash relationshipGroupJesus;
	ADD_RELATIONSHIP_GROUP("_HOSTILE_JESUS", &relationshipGroupJesus);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupJesus, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupJesus, civGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupJesus, femCivGroup);

	Ped ped = CreatePoolPed(4, modelHash, playerPos.x, playerPos.y, playerPos.z, 0.f);
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		SET_PED_INTO_VEHICLE(ped, GET_VEHICLE_PED_IS_IN(playerPed, false), -2);
	}

	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroupJesus);
	SET_PED_HEARING_RANGE(ped, 9999.f);
	SET_PED_CONFIG_FLAG(ped, 281, true);

	SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

	SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

	SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
	SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
	SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

	GIVE_WEAPON_TO_PED(ped, GET_HASH_KEY("WEAPON_RAILGUN"), 9999, true, true);
	TASK_COMBAT_PED(ped, playerPed, 0, 16);

	SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);
}

static void OnStartJimmy()
{
	static constexpr Hash modelHash = 1459905209;

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	static const Hash civGroup = GET_HASH_KEY("CIVMALE");
	static const Hash femCivGroup = GET_HASH_KEY("CIVFEMALE");

	Hash relationshipGroupJimmy;
	ADD_RELATIONSHIP_GROUP("_HOSTILE_JIMMY", &relationshipGroupJimmy);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupJimmy, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupJimmy, civGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupJimmy, femCivGroup);

	Ped ped = CreatePoolPed(4, modelHash, playerPos.x, playerPos.y, playerPos.z, 0.f);
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		SET_PED_INTO_VEHICLE(ped, GET_VEHICLE_PED_IS_IN(playerPed, false), -2);
	}

	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroupJimmy);
	SET_PED_HEARING_RANGE(ped, 9999.f);
	SET_PED_CONFIG_FLAG(ped, 281, true);

	SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

	SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

	SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
	SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
	SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

	GIVE_WEAPON_TO_PED(ped, GET_HASH_KEY("WEAPON_COMBATMG"), 9999, true, true);
	TASK_COMBAT_PED(ped, playerPed, 0, 16);

	SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);
}

static void OnStartAlien()
{
	static const Hash alienHash = GET_HASH_KEY("s_m_m_movalien_01");

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	static const Hash civGroup = GET_HASH_KEY("CIVMALE");
	static const Hash femCivGroup = GET_HASH_KEY("CIVFEMALE");

	Hash relationshipGroup;
	ADD_RELATIONSHIP_GROUP("_HOSTILE_ALIEN", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, civGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, femCivGroup);

	Ped ped = CreatePoolPed(4, alienHash, playerPos.x, playerPos.y, playerPos.z, 0.f);
	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
	SET_PED_HEARING_RANGE(ped, 9999.f);
	SET_PED_CONFIG_FLAG(ped, 281, true);

	SET_PED_COMPONENT_VARIATION(ped, 0, 0, 0, 0);
	SET_PED_COMPONENT_VARIATION(ped, 3, 0, 0, 0);
	SET_PED_COMPONENT_VARIATION(ped, 4, 0, 0, 0);
	SET_PED_COMPONENT_VARIATION(ped, 5, 0, 0, 0);
	SET_PED_COMPONENT_VARIATION(ped, 6, 0, 0, 0);

	SET_ENTITY_HEALTH(ped, 500, 0);
	SET_PED_ARMOUR(ped, 500);
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		SET_PED_INTO_VEHICLE(ped, GET_VEHICLE_PED_IS_IN(playerPed, false), -2);
	}

	SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 0, true);

	SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
	SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
	SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

	GIVE_WEAPON_TO_PED(ped, GET_HASH_KEY("WEAPON_RAYPISTOL"), 9999, true, true); // give the alien an up n atomizer
	TASK_COMBAT_PED(ped, playerPed, 0, 16);

	SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);
}

struct EnemyGroup
{
	Vehicle vehicle;
	std::vector<Ped> peds;
};

static Hash model;
static Hash microSmgHash;
static Hash relationshipGroup;
static EnemyGroup helicopterGroup;
static EnemyGroup mesaGroup;


static Vector3 getRandomOffsetCoord(Vector3 startCoord, float minOffset, float maxOffset)
{
	Vector3 randomCoord;
	if (g_random.GetRandomInt(0, 1) % 2 == 0)
	{
		randomCoord.x = startCoord.x + g_random.GetRandomInt(minOffset, maxOffset);
	}
	else
	{
		randomCoord.x = startCoord.x - g_random.GetRandomInt(minOffset, maxOffset);
	}
	if (g_random.GetRandomInt(0, 1) % 2 == 0)
	{
		randomCoord.y = startCoord.y + g_random.GetRandomInt(minOffset, maxOffset);
	}
	else
	{
		randomCoord.y = startCoord.y - g_random.GetRandomInt(minOffset, maxOffset);
	}
	randomCoord.z = startCoord.z;
	return randomCoord;
}

static void fillVehicleWithPeds(Vehicle veh, Ped playerPed, Hash relationshipGroup, Hash modelHash, Hash weaponHash, std::vector<Ped>& listToAddPedTo, bool canExitVehicle)
{
	for (int seatPos = -1; seatPos < 3; seatPos++)
	{
		LoadModel(modelHash);
		Ped ped = CREATE_PED_INSIDE_VEHICLE(veh, -1, modelHash, seatPos, true, false);
		SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);

		SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
		SET_PED_HEARING_RANGE(ped, 9999.f);

		GIVE_WEAPON_TO_PED(ped, weaponHash, 9999, true, true);
		SET_PED_ACCURACY(ped, 50);

		SET_PED_COMBAT_ATTRIBUTES(ped, 0, true);
		SET_PED_COMBAT_ATTRIBUTES(ped, 1, true);
		SET_PED_COMBAT_ATTRIBUTES(ped, 2, true);
		SET_PED_COMBAT_ATTRIBUTES(ped, 3, canExitVehicle);
		SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
		SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

		REGISTER_TARGET(ped, playerPed);
		TASK_COMBAT_PED(ped, playerPed, 0, 16);
		listToAddPedTo.push_back(ped);
		WAIT(0);
	}
}


static void spawnBuzzard()
{
	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);
	Hash buzzardHash = GET_HASH_KEY("BUZZARD");
	Vector3 spawnPoint = getRandomOffsetCoord(playerPos, 200, 250);
	float xDiff = playerPos.x - spawnPoint.x;
	float yDiff = playerPos.y - spawnPoint.y;
	float heading = GET_HEADING_FROM_VECTOR_2D(xDiff, yDiff);
	helicopterGroup = EnemyGroup();
	LoadModel(buzzardHash);
	helicopterGroup.vehicle = CREATE_VEHICLE(buzzardHash, spawnPoint.x, spawnPoint.y, spawnPoint.z + 50, heading, true, false, false);
	SET_VEHICLE_COLOURS(helicopterGroup.vehicle, 0, 0);
	SET_VEHICLE_ENGINE_ON(helicopterGroup.vehicle, true, true, true);
	SET_VEHICLE_FORWARD_SPEED(helicopterGroup.vehicle, 0); // Needed, so the heli doesn't fall down instantly
	SET_VEHICLE_CHEAT_POWER_INCREASE(helicopterGroup.vehicle, 2); // Make it easier to catch up
	fillVehicleWithPeds(helicopterGroup.vehicle, playerPed, relationshipGroup, model, microSmgHash, helicopterGroup.peds, false);
}


static void spawnMesa()
{
	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);
	Vector3 spawnPoint;
	// Try spawning on a vehicle node, fall back to random coord
	int nodeId;
	if (!GET_RANDOM_VEHICLE_NODE(playerPos.x, playerPos.y, playerPos.z, 150, false, false, false, &spawnPoint, &nodeId))
	{
		spawnPoint = getRandomOffsetCoord(playerPos, 50, 50);
		float groundZ;
		if (GET_GROUND_Z_FOR_3D_COORD(spawnPoint.x, spawnPoint.y, spawnPoint.z, &groundZ, false, false))
		{
			spawnPoint.z = groundZ;
		}
	}
	float xDiff = playerPos.x - spawnPoint.x;
	float yDiff = playerPos.y - spawnPoint.y;
	float heading = GET_HEADING_FROM_VECTOR_2D(xDiff, yDiff);
	Hash mesaHash = GET_HASH_KEY("Mesa3");
	mesaGroup = EnemyGroup();
	LoadModel(mesaHash);
	mesaGroup.vehicle = CREATE_VEHICLE(mesaHash, spawnPoint.x, spawnPoint.y, spawnPoint.z + 5, heading, true, false, false);
	SET_VEHICLE_ON_GROUND_PROPERLY(mesaGroup.vehicle, 5);
	SET_VEHICLE_COLOURS(mesaGroup.vehicle, 0, 0);
	SET_VEHICLE_ENGINE_ON(mesaGroup.vehicle, true, true, true);
	SET_VEHICLE_CHEAT_POWER_INCREASE(mesaGroup.vehicle, 2); // Make it easier to catch up
	fillVehicleWithPeds(mesaGroup.vehicle, playerPed, relationshipGroup, model, microSmgHash, mesaGroup.peds, true);
}

static void OnStartMercs()
{
	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	model = GET_HASH_KEY("csb_mweather");
	microSmgHash = GET_HASH_KEY("WEAPON_MICROSMG");

	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	ADD_RELATIONSHIP_GROUP("_HOSTILE_MERRYWEATHER", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroup, relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, relationshipGroup, relationshipGroup);

	spawnBuzzard();
	spawnMesa();
}

static void OnStopMercs()
{
	SET_VEHICLE_AS_NO_LONGER_NEEDED(&helicopterGroup.vehicle);
	for (Ped ped : helicopterGroup.peds)
	{
		SET_PED_AS_NO_LONGER_NEEDED(&ped);
	}
	SET_VEHICLE_AS_NO_LONGER_NEEDED(&mesaGroup.vehicle);
	for (Ped ped : mesaGroup.peds)
	{
		SET_PED_AS_NO_LONGER_NEEDED(&ped);
	}
}

static bool checkPedsAlive(std::vector<Ped> pedList)
{
	bool allDead = true;
	Ped player = PLAYER_PED_ID();
	for (Ped ped : pedList)
	{
		if (!DOES_ENTITY_EXIST(ped) || IS_PED_DEAD_OR_DYING(ped, false))
		{
			SET_ENTITY_HEALTH(ped, 0, false);
			SET_PED_AS_NO_LONGER_NEEDED(&ped);
		}
		else
		{
			allDead = false;
			Vector3 playerPos = GET_ENTITY_COORDS(PLAYER_PED_ID(), false);
			Vector3 enemyPos = GET_ENTITY_COORDS(ped, false);
			if (GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, enemyPos.x, enemyPos.y, enemyPos.z, false) > 350)
			{
				SET_ENTITY_HEALTH(ped, 0, false);
				SET_PED_AS_NO_LONGER_NEEDED(&ped);
			}
			else
			{
				TASK_COMBAT_PED(ped, player, 0, 16);
			}
		}
	}
	return allDead;
}

static void OnTickMercs()
{
	bool allHelicopterDead = checkPedsAlive(helicopterGroup.peds);
	if (allHelicopterDead)
	{
		helicopterGroup.peds.clear();
		SET_VEHICLE_AS_NO_LONGER_NEEDED(&helicopterGroup.vehicle);
		spawnBuzzard();
	}

	bool allVanDead = checkPedsAlive(mesaGroup.peds);
	if (allVanDead)
	{
		mesaGroup.peds.clear();
		SET_VEHICLE_AS_NO_LONGER_NEEDED(&mesaGroup.vehicle);
		spawnMesa();
	}
}

static std::list<Ped> clownEnemies;
static int spawnTimer = -1;
static Hash relationshipGroupClowns;
static int maxClownsToSpawn = 3;

static Vector3 getRandomOffsetCoord(Vector3 startCoord, int minOffset, int maxOffset)
{
	Vector3 randomCoord;
	randomCoord.z = startCoord.z;
	float groundZ;
	for (int i = 0; i < 10; i++) {
		if (g_random.GetRandomInt(0, 1) % 2 == 0)
		{
			randomCoord.x = startCoord.x + g_random.GetRandomInt(minOffset, maxOffset);
		}
		else
		{
			randomCoord.x = startCoord.x - g_random.GetRandomInt(minOffset, maxOffset);
		}
		if (g_random.GetRandomInt(0, 1) % 2 == 0)
		{
			randomCoord.y = startCoord.y + g_random.GetRandomInt(minOffset, maxOffset);
		}
		else
		{
			randomCoord.y = startCoord.y - g_random.GetRandomInt(minOffset, maxOffset);
		}
		randomCoord.z = startCoord.z;
		if (GET_GROUND_Z_FOR_3D_COORD(randomCoord.x, randomCoord.y, randomCoord.z, &groundZ, false, false))
		{
			randomCoord.z = groundZ;
			break;
		}
	}
	return randomCoord;
}

static void OnStopClowns()
{
	REMOVE_NAMED_PTFX_ASSET("scr_rcbarry2");

	for (Ped ped : clownEnemies)
	{
		SET_PED_AS_NO_LONGER_NEEDED(&ped);
	}

	clownEnemies.clear();
}

static void OnStartClowns()
{
	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	ADD_RELATIONSHIP_GROUP("_HOSTILE_KILLER_CLOWNS", &relationshipGroupClowns);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupClowns, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroup, relationshipGroupClowns);
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, relationshipGroupClowns, relationshipGroupClowns);
}

static void OnTickClowns()
{
	REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
	while (!HAS_NAMED_PTFX_ASSET_LOADED("scr_rcbarry2"))
	{
		WAIT(0);
	}

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);
	int current_time = GET_GAME_TIMER();

	for (std::list<Entity>::iterator it = clownEnemies.begin(); it != clownEnemies.end(); )
	{
		Ped clown = *it;
		Vector3 clownPos = GET_ENTITY_COORDS(clown, false);
		if (IS_PED_DEAD_OR_DYING(clown, false) || IS_PED_INJURED(clown) || GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, clownPos.x, clownPos.y, clownPos.z, false) > 100.f)
		{
			SET_ENTITY_HEALTH(clown, 0, 0);
			USE_PARTICLE_FX_ASSET("scr_rcbarry2");
			START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_clown_death", clownPos.x, clownPos.y, clownPos.z, 0, 0, 0, 3, false, false, false);
			WAIT(300);
			SET_ENTITY_ALPHA(clown, 0, true);
			SET_PED_AS_NO_LONGER_NEEDED(&clown);
			DELETE_PED(&clown);
			it = clownEnemies.erase(it);
			WAIT(0);
		}
		else
		{
			it++;
		}
	}

	if (clownEnemies.size() < maxClownsToSpawn && current_time > spawnTimer + 2000)
	{
		spawnTimer = current_time;
		Vector3 spawnPos = getRandomOffsetCoord(playerPos, 10, 25);
		USE_PARTICLE_FX_ASSET("scr_rcbarry2");
		START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_clown_appears", spawnPos.x, spawnPos.y, spawnPos.z, 0, 0, 0, 2, true, true, true);
		WAIT(300);
		Hash clownHash = GET_HASH_KEY("s_m_y_clown_01");
		Hash weaponHash = GET_HASH_KEY("WEAPON_MICROSMG");
		LoadModel(clownHash);
		Ped ped = CREATE_PED(-1, clownHash, spawnPos.x, spawnPos.y, spawnPos.z, 0, true, false);
		SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
		SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroupClowns);
		SET_PED_HEARING_RANGE(ped, 9999.f);
		GIVE_WEAPON_TO_PED(ped, weaponHash, 9999, true, true);
		SET_PED_ACCURACY(ped, 20);
		TASK_COMBAT_PED(ped, playerPed, 0, 16);
		clownEnemies.push_back(ped);
	}
}



static void OnStartJamesBond()
{
	Ped playerPed = PLAYER_PED_ID();

	static const Hash model = GET_HASH_KEY("cs_milton");

	static const Hash playerGroup = GET_HASH_KEY("PLAYER");

	Hash relationshipGroupBond;
	ADD_RELATIONSHIP_GROUP("_HOSTILE_BOND", &relationshipGroupBond);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroupBond, playerGroup);

	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	float heading = GET_ENTITY_HEADING(IS_PED_IN_ANY_VEHICLE(playerPed, false) ? GET_VEHICLE_PED_IS_IN(playerPed, false) : playerPed);

	float x_pos = sin((360 - heading) * PI / 180) * 10;
	float y_pos = cos((360 - heading) * PI / 180) * 10;

	Vehicle veh = CreatePoolVehicle(GET_HASH_KEY("JB700"), playerPos.x - x_pos, playerPos.y - y_pos, playerPos.z, heading);
	SET_VEHICLE_ENGINE_ON(veh, true, true, false);

	Vector3 vel = GET_ENTITY_VELOCITY(playerPed);
	SET_ENTITY_VELOCITY(veh, vel.x, vel.y, vel.z);

	Ped bond = CreatePoolPedInsideVehicle(veh, 4, model, -1);

	SET_PED_RELATIONSHIP_GROUP_HASH(bond, relationshipGroupBond);

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

static void OnTick()
{
	OnTickInvincible();
	OnTickOneBullet();
	OnTickMercs();
	OnTickClowns();
}

static void OnStop()
{
	OnStopInvincible();
	OnStopMercs();
	OnStopClowns();
}

static void OnStart()
{
	OnStartRandomEnemy();
	OnStartExtremeJesus();
	OnStartJesus();
	OnStartJimmy();
	OnStartAlien();
	OnStartMercs();
	OnStartClowns();
	OnStartJamesBond();
}

static RegisterEffect registerEffect(EFFECT_MAYBE_ESCAPE, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Maybe escape",
		.Id = "misc_maybe_escape",
		.IsTimed = true
	}
);