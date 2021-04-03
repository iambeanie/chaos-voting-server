#include <stdafx.h>

static void OnStopInsaneGravity()
{
	SET_GRAVITY_LEVEL(0);
}

static void OnTickInsane()
{
	Memory::SetGravityLevel(200.f);

	for (auto ped : GetAllPeds())
	{
		if (!IS_PED_IN_ANY_VEHICLE(ped, false))
		{
			SET_PED_TO_RAGDOLL(ped, 1000, 1000, 0, true, true, false);

			APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(ped, 0, 0, 0, -75.f, false, false, true, false);
		}
	}

	for (auto object : GetAllProps())
	{
		APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(object, 0, 0, 0, -200.f, false, false, true, false);
	}
}

static void OnStop10XVehicleSpeed()
{
	for (auto veh : GetAllVehs())
	{
		MODIFY_VEHICLE_TOP_SPEED(veh, 1.f);
		SET_VEHICLE_CHEAT_POWER_INCREASE(veh, 1.f);
	}
}

static void OnTickX10()
{
	for (auto veh : GetAllVehs())
	{
		MODIFY_VEHICLE_TOP_SPEED(veh, 10.f);
		SET_VEHICLE_CHEAT_POWER_INCREASE(veh, 10.f);
	}
}

static void OnStopInvincibleVehicles()
{
	for (auto veh : GetAllVehs())
	{
		SET_ENTITY_INVINCIBLE(veh, false);
	}
}

static void OnTickInvincibleVehicles()
{
	for (auto veh : GetAllVehs())
	{
		SET_ENTITY_INVINCIBLE(veh, true);
	}
}

static void OnStartMaxUpgrades()
{
	for (Vehicle veh : GetAllVehs())
	{
		SET_VEHICLE_MOD_KIT(veh, 0);
		for (int i = 0; i < 50; i++)
		{
			int max = GET_NUM_VEHICLE_MODS(veh, i);
			if (max > 0)
			{
				SET_VEHICLE_MOD(veh, i, max - 1, true);
			}

			TOGGLE_VEHICLE_MOD(veh, i, true);
		}

		SET_VEHICLE_TYRES_CAN_BURST(veh, false);
		SET_VEHICLE_WINDOW_TINT(veh, 1);

		SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, g_random.GetRandomInt(0, 255), g_random.GetRandomInt(0, 255), g_random.GetRandomInt(0, 255));
		SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, g_random.GetRandomInt(0, 255), g_random.GetRandomInt(0, 255), g_random.GetRandomInt(0, 255));

		_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, g_random.GetRandomInt(0, 255), g_random.GetRandomInt(0, 255), g_random.GetRandomInt(0, 255));
		for (int i = 0; i < 4; i++)
		{
			_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, i, true);
		}

		_SET_VEHICLE_XENON_LIGHTS_COLOR(veh, g_random.GetRandomInt(0, 12));

		WAIT(0);
	}
}

static void OnTick()
{
	OnTickInsane();
	OnTickX10();
	OnTickInvincibleVehicles();
}

static void OnStop()
{
	OnStopInsaneGravity();
	OnStop10XVehicleSpeed();
	OnStopInvincibleVehicles();
}

static void OnStart()
{
	OnStartMaxUpgrades();
}

static RegisterEffect registerEffect(EFFECT_GOTTA_GO_FAST, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Took too much",
		.Id = "misc_took_too_much",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_LOW_GRAV, EFFECT_VERY_LOW_GRAV, EFFECT_INVERT_GRAV, EFFECT_2XENGINE_VEHS, EFFECT_05XENGINE_VEHS }
	}
);