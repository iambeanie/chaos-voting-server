#include <stdafx.h>

static void OnStop()
{
	SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PLAYER_ID(), 1.f);

	for (auto ped : GetAllPeds())
	{
		SET_PED_RAGDOLL_ON_COLLISION(ped, false);
	}
}

static void OnTick()
{
	Player player = PLAYER_ID();

	SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49f);

	SET_SUPER_JUMP_THIS_FRAME(player);

	for (auto ped : GetAllPeds())
	{
		SET_PED_RAGDOLL_ON_COLLISION(ped, true);
	}
}


static RegisterEffect registerEffect(EFFECT_GET_UP, nullptr, OnStop, OnTick, EffectInfo
	{
		.Name = "Please get up",
		.Id = "misc_get_up",
		.IsTimed = true,
		.IsShortDuration = true
	}
);