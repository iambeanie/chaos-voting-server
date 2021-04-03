#include <stdafx.h>

static void OnTickFpsLimit()
{
	static const int lagTimeDelay = 1000 / 25;
	int lastUpdateTick = GetTickCount64();
	while (lastUpdateTick > GetTickCount64() - lagTimeDelay)
	{
		// Create Lag
	}
}

static void OnStopLowPoly()
{
	_0xA76359FC80B2438E(1.f);
}

static void OnTickLowPoly()
{
	_0xA76359FC80B2438E(.04f);

	// By Juhana
	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped))
		{
			FORCE_PED_MOTION_STATE(ped, 0xbac0f10b, 0, 0, 0);	// 0xbac0f10b is "nothing" according to Script Hook V
		}
	}
}

static const float height = 35.f;
static const float speedFactor = 1.f + (0.5f / (180.f / 2.236936f));
static float baseFov;
static Cam camera;

static void UpdateLocation() {
	Ped player = PLAYER_PED_ID();
	Vector3 pos = GET_ENTITY_COORDS(player, IS_PED_DEAD_OR_DYING(player, 1));

	SET_CAM_COORD(camera, pos.x, pos.y, pos.z + height);
}

static void OnStartGTA2() {
	camera = CREATE_CAM("DEFAULT_SCRIPTED_FLY_CAMERA", 1);

	SET_CAM_ROT(camera, -90.f, 0.f, 0.0, 2);
	UpdateLocation();
	RENDER_SCRIPT_CAMS(true, true, 500, 0, 1, 0);
	baseFov = GET_CAM_FOV(camera);

	SET_CAM_AFFECTS_AIMING(camera, false);
}

static void OnTickGTA2() {
	Ped player = PLAYER_PED_ID();
	float speed = GET_ENTITY_SPEED(player);
	float offset = speed * speedFactor;

	SET_CAM_FOV(camera, baseFov + offset);
	UpdateLocation();
}

static void OnStopGTA2() {
	RENDER_SCRIPT_CAMS(false, true, 500, 0, 1, 0);
	SET_CAM_AFFECTS_AIMING(camera, true);
	DESTROY_CAM(camera, true);
}

static void OnTick()
{
	OnTickLowPoly();
	OnTickFpsLimit();
	OnTickGTA2();
}

static void OnStop()
{
	OnStopLowPoly();
	OnStopGTA2();
}

static void OnStart()
{
	OnStartGTA2();
}

static RegisterEffect registerEffect(EFFECT_FILTHY_CONSOLE_PEASANT, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Filthy Console Peasant",
		.Id = "misc_filthy_console_peasant",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_PLAYER_QUAKE_FOV, EFFECT_FLIP_CAMERA }
	}
);