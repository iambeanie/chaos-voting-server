#include <stdafx.h>

static void OnStartBreakDoors()
{
	int count = 10;

	for (Vehicle veh : GetAllVehs())
	{
		for (int i = 0; i < 6; i++)
		{
			SET_VEHICLE_DOOR_BROKEN(veh, i, false);

			if (--count == 0)
			{
				count = 10;

				WAIT(0);
			}
		}
	}
}

static void OnTickVehiclesCrumble()
{
	std::vector<Vehicle> vehs = GetAllVehs();
	int vehicleAmount = static_cast<int>(vehs.size());

	if (vehicleAmount > 0)
	{
		// apply random damage to a random part of a random vehicle
		Vehicle veh = vehs[g_random.GetRandomInt(0, vehicleAmount - 1)];
		SET_VEHICLE_DAMAGE(veh, g_random.GetRandomFloat(-1.f, 1.f), g_random.GetRandomFloat(-1.f, 1.f), g_random.GetRandomFloat(-1.f, 1.f), g_random.GetRandomFloat(1000.f, 10000.f), g_random.GetRandomFloat(100.f, 1000.f), true);
	}
}

static void OnTickKillEngine()
{
	for (Vehicle veh : GetAllVehs())
	{
		SET_VEHICLE_ENGINE_HEALTH(veh, 0.f);
	}
}

static void OnTick()
{
	OnTickVehiclesCrumble();
	OnTickKillEngine();
}

static void OnStart()
{
	OnStartBreakDoors();
}

static RegisterEffect registerEffect(EFFECT_JUNKER, OnStart, nullptr, OnTick, EffectInfo
	{
		.Name = "Junker",
		.Id = "misc_junker",
		.IsTimed = true,
		.IsShortDuration = true
	}
);