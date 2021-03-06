#include <stdafx.h>

DWORD64 initial_time = 0;

static void OnTickWhales()
{
	static constexpr int MAX_WHALES = 40;
	static const Hash WHALE_MODEL = 1193010354;

	static Ped whales[MAX_WHALES] = {};
	static int whaleDespawnTime[MAX_WHALES];
	static int whaleAmount = 0;

	Vector3 playerPos = GET_ENTITY_COORDS(PLAYER_PED_ID(), false);

	static DWORD64 lastTick = 0;
	DWORD64 curTick = GET_GAME_TIMER();

	if (curTick - initial_time > 8000 && whaleAmount <= MAX_WHALES && curTick > lastTick + 50)
	{
		lastTick = curTick;

		Vector3 spawnPos = Vector3::Init(
			playerPos.x + g_random.GetRandomInt(-10, 10),
			playerPos.y + g_random.GetRandomInt(-10, 10),
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

static void OnStart()
{
	initial_time = GET_GAME_TIMER();
}

static void OnTick() {
	OnTickWhales();
}

static RegisterEffect registerEffect(EFFECT_BODIES_HIT_THE_FLOOR, OnStart, nullptr, OnTick, EffectInfo
	{
		.Name = "Let the bodies hit the floor",
		.Id = "misc_bodies_hit_the_floor",
		.IsTimed = true,
		.IsShortDuration = true
	}
);