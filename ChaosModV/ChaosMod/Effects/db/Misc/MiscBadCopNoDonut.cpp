#include <stdafx.h>

static void OnStartPlusTwo()
{
	Ped player = PLAYER_ID();

	SET_PLAYER_WANTED_LEVEL(player, PLAYER::GET_PLAYER_WANTED_LEVEL(player) + 2, false);
	SET_PLAYER_WANTED_LEVEL_NOW(player, false);
}

static void OnTickPedsCop()
{
	for (Ped ped : GetAllPeds())
	{
		Ped pedType = GET_PED_TYPE(ped);

		if (!IS_PED_A_PLAYER(ped) && pedType != 0 && pedType != 1 && pedType != 2)
		{
			SET_PED_AS_COP(ped, true);
		}
	}
}

static void OnStartPortalGun()
{
	for (Vehicle veh : GetAllVehs())
	{
		CLEAR_ENTITY_LAST_WEAPON_DAMAGE(veh);
	}
}


static void OnTickPortalGun()
{
	Vector3 impactCoords;

	for (Ped ped : GetAllPeds())
	{
		if (GET_PED_LAST_WEAPON_IMPACT_COORD(ped, &impactCoords))
		{
			Hash wepHash;
			GET_CURRENT_PED_WEAPON(ped, &wepHash, true);

			bool alreadyTeleported = false;
			for (Vehicle veh : GetAllVehs())
			{
				if (HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(veh, ped, true) && ARE_ANY_VEHICLE_SEATS_FREE(veh) && HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(veh, wepHash, 0))
				{
					int seat = -2;
					if (IS_PED_A_PLAYER(ped))
					{
						seat = -1;

						if (!IS_VEHICLE_SEAT_FREE(veh, -1, false))
						{
							Ped seatPed = GET_PED_IN_VEHICLE_SEAT(veh, -1, false);

							CLEAR_PED_TASKS_IMMEDIATELY(seatPed);

							WAIT(0);
						}
					}
					else if (IS_VEHICLE_SEAT_FREE(veh, -1, false))
					{
						seat = -1;
					}

					SET_PED_INTO_VEHICLE(ped, veh, seat);

					CLEAR_ENTITY_LAST_WEAPON_DAMAGE(veh);

					alreadyTeleported = true;
					break;
				}
			}

			if (!alreadyTeleported)
			{
				Entity toTeleport = ped;

				if (IS_PED_IN_ANY_VEHICLE(ped, false))
				{
					toTeleport = GET_VEHICLE_PED_IS_IN(ped, false);
				}

				float heading = GET_ENTITY_HEADING(toTeleport);
				Vector3 vel = GET_ENTITY_VELOCITY(toTeleport);
				float forward = GET_ENTITY_SPEED(toTeleport);

				SET_ENTITY_COORDS(toTeleport, impactCoords.x, impactCoords.y, impactCoords.z, true, true, true, false);
				SET_ENTITY_VELOCITY(toTeleport, vel.x, vel.y, vel.z);

				if (IS_ENTITY_A_VEHICLE(toTeleport))
				{
					SET_VEHICLE_FORWARD_SPEED(toTeleport, forward);
				}
			}
		}
	}
}

static void OnTick()
{
	OnTickPedsCop();
	OnTickPortalGun();
}

static void OnStop()
{

}

static void OnStart()
{
	OnStartPlusTwo();
	OnStartPortalGun();
}

static RegisterEffect registerEffect(EFFECT_BAD_COP_NO_DONUT, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Bad cop no donut",
		.Id = "misc_bad_cop_no_donut",
		.IsTimed = true,
		.IncompatibleWith = {EFFECT_NEVER_WANTED, EFFECT_PEDS_RIOT}
	}
);