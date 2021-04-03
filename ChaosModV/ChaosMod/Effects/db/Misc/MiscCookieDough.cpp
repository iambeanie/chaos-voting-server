#include <stdafx.h>

static void OnStop()
{
	SET_PLAYER_INVINCIBLE(PLAYER_ID(), false);

	for (auto ped : GetAllPeds())
	{
		SET_PED_RAGDOLL_ON_COLLISION(ped, false);
	}
}

static void OnTick()
{
	SET_PLAYER_INVINCIBLE(PLAYER_ID(), true);

	for (auto ped : GetAllPeds())
	{
		SET_PED_RAGDOLL_ON_COLLISION(ped, true);
	}

	Ped playerPed = PLAYER_PED_ID();

	int curTime = GET_GAME_TIMER();

	for (Hash weapon : Memory::GetAllWeapons())
	{
		int timeSinceDmg = _GET_TIME_OF_LAST_PED_WEAPON_DAMAGE(playerPed, weapon);
		if (timeSinceDmg && curTime - timeSinceDmg < 200)
		{
			if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
			{
				CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			}

			SET_PED_TO_RAGDOLL(playerPed, 500, 1000, 0, true, true, false);

			CREATE_NM_MESSAGE(true, 0);
			GIVE_PED_NM_MESSAGE(playerPed);

			break;
		}
	}
}

static RegisterEffect registerEffect(EFFECT_COOKIE_DOUGH, nullptr, OnStop, OnTick, EffectInfo
	{
		.Name = "Cookie Dough",
		.Id = "misc_cookie_dough",
		.IsTimed = true,
		.IsShortDuration = true
	}
);