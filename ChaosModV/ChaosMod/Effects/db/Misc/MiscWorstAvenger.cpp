#include <stdafx.h>

static void OnStop()
{
	for (auto ped : GetAllPeds())
	{
		SET_PED_RAGDOLL_ON_COLLISION(ped, false);
	}
}

static void OnTick()
{
	for (auto ped : GetAllPeds())
	{
		SET_PED_RAGDOLL_ON_COLLISION(ped, true);
	}
}

static void OnStart()
{
	Ped playerPed = PLAYER_PED_ID();

	CLEAR_PED_TASKS_IMMEDIATELY(playerPed);

	SET_PED_TO_RAGDOLL(playerPed, 10000, 10000, 0, true, true, false);
}

static RegisterEffect registerEffect(EFFECT_WORST_AVENGER, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Worst Avenger",
		.Id = "misc_worst_avenger",
		.IsTimed = true,
		.IsShortDuration = true
	}
);