#include <stdafx.h>

static const double SPEED = 0.003;
static const double BOX_HEIGHT_RATIO = 0.45;
static double boxWidth, boxHeight, offsetX, offsetY;
static bool goingDown = true, goingRight = true;

static int last_firework = 0;

static void StartTransitionTimecycle(std::string modifier)
{
	if (GET_TIMECYCLE_TRANSITION_MODIFIER_INDEX() == -1 && GET_TIMECYCLE_MODIFIER_INDEX() == -1)
	{
		SET_TRANSITION_TIMECYCLE_MODIFIER(modifier.c_str(), 5.f);
	}
}

static void OnStart()
{
	// Get Screenresolution to calculate the square ratio
	int screenresx, screenresy;
	_GET_ACTIVE_SCREEN_RESOLUTION(&screenresx, &screenresy);
	boxHeight = BOX_HEIGHT_RATIO;
	boxWidth = boxHeight * (double(screenresy) / double(screenresx));
	offsetX = 0;
	offsetY = 0;
	goingDown = true;
	goingRight = true;

	SET_CLOCK_TIME(0, 0, 0);
}

static void Drunk() {
	if (!IS_GAMEPLAY_CAM_SHAKING())
	{
		SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", 2.f);
	}

	Ped playerPed = PLAYER_PED_ID();

	SET_PED_IS_DRUNK(playerPed, true);

	REQUEST_CLIP_SET("MOVE_M@DRUNK@VERYDRUNK");
	SET_PED_MOVEMENT_CLIPSET(playerPed, "MOVE_M@DRUNK@VERYDRUNK", 1.f);

	SET_AUDIO_SPECIAL_EFFECT_MODE(2);

	// No idea what these do, but game scripts also call these so just blindly follow
	_0x487A82C650EB7799(1.f);
	_0x0225778816FDC28C(1.f);

	// Random right / left steering
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (GET_PED_IN_VEHICLE_SEAT(playerVeh, -1, 0) != playerPed)
		{
			return;
		}

		static DWORD64 timeUntilSteer = GET_GAME_TIMER();
		static bool enableDrunkSteering = false;
		static float steering;

		if (enableDrunkSteering)
		{
			SET_VEHICLE_STEER_BIAS(playerVeh, steering);
		}

		DWORD64 curTick = GET_GAME_TIMER();

		if (timeUntilSteer < curTick)
		{
			timeUntilSteer = GET_GAME_TIMER();

			if (enableDrunkSteering)
			{
				// Give player back control

				timeUntilSteer += g_random.GetRandomInt(100, 500);
			}
			else
			{
				// Take control from player

				steering = GET_RANDOM_FLOAT_IN_RANGE(-.5f, .5f);

				timeUntilSteer += g_random.GetRandomInt(50, 300);
			}

			enableDrunkSteering = !enableDrunkSteering;
		}
	}
}


static void OnTick()
{
	if (goingRight)
	{
		offsetX += SPEED;
		if (offsetX + boxWidth >= 1)
		{
			goingRight = false;
		}
	}
	else
	{
		offsetX -= SPEED;
		if (offsetX <= 0)
		{
			goingRight = true;
		}
	}
	if (goingDown)
	{
		offsetY += SPEED;
		if (offsetY + boxHeight >= 1)
		{
			goingDown = false;
		}
	}
	else
	{
		offsetY -= SPEED;
		if (offsetY <= 0)
		{
			goingDown = true;
		}
	}
	DRAW_RECT(0.5, offsetY / 2, 1, offsetY, 0, 0, 0, 255, false); // Top bar
	double lowerHeight = (1 - offsetY - boxHeight);
	DRAW_RECT(0.5, 1 - (lowerHeight / 2), 1, lowerHeight, 0, 0, 0, 255, false); // Bottom bar
	DRAW_RECT(offsetX / 2, 0.5, offsetX, 1, 0, 0, 0, 255, false); // Left bar
	double rightWidth = (1 - offsetX - boxWidth);
	DRAW_RECT(1 - (rightWidth / 2), 0.5, rightWidth, 1, 0, 0, 0, 255, false); // Right bar

	StartTransitionTimecycle("hud_def_blur");
	StartTransitionTimecycle("Bloom");

	int current_time = GET_GAME_TIMER();

	// Launch a firework every 500 miliseconds
	if (current_time - last_firework > 500)
	{
		last_firework = current_time;

		Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
		REQUEST_NAMED_PTFX_ASSET("proj_indep_firework_v2");
		while (!HAS_NAMED_PTFX_ASSET_LOADED("proj_indep_firework_v2"))
		{
			WAIT(0);
		}
		USE_PARTICLE_FX_ASSET("proj_indep_firework_v2");

		int x = pos.x + g_random.GetRandomInt(-220, 220);
		int y = pos.y + g_random.GetRandomInt(-220, 220);
		int z = pos.z + g_random.GetRandomInt(50, 150);
		START_PARTICLE_FX_NON_LOOPED_AT_COORD(g_random.GetRandomInt(0, 1) == 0 ? "scr_firework_indep_ring_burst_rwb" : "scr_firework_indep_spiral_burst_rwb", x, y, z, 0.0f, 0.0f, 0.0f, 2.0f, true, true, true);
		// Spawn an explosion to make an explosion sound and a screen shake (explosionType 38 = fireworks)
		ADD_EXPLOSION(x, y, z, 38, 1.0f, true, false, 1.0f, false);

		SET_ARTIFICIAL_LIGHTS_STATE(true);
	}

	Drunk();
}

static void OnStop()
{
	SET_ARTIFICIAL_LIGHTS_STATE(false);

	Ped playerPed = PLAYER_PED_ID();

	SET_PED_IS_DRUNK(playerPed, false);

	RESET_PED_MOVEMENT_CLIPSET(playerPed, .0f);

	REMOVE_CLIP_SET("MOVE_M@DRUNK@VERYDRUNK");

	STOP_GAMEPLAY_CAM_SHAKING(true);

	_0x487A82C650EB7799(.0f);
	_0x0225778816FDC28C(.0f);

	CLEAR_TIMECYCLE_MODIFIER();
}


static RegisterEffect registerEffect(EFFECT_TWENTY_TWENTY, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "The 2020 experience",
		.Id = "misc_twenty_twenty",
		.IsTimed = true,
		.IsShortDuration = true
	}
);