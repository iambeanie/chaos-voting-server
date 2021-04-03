#include <stdafx.h>

static void OnStopGravity()
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

static void OnTickX1000()
{
	Ped player = PLAYER_PED_ID();

	if (IS_PED_IN_ANY_VEHICLE(player, false)) {
		Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(player, false);

		MODIFY_VEHICLE_TOP_SPEED(playerVeh, 1000.f);
		SET_VEHICLE_CHEAT_POWER_INCREASE(playerVeh, 1000.f);
	}
}

static void OnStart()
{
	Ped player = PLAYER_PED_ID();

	SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49f);
}

static void OnStop() {
	OnStopGravity();

	SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PLAYER_ID(), 1.f);
}

static void OnTick() {
	OnTickInsane();

	SIMULATE_PLAYER_INPUT_GAIT(PLAYER_ID(), 5.f, 100, 1.f, true, false);

	_SET_CONTROL_NORMAL(0, 32, 1);
	_SET_CONTROL_NORMAL(0, 71, 1);
	_SET_CONTROL_NORMAL(0, 77, 1);
	_SET_CONTROL_NORMAL(0, 87, 1);
	_SET_CONTROL_NORMAL(0, 129, 1);
	_SET_CONTROL_NORMAL(0, 136, 1);
	_SET_CONTROL_NORMAL(0, 150, 1);
	_SET_CONTROL_NORMAL(0, 232, 1);
	_SET_CONTROL_NORMAL(0, 280, 1);

	// Disable all brake and descend actions
	DISABLE_CONTROL_ACTION(0, 72, true);
	DISABLE_CONTROL_ACTION(0, 76, true);
	DISABLE_CONTROL_ACTION(0, 88, true);
	DISABLE_CONTROL_ACTION(0, 138, true);
	DISABLE_CONTROL_ACTION(0, 139, true);
	DISABLE_CONTROL_ACTION(0, 152, true);
	DISABLE_CONTROL_ACTION(0, 153, true);

	// Disable aiming actions, would cancel forward movement
	DISABLE_CONTROL_ACTION(0, 25, true);  //	INPUT_AIM
	DISABLE_CONTROL_ACTION(0, 44, true);  //  	INPUT_COVER
	DISABLE_CONTROL_ACTION(0, 50, true);  //  	INPUT_ACCURATE_AIM
	DISABLE_CONTROL_ACTION(0, 68, true);  //  	INPUT_VEH_AIM
}

static RegisterEffect registerEffect(EFFECT_SANIC, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Sanic",
		.Id = "misc_sanic",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_LOW_GRAV, EFFECT_VERY_LOW_GRAV, EFFECT_INVERT_GRAV }
	}
);