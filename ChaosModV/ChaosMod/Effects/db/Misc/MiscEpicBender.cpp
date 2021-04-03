#include <stdafx.h>

static void RandomizeClothesForPed(Ped ped) {
	for (int i = 0; i < 12; i++)
	{
		int drawableAmount = GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(ped, i);
		int drawable = drawableAmount == 0 ? 0 : g_random.GetRandomInt(0, drawableAmount - 1);

		int textureAmount = GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(ped, i, drawable);
		int texture = textureAmount == 0 ? 0 : g_random.GetRandomInt(0, textureAmount - 1);

		SET_PED_COMPONENT_VARIATION(ped, i, drawable, texture, g_random.GetRandomInt(0, 3));

		if (i < 4)
		{
			int propDrawableAmount = GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(ped, i);
			int propDrawable = propDrawableAmount == 0 ? 0 : g_random.GetRandomInt(0, propDrawableAmount - 1);

			int propTextureAmount = GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(ped, i, drawable);
			int propTexture = propTextureAmount == 0 ? 0 : g_random.GetRandomInt(0, propTextureAmount - 1);

			SET_PED_PROP_INDEX(ped, i, propDrawable, propTexture, true);
		}
	}
}


static void OnStartRandomizeClothes()
{
	Ped playerPed = PLAYER_PED_ID();

	RandomizeClothesForPed(playerPed);
}


static void OnStopDancing()
{
	REMOVE_ANIM_DICT("missfbi3_sniping");
}

static void OnTickDancing()
{
	REQUEST_ANIM_DICT("missfbi3_sniping");

	for (auto ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped) && !IS_ENTITY_PLAYING_ANIM(ped, "missfbi3_sniping", "dance_m_default", 3))
		{
			TASK_PLAY_ANIM(ped, "missfbi3_sniping", "dance_m_default", 4.0f, -4.0f, -1.f, 1, 0.f, false, false, false);
		}
	}
}

static void OnStopDrunk()
{
	Ped playerPed = PLAYER_PED_ID();

	SET_PED_IS_DRUNK(playerPed, false);

	RESET_PED_MOVEMENT_CLIPSET(playerPed, .0f);

	REMOVE_CLIP_SET("MOVE_M@DRUNK@VERYDRUNK");

	STOP_GAMEPLAY_CAM_SHAKING(true);

	_0x487A82C650EB7799(.0f);
	_0x0225778816FDC28C(.0f);
}

static void OnTickDrunk()
{
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

enum TiredMode {
	closingEyes,
	openingEyes,
	waiting,
};
static TiredMode currentMode = TiredMode::closingEyes;
static float alpha;
static int closingIterator;
static int nextTimestamp;
static float steeringDirection;

static void BlackOut(float alpha)
{
	DRAW_RECT(.5f, .5f, 1.f, 1.f, 0, 0, 0, alpha, false);
}

static void SteerVehicle()
{
	Ped playerPed = PLAYER_PED_ID();
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vehicle veh = GET_VEHICLE_PED_IS_IN(playerPed, false);
		SET_VEHICLE_STEER_BIAS(veh, steeringDirection);
	}
}

static void RagdollOnFoot()
{
	Ped playerPed = PLAYER_PED_ID();
	if (!IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		int sleepDuration = 3000;
		SET_PED_TO_RAGDOLL(playerPed, sleepDuration, sleepDuration, 0, true, true, false);
	}
}

static void OnStopTired()
{
	BlackOut(0);
}

static void OnStartTired()
{
	currentMode = TiredMode::closingEyes;
	alpha = 0;
	closingIterator = 20;
}

static void OnTickTired()
{
	switch (currentMode)
	{
	case closingEyes:
		alpha += closingIterator;
		// Chance for player who's on foot to ragdoll halfway through blinking
		if (alpha / closingIterator == floor(255.f / closingIterator / 2.f) && g_random.GetRandomFloat(0.f, 1.f) < .25f)
		{
			RagdollOnFoot();
		}
		// Fall asleep at the wheel near the end of blinking
		if (alpha > 200.f)
		{
			SteerVehicle();
		}
		if (alpha >= 255)
		{
			currentMode = TiredMode::openingEyes;
			nextTimestamp = GET_GAME_TIMER() + ((20 - closingIterator) * 20);
			if (closingIterator > 1)
			{
				closingIterator = max(1, closingIterator - 2);
			}
		}
		break;
	case TiredMode::openingEyes:
		if (GET_GAME_TIMER() > nextTimestamp)
		{
			alpha -= 30;
			if (alpha <= 0)
			{
				alpha = 0;
				currentMode = TiredMode::waiting;
				nextTimestamp = GET_GAME_TIMER() + g_random.GetRandomInt(250, 3000);
			}
		}
		break;
	case TiredMode::waiting:
		if (GET_GAME_TIMER() > nextTimestamp)
		{
			currentMode = TiredMode::closingEyes;
			steeringDirection = (g_random.GetRandomFloat(0, 1) < .5f) ? 1.0f : -1.0f;
		}
		break;
	}
	BlackOut(alpha);
}

static const double maxBoxWidth = 0.35;
static double currentBoxWidth = 0;


static void OnStartPortrait()
{
	currentBoxWidth = 0;
}

static void OnTickPortrait()
{
	DRAW_RECT((currentBoxWidth / 2), 0.5, currentBoxWidth, 1, 0, 0, 0, 255, false); // Left bar
	DRAW_RECT(1 - (currentBoxWidth / 2), 0.5, currentBoxWidth, 1, 0, 0, 0, 255, false); // Right bar
	if (currentBoxWidth < maxBoxWidth)
	{
		currentBoxWidth += 0.01;
	}
}

static void OnStart()
{
	OnStartRandomizeClothes();
	OnStartTired();
	OnStartPortrait();
}

static void OnStop() {
	OnStopDancing();
	OnStopDrunk();
	OnStopTired();
}

static void OnTick() {
	OnTickDancing();
	OnTickDrunk();
	OnTickTired();
	OnTickPortrait();
}

static RegisterEffect registerEffect(EFFECT_EPIC_BENDER, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Epic Bender",
		.Id = "misc_epic_bender",
		.IsTimed = true,
		.IsShortDuration = true
	}
);