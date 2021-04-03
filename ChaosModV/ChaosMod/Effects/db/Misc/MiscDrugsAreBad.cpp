#include <stdafx.h>
#include "Memory/Hooks/AudioPitchHook.h"

static int lastCheck = 0;

static void OnStopSuperHot()
{
	SET_TIME_SCALE(1.f);
}

static void OnTickSuperHot()
{
	int current_time = GET_GAME_TIMER();
	if (current_time - lastCheck > 100)
	{
		lastCheck = current_time;
		Ped playerPed = PLAYER_PED_ID();
		float gameSpeed = 1;
		if (!IS_PED_GETTING_INTO_A_VEHICLE(playerPed) && !IS_PED_CLIMBING(playerPed) && !IS_PED_DIVING(playerPed) && !IS_PED_JUMPING_OUT_OF_VEHICLE(playerPed) && !IS_PED_RAGDOLL(playerPed) && !IS_PED_GETTING_UP(playerPed))
		{
			float speed = GET_ENTITY_SPEED(playerPed);
			gameSpeed = max(min(speed, 4.f) / 4, 0.2);
		}
		SET_TIME_SCALE(gameSpeed);
	}
}


static DWORD64 m_anchorTick;

static void OnStartTotalChaos()
{
	m_anchorTick = GET_GAME_TIMER();

	SET_WEATHER_TYPE_OVERTIME_PERSIST("THUNDER", 2.f);
}

static void OnStopTotalChaos()
{
	CLEAR_WEATHER_TYPE_PERSIST();

	SET_WEATHER_TYPE_NOW("EXTRASUNNY");
}

static void OnTickTotalChaos()
{
	Ped playerPed = PLAYER_PED_ID();
	Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);

	for (Vehicle veh : GetAllVehs())
	{
		if (veh != playerVeh)
		{
			APPLY_FORCE_TO_ENTITY(veh, 3, 10.f, .1f, .1f, 0, 0, 0, 0, true, true, true, false, true);
		}
	}

	for (Object prop : GetAllProps())
	{
		APPLY_FORCE_TO_ENTITY(prop, 3, 10.f, 5.f, .1f, 0, 0, 0, 0, true, true, true, false, true);
	}

	DWORD64 curTick = GET_GAME_TIMER();

	static DWORD64 lastTick = GET_GAME_TIMER();
	if (lastTick < curTick - 100)
	{
		lastTick = curTick;

		SHAKE_GAMEPLAY_CAM("SMALL_EXPLOSION_SHAKE", .1f);
	}

	// Make sure weather is always set to thunder after the transition
	if (m_anchorTick < curTick - 2000)
	{
		SET_WEATHER_TYPE_NOW("THUNDER");
	}

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

				timeUntilSteer += g_random.GetRandomInt(50, 250);
			}
			else
			{
				// Take control from player

				steering = GET_RANDOM_FLOAT_IN_RANGE(-.7f, .7f);

				timeUntilSteer += g_random.GetRandomInt(50, 300);
			}

			enableDrunkSteering = !enableDrunkSteering;
		}
	}
}



static void OnTick()
{
	OnTickSuperHot();
	OnTickTotalChaos();
}

static void OnStop()
{
	OnStopSuperHot();
	OnStopTotalChaos();
}

static void OnStart()
{
	OnStartTotalChaos();
}

static RegisterEffect registerEffect(EFFECT_DRUGS_ARE_BAD, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Drugs are bad",
		.Id = "misc_drugs_are_bad",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_GAMESPEED_X05, EFFECT_GAMESPEED_LAG, EFFECT_PLAYER_DEAD_EYE }
	}
);