#include <stdafx.h>

static void OnStopBeyBlades()
{
	for (Vehicle veh : GetAllVehs())
	{
		SET_ENTITY_INVINCIBLE(veh, false);
		SET_VEHICLE_REDUCE_GRIP(veh, false);
	}
}

static void OnTickBeyBlades()
{
	int count = 5;

	float force = 100;
	float velocityMultiplier = 3;
	for (Vehicle veh : GetAllVehs())
	{
		bool doBeyblade = IS_VEHICLE_SEAT_FREE(veh, -1, false) ? true : !IS_PED_A_PLAYER(GET_PED_IN_VEHICLE_SEAT(veh, -1, false));

		if (doBeyblade)
		{
			APPLY_FORCE_TO_ENTITY(veh, 3, force, 0, 0, 0, 4, 0, 0, true, true, true, true, true);
			APPLY_FORCE_TO_ENTITY(veh, 3, -force, 0, 0, 0, -4, 0, 0, true, true, true, true, true);
			SET_ENTITY_INVINCIBLE(veh, true);
			SET_VEHICLE_REDUCE_GRIP(veh, true);
			if (GET_ENTITY_SPEED(veh) < 10)
			{
				Vector3 velocity = GET_ENTITY_SPEED_VECTOR(veh, true);
				velocity.x *= velocityMultiplier;
				velocity.y *= velocityMultiplier;
				SET_ENTITY_VELOCITY(veh, velocity.x, velocity.y, velocity.z);
			}
		}

		if (--count == 0)
		{
			count = 5;

			WAIT(0);
		}
	}
}

static void OnTickX10()
{
	for (auto veh : GetAllVehs())
	{
		MODIFY_VEHICLE_TOP_SPEED(veh, 100.f);
		SET_VEHICLE_CHEAT_POWER_INCREASE(veh, 100.f);
	}
}

static void OnStopX10()
{
	for (auto veh : GetAllVehs())
	{
		MODIFY_VEHICLE_TOP_SPEED(veh, 1.f);
		SET_VEHICLE_CHEAT_POWER_INCREASE(veh, 1.f);
	}
}

static void OnStart()
{
	Ped player = PLAYER_PED_ID();

	Vector3 playerPos = GET_ENTITY_COORDS(player, false);

	Vehicle dumpTruck = CreatePoolVehicle(GET_HASH_KEY("DUMP"), playerPos.x, playerPos.y, playerPos.z, GET_ENTITY_HEADING(PLAYER_PED_ID()));

	SET_PED_INTO_VEHICLE(player, dumpTruck, -1);
}

static void OnStop() {
	OnStopBeyBlades();
	OnStopX10();
}

static void OnTick() {
	OnTickBeyBlades();
	OnTickX10();
}

static RegisterEffect registerEffect(EFFECT_BUMPER_CARS, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Bumper Cars",
		.Id = "misc_bumper_cars",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_2XENGINE_VEHS, EFFECT_05XENGINE_VEHS }
	}
);