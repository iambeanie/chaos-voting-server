#include <stdafx.h>

static void OnStopLowGravity()
{
	SET_GRAVITY_LEVEL(0);
}

static void OnTickLowGravity()
{
	SET_GRAVITY_LEVEL(1);
}

static void OnTickJumpyVehicles()
{
	static auto lastTick = GET_GAME_TIMER();
	auto curTick = GET_GAME_TIMER();

	if (lastTick < curTick - 100)
	{
		lastTick = curTick;

		for (auto veh : GetAllVehs())
		{
			if (!IS_ENTITY_IN_AIR(veh))
			{
				APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(veh, 0, .0f, .0f, 200.f, true, false, true, true);
			}
		}
	}
}

static void OnTick()
{
	OnTickLowGravity();
	OnTickJumpyVehicles();
}

static void OnStop()
{
	OnStopLowGravity();
}

static RegisterEffect registerEffect(EFFECT_MOON_QUAKE, nullptr, OnStop, OnTick, EffectInfo
	{
		.Name = "Moon Quake",
		.Id = "misc_moon_quake",
		.IsTimed = true,
		.IsShortDuration = true
	}
);