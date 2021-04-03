#include <stdafx.h>

static void OnTick()
{
	for (auto veh : GetAllVehs())
	{
		_SOUND_VEHICLE_HORN_THIS_FRAME(veh);
		APPLY_FORCE_TO_ENTITY(veh, 0, .0f, 50.f, .0f, .0f, .0f, .0f, 0, true, true, true, false, true);
	}
}

static RegisterEffect registerEffect(EFFECT_HONK_HONK, nullptr, nullptr, OnTick, EffectInfo
	{
		.Name = "Honk make me fast",
		.Id = "misc_honk_honk",
		.IsTimed = true,
		.IsShortDuration = true
	}
);