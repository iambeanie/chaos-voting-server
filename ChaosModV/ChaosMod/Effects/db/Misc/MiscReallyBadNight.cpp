#include <stdafx.h>

static void OnStopDrunk()
{
	Ped playerPed = PLAYER_PED_ID();

	SET_PED_IS_DRUNK(playerPed, false);

	RESET_PED_MOVEMENT_CLIPSET(playerPed, .0f);

	REMOVE_CLIP_SET("MOVE_M@DRUNK@VERYDRUNK");

	STOP_GAMEPLAY_CAM_SHAKING(true);

	_0x487A82C650EB7799(.0f);
	_0x0225778816FDC28C(.0f);
}

static void OnTickDrunk()
{
	if (!IS_GAMEPLAY_CAM_SHAKING())
	{
		SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", 2.f);
	}

	Ped playerPed = PLAYER_PED_ID();

	SET_PED_IS_DRUNK(playerPed, true);

	REQUEST_CLIP_SET("MOVE_M@DRUNK@VERYDRUNK");
	SET_PED_MOVEMENT_CLIPSET(playerPed, "MOVE_M@DRUNK@VERYDRUNK", 1.f);

	SET_AUDIO_SPECIAL_EFFECT_MODE(2);

	// No idea what these do, but game scripts also call these so just blindly follow
	_0x487A82C650EB7799(1.f);
	_0x0225778816FDC28C(1.f);

	// Random right / left steering
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (GET_PED_IN_VEHICLE_SEAT(playerVeh, -1, 0) != playerPed)
		{
			return;
		}

		static DWORD64 timeUntilSteer = GET_GAME_TIMER();
		static bool enableDrunkSteering = false;
		static float steering;

		if (enableDrunkSteering)
		{
			SET_VEHICLE_STEER_BIAS(playerVeh, steering);
		}

		DWORD64 curTick = GET_GAME_TIMER();

		if (timeUntilSteer < curTick)
		{
			timeUntilSteer = GET_GAME_TIMER();

			if (enableDrunkSteering)
			{
				// Give player back control

				timeUntilSteer += g_random.GetRandomInt(100, 500);
			}
			else
			{
				// Take control from player

				steering = GET_RANDOM_FLOAT_IN_RANGE(-.5f, .5f);

				timeUntilSteer += g_random.GetRandomInt(50, 300);
			}

			enableDrunkSteering = !enableDrunkSteering;
		}
	}
}

static void OnStopSensitive()
{
	for (auto ped : GetAllPeds())
	{
		SET_PED_RAGDOLL_ON_COLLISION(ped, false);
	}
}

static void OnTickSensitive()
{
	for (auto ped : GetAllPeds())
	{
		SET_PED_RAGDOLL_ON_COLLISION(ped, true);
	}
}

static std::list<Ped> m_zombies;

static void OnStartZombies()
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
}

static void OnStopZombies()
{
	SET_MAX_WANTED_LEVEL(5);

	for (Ped ped : m_zombies)
	{
		if (DOES_ENTITY_EXIST(ped))
		{
			SET_PED_AS_NO_LONGER_NEEDED(&ped);
		}
	}
}

static void OnTickZombies()
{
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

	for (std::list<Ped>::iterator it = m_zombies.begin(); it != m_zombies.end(); )
	{
		Ped zombie = *it;

		if (DOES_ENTITY_EXIST(zombie))
		{
			Vector3 zombiePos = GET_ENTITY_COORDS(zombie, false);
			if (GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, zombiePos.x, zombiePos.y, zombiePos.z, false) < 300.f)
			{
				int maxHealth = GET_ENTITY_MAX_HEALTH(zombie);

				if (maxHealth > 0)
				{
					if (IS_PED_INJURED(zombie) || IS_PED_RAGDOLL(zombie))
					{
						Vector3 zombiePos = GET_ENTITY_COORDS(zombie, false);

						ADD_EXPLOSION(zombiePos.x, zombiePos.y, zombiePos.z, 4, 9999.f, true, false, 1.f, false);

						SET_ENTITY_HEALTH(zombie, 0, false);
						SET_ENTITY_MAX_HEALTH(zombie, 0);
					}

					it++;

					continue;
				}
			}

			SET_PED_AS_NO_LONGER_NEEDED(&zombie);
		}

		it = m_zombies.erase(it);
	}
}

static void OnTick()
{
	OnTickDrunk();
	OnTickSensitive();
	OnTickZombies();
}

static void OnStop()
{
	OnStopDrunk();
	OnStopSensitive();
	OnStopZombies();
}

static void OnStart()
{
	OnStartZombies();
}

static RegisterEffect registerEffect(EFFECT_REALLY_BAD_NIGHT, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Really bad night",
		.Id = "misc_really_bad_night",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_NO_RAGDOLL }
	}
);