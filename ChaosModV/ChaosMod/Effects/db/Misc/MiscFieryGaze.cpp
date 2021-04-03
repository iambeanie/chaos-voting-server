#include <stdafx.h>

static void OnStart()
{
	auto playerPed = PLAYER_PED_ID();

	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped))
		{
			TASK_REACT_AND_FLEE_PED(ped, playerPed);

			SET_PED_FLEE_ATTRIBUTES(ped, 2, true);
		}

		if (!IS_PED_A_PLAYER(ped))
		{
			START_ENTITY_FIRE(ped);
		}
	}
}

static RegisterEffect registerEffect(EFFECT_FIERY_GAZE, OnStart, nullptr, nullptr, EffectInfo
	{
		.Name = "Fiery Gaze",
		.Id = "misc_fiery_gaze",
		.IsTimed = true,
		.IsShortDuration = true
	}
);