#include <stdafx.h>

static void OnStartKickFlip()
{
	Ped player = PLAYER_PED_ID();
	Entity entityToFlip;
	if (IS_PED_IN_ANY_VEHICLE(player, false))
	{
		entityToFlip = GET_VEHICLE_PED_IS_IN(player, false);
	}
	else
	{
		entityToFlip = player;
		SET_PED_TO_RAGDOLL(player, 200, 0, 0, true, true, false);
	}
	APPLY_FORCE_TO_ENTITY(entityToFlip, 1, 0, 0, 10, 2, 0, 0, 0, true, true, true, false, true);
}

static Camera flippedCamera = 0;

static void UpdateCamera()
{
	auto coord = CAM::GET_GAMEPLAY_CAM_COORD();
	auto rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	auto fov = CAM::GET_GAMEPLAY_CAM_FOV();
	CAM::SET_CAM_PARAMS(flippedCamera, coord.x, coord.y, coord.z, rot.x, 180.0f, rot.z, fov, 0, 1, 1, 2);
}

static void OnStartFlipCamera()
{
	flippedCamera = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
	CAM::RENDER_SCRIPT_CAMS(true, true, 700, 1, 1, 1);
}

static void OnTickFlipCamera()
{
	CAM::SET_CAM_ACTIVE(flippedCamera, true);
	UpdateCamera();
}

static void OnStopFlipCamera()
{
	CAM::SET_CAM_ACTIVE(flippedCamera, false);
	CAM::RENDER_SCRIPT_CAMS(false, true, 700, 1, 1, 1);
	CAM::DESTROY_CAM(flippedCamera, true);
	flippedCamera = 0;
}

static void OnTickGTAOTraffic()
{
	static std::vector<Ped> goneThrough;

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped) && IS_PED_IN_ANY_VEHICLE(ped, false) && GET_PED_IN_VEHICLE_SEAT(GET_VEHICLE_PED_IS_IN(ped, false), -1, 0) == ped
			&& std::find(goneThrough.begin(), goneThrough.end(), ped) == goneThrough.end())
		{
			Vehicle veh = GET_VEHICLE_PED_IS_IN(ped, false);

			SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);

			TASK_VEHICLE_MISSION_PED_TARGET(ped, veh, playerPed, 13, 9999.f, 4176732, .0f, .0f, false);

			goneThrough.push_back(ped);
		}
	}

	std::vector<Ped>::iterator it;
	for (it = goneThrough.begin(); it != goneThrough.end(); )
	{
		if (!DOES_ENTITY_EXIST(*it))
		{
			it = goneThrough.erase(it);
		}
		else
		{
			it++;
		}
	}
}

static void OnTick()
{
	OnTickFlipCamera();
	OnTickGTAOTraffic();
}

static void OnStop()
{
	OnStopFlipCamera();
}

static void OnStart()
{
	OnStartKickFlip();
	OnStartFlipCamera();
}

static RegisterEffect registerEffect(EFFECT_TOPSY_TURVY, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Topsy Turvy",
		.Id = "misc_topsy_turvy",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_PLAYER_QUAKE_FOV, EFFECT_PLAYER_GTA_2 }
	}
);