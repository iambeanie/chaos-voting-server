#include <stdafx.h>

static void OnTickVehicleRain()
{
	Vector3 playerPos = GET_ENTITY_COORDS(PLAYER_PED_ID(), false);

	static DWORD64 lastTick = 0;
	DWORD64 curTick = GET_GAME_TIMER();

	if (curTick > lastTick + 500)
	{
		lastTick = curTick;

		Vector3 spawnPos;
		spawnPos.x = playerPos.x + g_random.GetRandomInt(-100, 100);
		spawnPos.y = playerPos.y + g_random.GetRandomInt(-100, 100);
		spawnPos.z = playerPos.z + g_random.GetRandomInt(25, 50);


		static std::vector<Hash> vehModels = Memory::GetAllVehModels();

		if (!vehModels.empty())
		{
			Vehicle veh = CreateTempVehicle(vehModels[g_random.GetRandomInt(0, vehModels.size() - 1)], spawnPos.x, spawnPos.y, spawnPos.z, GET_ENTITY_HEADING(PLAYER_PED_ID()));

			// Also apply random upgrades
			SET_VEHICLE_MOD_KIT(veh, 0);
			for (int i = 0; i < 50; i++)
			{
				int max = GET_NUM_VEHICLE_MODS(veh, i);
				SET_VEHICLE_MOD(veh, i, max > 0 ? g_random.GetRandomInt(0, max - 1) : 0, g_random.GetRandomInt(0, 1));

				TOGGLE_VEHICLE_MOD(veh, i, g_random.GetRandomInt(0, 1));
			}

			SET_VEHICLE_TYRES_CAN_BURST(veh, g_random.GetRandomInt(0, 1));
			SET_VEHICLE_WINDOW_TINT(veh, g_random.GetRandomInt(0, 6));
		}
	}
}


static void OnTickWhaleRain()
{
	static constexpr int MAX_WHALES = 20;
	static const Hash WHALE_MODEL = 1193010354;

	static Ped whales[MAX_WHALES] = {};
	static int whaleDespawnTime[MAX_WHALES];
	static int whaleAmount = 0;

	Vector3 playerPos = GET_ENTITY_COORDS(PLAYER_PED_ID(), false);

	static DWORD64 lastTick = 0;
	DWORD64 curTick = GET_GAME_TIMER();

	if (whaleAmount <= MAX_WHALES && curTick > lastTick + 200)
	{
		lastTick = curTick;

		Vector3 spawnPos = Vector3::Init(
			playerPos.x + g_random.GetRandomInt(-100, 100),
			playerPos.y + g_random.GetRandomInt(-100, 100),
			playerPos.z + g_random.GetRandomInt(25, 50)
		);

		LoadModel(WHALE_MODEL);

		Ped whale = CREATE_PED(28, WHALE_MODEL, spawnPos.x, spawnPos.y, spawnPos.z, g_random.GetRandomInt(0, 359), true, false);

		whaleAmount++;
		for (int i = 0; i < MAX_WHALES; i++)
		{
			Ped& ped = whales[i];
			if (!ped)
			{
				ped = whale;
				whaleDespawnTime[i] = 5;
				break;
			}
		}

		SET_PED_TO_RAGDOLL(whale, 10000, 10000, 0, true, true, false);
		SET_ENTITY_HEALTH(whale, 0, 0);

		APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(whale, 0, 35.f, 0, -5000.f, true, false, true, true);

		SET_MODEL_AS_NO_LONGER_NEEDED(WHALE_MODEL);
	}

	static DWORD64 lastTick2 = 0;
	for (int i = 0; i < MAX_WHALES; i++)
	{
		Ped& whale = whales[i];
		if (whale)
		{
			if (DOES_ENTITY_EXIST(whale) && whaleDespawnTime[i] > 0)
			{
				Vector3 propPos = GET_ENTITY_COORDS(whale, false);
				if (GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, propPos.x, propPos.y, propPos.z, true) < 400.f)
				{
					if (HAS_ENTITY_COLLIDED_WITH_ANYTHING(whale))
					{
						if (lastTick2 < curTick - 1000)
						{
							whaleDespawnTime[i]--;
						}
					}
					continue;
				}
			}

			whaleAmount--;

			SET_OBJECT_AS_NO_LONGER_NEEDED(&whale);

			whale = 0;
		}
	}

	if (lastTick2 < curTick - 1000)
	{
		lastTick2 = curTick;
	}
}

static void OnTickMeteor()
{
	// Thanks to menyoo for the prop names
	static const char* propNames[] = { "prop_asteroid_01", "prop_test_boulder_01", "prop_test_boulder_02", "prop_test_boulder_03", "prop_test_boulder_04" };
	static constexpr int MAX_METEORS = 20;

	static Object meteors[MAX_METEORS] = {};
	static int meteorDespawnTime[MAX_METEORS];
	static int meteorsAmount = 0;

	Vector3 playerPos = GET_ENTITY_COORDS(PLAYER_PED_ID(), false);

	static DWORD64 lastTick = 0;
	DWORD64 curTick = GET_GAME_TIMER();

	if (meteorsAmount <= MAX_METEORS && curTick > lastTick + 200)
	{
		lastTick = curTick;

		Vector3 spawnPos = Vector3::Init(
			playerPos.x + g_random.GetRandomInt(-100, 100),
			playerPos.y + g_random.GetRandomInt(-100, 100),
			playerPos.z + g_random.GetRandomInt(25, 50)
		);
		Hash choosenPropHash = GET_HASH_KEY(propNames[g_random.GetRandomInt(0, 4)]);
		LoadModel(choosenPropHash);

		Object meteor = CREATE_OBJECT(choosenPropHash, spawnPos.x, spawnPos.y, spawnPos.z, true, false, true);
		meteorsAmount++;

		for (int i = 0; i < MAX_METEORS; i++)
		{
			Object& prop = meteors[i];
			if (!prop)
			{
				prop = meteor;
				meteorDespawnTime[i] = 5;
				break;
			}
		}

		SET_OBJECT_PHYSICS_PARAMS(meteor, 100000.f, 1.f, 1.f, 0.f, 0.f, .5f, 0.f, 0.f, 0.f, 0.f, 0.f);
		APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(meteor, 0, 50.f, 0, -10000.f, true, false, true, true);

		SET_MODEL_AS_NO_LONGER_NEEDED(choosenPropHash);
	}

	static DWORD64 lastTick2 = 0;
	for (int i = 0; i < MAX_METEORS; i++)
	{
		Object& prop = meteors[i];
		if (prop)
		{
			if (DOES_ENTITY_EXIST(prop) && meteorDespawnTime[i] > 0)
			{
				Vector3 propPos = GET_ENTITY_COORDS(prop, false);
				if (GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, propPos.x, propPos.y, propPos.z, true) < 400.f)
				{
					if (HAS_ENTITY_COLLIDED_WITH_ANYTHING(prop))
					{
						if (lastTick2 < curTick - 1000)
						{
							meteorDespawnTime[i]--;
						}
					}
					continue;
				}
			}

			meteorsAmount--;

			SET_OBJECT_AS_NO_LONGER_NEEDED(&prop);

			prop = 0;
		}
	}

	if (lastTick2 < curTick - 1000)
	{
		lastTick2 = curTick;
	}
}

static void OnTickOneHitKOVeh()
{
	static DWORD64 lastTick = GET_GAME_TIMER();
	DWORD64 curTick = GET_GAME_TIMER();

	if (lastTick < curTick - 1000)
	{
		lastTick = curTick;

		for (Vehicle veh : GetAllVehs())
		{
			Memory::SetVehicleOutOfControl(veh, true);
		}
	}
}

static void OnStopOneHitKOVeh() {
	for (Vehicle veh : GetAllVehs())
	{
		Memory::SetVehicleOutOfControl(veh, false);
	}
}

static void OnStop()
{
	OnStopOneHitKOVeh();
}


static void OnTick()
{
	OnTickVehicleRain();
	OnTickWhaleRain();
	OnTickMeteor();
	OnTickOneHitKOVeh();
}

static RegisterEffect registerEffect(EFFECT_RAIN_FIRE, nullptr, OnStop, OnTick, EffectInfo
	{
		.Name = "Rain Fire",
		.Id = "misc_rain_fire",
		.IsTimed = true,
		.IsShortDuration = true
	}
);