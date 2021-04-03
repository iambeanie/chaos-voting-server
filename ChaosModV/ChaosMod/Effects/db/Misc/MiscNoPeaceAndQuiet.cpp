#include <stdafx.h>

static void OnTickPhones()
{
	for (auto ped : GetAllPeds())
	{
		if (!IS_PED_RINGTONE_PLAYING(ped))
		{
			PLAY_PED_RINGTONE("Remote_Ring", ped, 1);
		}
	}
}

static void OnStopPhones()
{
	for (auto ped : GetAllPeds())
	{
		STOP_PED_RINGTONE(ped);
	}
}

static void OnTickHorns()
{
	for (auto veh : GetAllVehs())
	{
		_SOUND_VEHICLE_HORN_THIS_FRAME(veh);
	}
}

static void OnTickAlarms()
{
	static auto lastTick = GET_GAME_TIMER();
	auto curTick = GET_GAME_TIMER();

	if (lastTick < curTick - 2000)
	{
		lastTick = curTick;

		for (auto veh : GetAllVehs())
		{
			SET_VEHICLE_ALARM(veh, true);
			START_VEHICLE_ALARM(veh);
		}
	}
}

static void OnStopAlarms()
{
	for (auto veh : GetAllVehs())
	{
		SET_VEHICLE_ALARM(veh, false);
	}
}

static void OnTickSpamDoors()
{
	static DWORD64 lastTick = GET_GAME_TIMER();
	DWORD64 currentTick = GET_GAME_TIMER();

	if (lastTick < currentTick - 500) //every second, half of second
	{
		for (auto veh : GetAllVehs())
		{
			for (int i = 0; i < 7; i++)
			{
				SET_VEHICLE_DOOR_OPEN(veh, i, false, false);
				_SET_VEHICLE_DOOR_CAN_BREAK(veh, i, false);
			}
		}
	}

	if (lastTick < currentTick - 1000) //every second, end of
	{
		lastTick = currentTick;
		for (auto veh : GetAllVehs())
		{
			SET_VEHICLE_DOORS_SHUT(veh, false); // Closes ALL doors of vehicle
		}
	}
}

static void OnStopSpamDoors()
{
	for (auto veh : GetAllVehs())
	{
		for (int i = 0; i < 7; i++)
		{
			_SET_VEHICLE_DOOR_CAN_BREAK(veh, i, true);
		}
	}
}

static void OnStartScooterBrothers()
{
	static const Hash faggioHash = GET_HASH_KEY("FAGGIO");

	Ped playerPed = PLAYER_PED_ID();

	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped) && !IS_PED_DEAD_OR_DYING(ped, false))
		{
			if (IS_PED_IN_ANY_VEHICLE(ped, false))
			{
				Vehicle veh = GET_VEHICLE_PED_IS_IN(ped, false);

				if (GET_ENTITY_MODEL(veh) == faggioHash)
				{
					continue;
				}
			}

			Vector3 pedPos = GET_ENTITY_COORDS(ped, false);
			float pedHeading = GET_ENTITY_HEADING(ped);

			SET_ENTITY_COORDS(ped, pedPos.x, pedPos.y, pedPos.z + 3.f, false, false, false, false);
			SET_PED_COMBAT_ATTRIBUTES(ped, 3, false); // Don't allow them to leave vehicle by themselves
			SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);

			Vehicle veh = CreateTempVehicle(faggioHash, pedPos.x, pedPos.y, pedPos.z, pedHeading);
			SET_PED_INTO_VEHICLE(ped, veh, -1);

			TASK_VEHICLE_MISSION_PED_TARGET(ped, veh, playerPed, 13, 9999.f, 4176732, .0f, .0f, false);
		}
	}
}

static void OnTick()
{
	OnTickPhones();
	OnTickHorns();
	OnTickAlarms();
	OnTickSpamDoors();
}

static void OnStop()
{
	OnStopPhones();
	OnStopAlarms();
	OnStopSpamDoors();
}

static void OnStart()
{
	OnStartScooterBrothers();
}

static RegisterEffect registerEffect(EFFECT_NO_PEACE_AND_QUIET, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "No piece and quiet",
		.Id = "misc_no_peace_and_quiet",
		.IsTimed = true
	}
);