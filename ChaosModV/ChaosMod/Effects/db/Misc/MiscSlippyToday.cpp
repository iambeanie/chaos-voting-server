#include <stdafx.h>

static void OnTick()
{
	static DWORD64 lastTick = GET_GAME_TIMER();
	DWORD64 currentTick = GET_GAME_TIMER();

	if (lastTick < currentTick - 1750) // 1750MS = every 1.75 seconds.
	{
		lastTick = currentTick;

		for (Vehicle veh : GetAllVehs())
		{
			for (int i = 0; i < 48; i++) // using code from VehsPopTires.cpp
			{
				if (g_random.GetRandomInt(0, 1)) // random true / false to get ideally 50% of tires popped.
				{
					SET_VEHICLE_TYRES_CAN_BURST(veh, true);
					SET_VEHICLE_TYRE_BURST(veh, i, true, 1000.f);
				}
				else
				{
					SET_VEHICLE_TYRE_FIXED(veh, i);
				}
			}
		}
	}

	Memory::SetSnow(true);

	SET_WEATHER_TYPE_NOW("XMAS");

	for (auto veh : GetAllVehs())
	{
		static bool toggle = true;

		SET_VEHICLE_REDUCE_GRIP(veh, toggle);

		toggle = !toggle;
	}
}

static void OnStop()
{
	Memory::SetSnow(false);

	SET_WEATHER_TYPE_NOW("EXTRASUNNY");

	for (auto veh : GetAllVehs())
	{
		SET_VEHICLE_REDUCE_GRIP(veh, false);
	}
}

static RegisterEffect registerEffect(EFFECT_NO_TRACTION, nullptr, OnStop, OnTick, EffectInfo
	{
		.Name = "Bit slippy today",
		.Id = "misc_no_traction",
		.IsTimed = true,
		.IsShortDuration = true
	}
);