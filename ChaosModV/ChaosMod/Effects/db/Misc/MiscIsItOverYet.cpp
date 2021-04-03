#include <stdafx.h>

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

static int s_alpha;
static float s_alphaTimer;

static void OnStartCredits()
{
	s_alpha = 0;
	s_alphaTimer = 0.f;

	REQUEST_ADDITIONAL_TEXT("CREDIT", 0);
	while (!HAS_ADDITIONAL_TEXT_LOADED(0))
	{
		WAIT(0);
	}

	PLAY_END_CREDITS_MUSIC(true);
	SET_CREDITS_ACTIVE(true);
	SET_MOBILE_PHONE_RADIO_STATE(true);

	int song = g_random.GetRandomInt(0, 2);
	if (song == 0)
	{
		SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "END_CREDITS_SAVE_MICHAEL_TREVOR", 1);
	}
	else if (song == 1)
	{
		SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "END_CREDITS_KILL_MICHAEL", 1);
	}
	else
	{
		SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "END_CREDITS_KILL_TREVOR", 1);
	}
}

static void OnStopCredits()
{
	SET_ENTITY_INVINCIBLE(PLAYER_PED_ID(), false);

	SET_CREDITS_ACTIVE(false);
	PLAY_END_CREDITS_MUSIC(false);
	SET_MOBILE_PHONE_RADIO_STATE(false);

	SET_USER_RADIO_CONTROL_ENABLED(true);
}

static void OnTickCredits()
{
	SET_ENTITY_INVINCIBLE(PLAYER_PED_ID(), true);

	SET_RADIO_TO_STATION_NAME("RADIO_16_SILVERLAKE");

	SET_USER_RADIO_CONTROL_ENABLED(false);

	if (s_alpha < 255 && (s_alphaTimer += GET_FRAME_TIME()) > 0.1f)
	{
		s_alphaTimer = 0;
		s_alpha++;
	}

	DRAW_RECT(.5f, .5f, 1.f, 1.f, 0, 0, 0, s_alpha, false);
}

static void OnTick()
{
	OnTickTired();
	OnTickCredits();
}

static void OnStop()
{
	OnStopTired();
	OnStopCredits();
}

static void OnStart()
{
	OnStartTired();
	OnStartCredits();
}

static RegisterEffect registerEffect(EFFECT_IS_IT_OVER_YET, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Is it over yet",
		.Id = "misc_is_it_over_yet",
		.IsTimed = true,
		.IsShortDuration = true
	}
);