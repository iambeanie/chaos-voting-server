#include <stdafx.h>

static void OnStart()
{
	Ped playerPed = PLAYER_PED_ID();

	for (auto ped : GetAllPeds())
	{
		if (IS_PED_IN_ANY_VEHICLE(ped, true))
		{
			Vehicle pedVeh = GET_VEHICLE_PED_IS_IN(ped, true);
			TASK_LEAVE_VEHICLE(ped, pedVeh, 256);
			BRING_VEHICLE_TO_HALT(pedVeh, 0.1f, 10, 0);
		}
		if (ped != playerPed)
		{
			TASK_TURN_PED_TO_FACE_ENTITY(ped, playerPed, -1);
			TASK_LOOK_AT_ENTITY(ped, playerPed, -1, 2048, 3);
		}
	}
}

static void OnTick()
{
	static DWORD64 lastTick = 0;
	DWORD64 curTick = GET_GAME_TIMER();

	if (lastTick < curTick - 1000)
	{
		lastTick = curTick;

		for (Ped ped : GetAllPeds())
		{
			if (!IS_PED_A_PLAYER(ped) && IS_PED_HUMAN(ped))
			{
				_PLAY_AMBIENT_SPEECH1(ped, "KIFFLOM_GREET", "SPEECH_PARAMS_FORCE_SHOUTED", 1);
			}
		}
	}
}

static RegisterEffect registerEffect(EFFECT_DO_NOT_BELONG, OnStart, nullptr, OnTick, EffectInfo
	{
		.Name = "You do not belong here",
		.Id = "misc_do_not_belong",
		.IsTimed = true,
		.IsShortDuration = true
	}
);