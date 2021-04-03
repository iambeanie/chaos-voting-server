#include <stdafx.h>

static std::list<Ped> m_zombies;

static std::list<Ped> clownEnemies;
static int spawnTimer = -1;
static Hash relationshipGroup;
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


static void OnStart()
{
	m_zombies.clear();

	static const Hash playerGroupHash = GET_HASH_KEY("PLAYER");
	static const Hash civMaleGroupHash = GET_HASH_KEY("CIVMALE");
	static const Hash civFemaleGroupHash = GET_HASH_KEY("CIVFEMALE");

	Hash groupHash;
	ADD_RELATIONSHIP_GROUP("_ZOMBIES", &groupHash);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, groupHash, playerGroupHash);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, groupHash, civMaleGroupHash);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, groupHash, civFemaleGroupHash);

	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	ADD_RELATIONSHIP_GROUP("_HOSTILE_KILLER_CLOWNS", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroup, relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, relationshipGroup, relationshipGroup);
}

static void OnStop()
{
	SET_MAX_WANTED_LEVEL(5);

	for (Ped ped : m_zombies)
	{
		if (DOES_ENTITY_EXIST(ped))
		{
			SET_PED_AS_NO_LONGER_NEEDED(&ped);
		}
	}

	REMOVE_NAMED_PTFX_ASSET("scr_rcbarry2");

	for (Ped ped : clownEnemies)
	{
		SET_PED_AS_NO_LONGER_NEEDED(&ped);
	}

	clownEnemies.clear();
}

static void Zombie() {
	static constexpr int MAX_ZOMBIES = 20;
	static constexpr Hash MODEL_HASH = -1404353274;

	static Hash zombieGroupHash = GET_HASH_KEY("_ZOMBIES");

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	SET_PLAYER_WANTED_LEVEL(PLAYER_ID(), 0, false);
	SET_MAX_WANTED_LEVEL(0);

	if (m_zombies.size() <= MAX_ZOMBIES)
	{
		Vector3 spawnPos;
		if (GET_NTH_CLOSEST_VEHICLE_NODE(playerPos.x, playerPos.y, playerPos.z, 10 + m_zombies.size(), &spawnPos, 0, 0, 0)
			&& GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, spawnPos.x, spawnPos.y, spawnPos.z, false) < 300.f)
		{
			LoadModel(MODEL_HASH);

			Ped zombie = CREATE_PED(26, MODEL_HASH, spawnPos.x, spawnPos.y, spawnPos.z, .0f, true, false);

			m_zombies.push_back(zombie);

			SET_PED_RELATIONSHIP_GROUP_HASH(zombie, zombieGroupHash);
			SET_PED_COMBAT_ATTRIBUTES(zombie, 5, true);
			SET_PED_COMBAT_ATTRIBUTES(zombie, 46, true);

			//SET_AMBIENT_VOICE_NAME(zombie, "ALIENS");
			DISABLE_PED_PAIN_AUDIO(zombie, true);

			TASK_COMBAT_PED(zombie, playerPed, 0, 16);

			SET_MODEL_AS_NO_LONGER_NEEDED(MODEL_HASH);
		}
	}
}

static void Clowns() {
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
		SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
		SET_PED_HEARING_RANGE(ped, 9999.f);
		GIVE_WEAPON_TO_PED(ped, weaponHash, 9999, true, true);
		SET_PED_ACCURACY(ped, 20);
		TASK_COMBAT_PED(ped, playerPed, 0, 16);
		clownEnemies.push_back(ped);
	}
}

static void Clip() {
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

static void OnTick()
{
	Zombie();
	Clowns();	
	Clip();
}

static RegisterEffect registerEffect(EFFECT_PANIC, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Panic!!",
		.Id = "misc_panic",
		.IsTimed = true,
		.IsShortDuration = true
	}
);