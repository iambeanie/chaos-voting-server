#include <stdafx.h>

static Camera fovCamera = 0;

static void OnStartQuakeFOV()
{
    fovCamera = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
    CAM::RENDER_SCRIPT_CAMS(true, true, 700, 1, 1, 1);
}

static void OnTickQuakeFOV()
{
    CAM::SET_CAM_ACTIVE(fovCamera, true);
    auto coord = CAM::GET_GAMEPLAY_CAM_COORD();
    auto rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
    CAM::SET_CAM_PARAMS(fovCamera, coord.x, coord.y, coord.z, rot.x, rot.y, rot.z, 120, 0, 1, 1, 2);
}

static void OnStopQuakeFOV()
{
    CAM::SET_CAM_ACTIVE(fovCamera, false);
    CAM::RENDER_SCRIPT_CAMS(false, true, 700, 1, 1, 1);
    CAM::DESTROY_CAM(fovCamera, true);
    fovCamera = 0;
}

static void StartTransitionTimecycle(std::string modifier)
{
	if (GET_TIMECYCLE_TRANSITION_MODIFIER_INDEX() == -1 && GET_TIMECYCLE_MODIFIER_INDEX() == -1)
	{
		SET_TRANSITION_TIMECYCLE_MODIFIER(modifier.c_str(), 5.f);
	}
}

static void OnStopBright()
{
	CLEAR_TIMECYCLE_MODIFIER();
	for (auto car : GetAllVehs())
	{
		SET_VEHICLE_LIGHTS(car, 0);
		SET_VEHICLE_LIGHT_MULTIPLIER(car, 1);
	}
}

static void OnTickBright()
{
	StartTransitionTimecycle("mp_x17dlc_int_02");

	Vector3 playerPos = GET_ENTITY_COORDS(PLAYER_PED_ID(), false);
	_DRAW_LIGHT_WITH_RANGE_AND_SHADOW(playerPos.x, playerPos.y, playerPos.z + 1, 255, 255, 255, 5000, 5, 0);
	_DRAW_LIGHT_WITH_RANGE_AND_SHADOW(playerPos.x, playerPos.y, playerPos.z + 100, 255, 255, 255, 5000, 10, 0);
	SET_WEATHER_TYPE_NOW("EXTRASUNNY");
	SET_TIMECYCLE_MODIFIER("mp_x17dlc_int_02");
	PUSH_TIMECYCLE_MODIFIER();

	for (auto car : GetAllVehs())
	{
		SET_VEHICLE_LIGHTS(car, 2); // Vehicles lights always on even if no peds inside
		SET_VEHICLE_LIGHT_MULTIPLIER(car, 10000); // Make lights eye hurting
	}
}

static void OnTickBubbleVision()
{
	StartTransitionTimecycle("ufo_deathray");

	SET_AUDIO_SPECIAL_EFFECT_MODE(1);
}

static void OnTickBloom()
{
	StartTransitionTimecycle("Bloom");
}

static void OnTick()
{
	OnTickQuakeFOV();
	OnTickBright();
	OnTickBubbleVision();
	OnTickBloom();
}

static void OnStop()
{
	OnStopQuakeFOV();
	OnStopBright();
}

static void OnStart()
{
	OnStartQuakeFOV();
}

static RegisterEffect registerEffect(EFFECT_TOOK_TOO_MUCH, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Took too much",
		.Id = "misc_took_too_much",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_FLIP_CAMERA, EFFECT_PLAYER_GTA_2, EFFECT_SCREEN_LS_NOIRE, EFFECT_SCREEN_LSD, EFFECT_SCREEN_RENDERDIST, EFFECT_SCREEN_FOG, EFFECT_SCREEN_BRIGHT, EFFECT_SCREEN_MEXICO, EFFECT_SCREEN_FULLBRIGHT, EFFECT_SCREEN_BUBBLEVISION }
	}
);