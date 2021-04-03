#include <stdafx.h>

static void OnStartExplodeNearbyVehicles()
{
	Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(PLAYER_PED_ID(), false);

	int count = 3;

	for (Vehicle veh : GetAllVehs())
	{
		if (veh != playerVeh)
		{
			EXPLODE_VEHICLE(veh, true, false);

			if (--count == 0)
			{
				count = 3;

				WAIT(0);
			}
		}
	}
}

static void OnTickExplosivePeds()
{
	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped))
		{
			int maxHealth = GET_ENTITY_MAX_HEALTH(ped);

			if (maxHealth > 0 && (IS_PED_INJURED(ped) || IS_PED_RAGDOLL(ped)))
			{
				Vector3 pedPos = GET_ENTITY_COORDS(ped, false);

				ADD_EXPLOSION(pedPos.x, pedPos.y, pedPos.z, 4, 9999.f, true, false, 1.f, false);

				SET_ENTITY_HEALTH(ped, 0, false);
				SET_ENTITY_MAX_HEALTH(ped, 0);
			}
		}
	}
}


static void OnTick()
{
	OnTickExplosivePeds();
}

static void OnStart()
{
	OnStartExplodeNearbyVehicles();
}

static RegisterEffect registerEffect(EFFECT_PLAGUE, OnStart, nullptr, OnTick, EffectInfo
	{
		.Name = "Plague",
		.Id = "misc_plague",
		.IsTimed = true
	}
);