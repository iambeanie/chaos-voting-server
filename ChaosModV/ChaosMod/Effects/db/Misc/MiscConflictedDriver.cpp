#include <stdafx.h>

static void OnStop()
{
	for (auto veh : GetAllVehs())
	{
		MODIFY_VEHICLE_TOP_SPEED(veh, 1.f);
		SET_VEHICLE_CHEAT_POWER_INCREASE(veh, 1.f);
		_SET_VEHICLE_MAX_SPEED(veh, GET_VEHICLE_MODEL_ESTIMATED_MAX_SPEED(GET_ENTITY_MODEL(veh)));
	}
}

static void OnTick()
{
	for (auto veh : GetAllVehs())
	{
		MODIFY_VEHICLE_TOP_SPEED(veh, 10.f);
		SET_VEHICLE_CHEAT_POWER_INCREASE(veh, 10.f);
		_SET_VEHICLE_MAX_SPEED(veh, 13.41); // 13.41 Meters Per Second = 30 MPH~
	}
}

static RegisterEffect registerEffect(EFFECT_CONFLICTED_DRIVER, nullptr, OnStop, OnTick, EffectInfo
	{
		.Name = "Conflicted Driver",
		.Id = "misc_conflicted_driver",
		.IsTimed = true,
		.IsShortDuration = true
	}
);