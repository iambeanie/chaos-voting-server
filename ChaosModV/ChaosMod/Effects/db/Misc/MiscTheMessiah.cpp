#include <stdafx.h>

static Vehicle m_savedPlayerVeh;

static void OnTickKifflom()
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


static void OnTickPedsFollowPlayer()
{
	Ped playerPed = PLAYER_PED_ID();
	bool isPlayerInAnyVeh = IS_PED_IN_ANY_VEHICLE(playerPed, false);
	Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);

	if (isPlayerInAnyVeh)
	{
		m_savedPlayerVeh = playerVeh;
	}

	static DWORD64 lastTick = GET_GAME_TIMER();
	DWORD64 curTick = GET_GAME_TIMER();

	if (lastTick < curTick - 2000)
	{
		lastTick = curTick;

		int count = 3;

		for (Ped ped : GetAllPeds())
		{
			if (!IS_PED_A_PLAYER(ped))
			{
				bool isPedInAnyVeh = IS_PED_IN_ANY_VEHICLE(ped, true);
				bool isPedGettingInAnyVeh = IS_PED_GETTING_INTO_A_VEHICLE(ped);
				Vehicle pedVeh = GET_VEHICLE_PED_IS_IN(ped, false);
				Vehicle pedTargetVeh = GET_VEHICLE_PED_IS_ENTERING(ped);

				if (isPlayerInAnyVeh && (!isPedInAnyVeh || pedVeh != m_savedPlayerVeh) && (!isPedGettingInAnyVeh || pedTargetVeh != m_savedPlayerVeh))
				{
					TASK_ENTER_VEHICLE(ped, m_savedPlayerVeh, -1, -2, 2.f, 1, 0);
				}
				else if ((isPedInAnyVeh && pedVeh == m_savedPlayerVeh) || (isPedGettingInAnyVeh && pedTargetVeh == m_savedPlayerVeh))
				{
					if (GET_PED_IN_VEHICLE_SEAT(m_savedPlayerVeh, -1, 0) == ped)
					{
						TASK_VEHICLE_DRIVE_WANDER(ped, m_savedPlayerVeh, 9999.f, 10);
					}
				}
				else
				{
					TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped, playerPed, .0f, .0f, .0f, 9999.f, -1, .0f, true);
				}

				SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);

				if (--count == 0)
				{
					count = 3;

					WAIT(0);
				}
			}
		}
	}
}

static void OnTickNoRagdoll()
{
	for (auto ped : GetAllPeds())
	{
		SET_PED_CAN_RAGDOLL(ped, false);
	}
}

static void OnTick()
{
	OnTickKifflom();
	OnTickPedsFollowPlayer();
	OnTickNoRagdoll();
}

static void OnStartPedsFollowPlayer()
{
	m_savedPlayerVeh = 0;
}

static void OnStartPlayerVehDespawn()
{
	Ped playerPed = PLAYER_PED_ID();

	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);

		std::vector<Ped> vehPeds;

		Hash vehModel = GET_ENTITY_MODEL(playerVeh);
		int maxSeats = GET_VEHICLE_MODEL_NUMBER_OF_SEATS(vehModel);
		for (int i = -1; i < maxSeats - 1; i++)
		{
			if (IS_VEHICLE_SEAT_FREE(playerVeh, i, false))
			{
				continue;
			}

			Ped ped = GET_PED_IN_VEHICLE_SEAT(playerVeh, i, false);

			CLEAR_PED_TASKS_IMMEDIATELY(ped);

			SET_PED_TO_RAGDOLL(ped, 5000, 5000, 0, true, true, false);

			vehPeds.push_back(ped);
		}

		Vector3 vehVel = GET_ENTITY_VELOCITY(playerVeh);

		SET_ENTITY_AS_MISSION_ENTITY(playerVeh, true, true);
		DELETE_VEHICLE(&playerVeh);

		WAIT(0);

		for (Ped ped : vehPeds)
		{
			SET_ENTITY_VELOCITY(ped, vehVel.x, vehVel.y, vehVel.z);
		}
	}
}

static void OnStart() {
	OnStartPedsFollowPlayer();
	OnStartPlayerVehDespawn();
}

static void OnStopPedsFollowPlayer()
{
	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped))
		{
			SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, false);
		}
	}
}

static void OnStopNoRagdoll()
{
	for (auto ped : GetAllPeds())
	{
		SET_PED_CAN_RAGDOLL(ped, true);
	}
}

static void OnStop() {
	OnStopPedsFollowPlayer();
	OnStopNoRagdoll();
}


static RegisterEffect registerEffect(EFFECT_THE_MESSIAH, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "The Messiah",
		.Id = "misc_the_messiah",
		.IsTimed = true,
		.IsShortDuration = true
	}
);