#include <stdafx.h>

#include "Memory/Hooks/AudioPitchHook.h"

static int m_targetPitch;

static void OnStartHighPitch()
{
	m_targetPitch = g_random.GetRandomInt(750, 2000);
}

static void OnStopHighPitch()
{
	Hooks::ResetAudioPitch();
}

static void OnTickHighPitch()
{
	Hooks::SetAudioPitch(m_targetPitch);
}

static void OnStopMinions()
{
	for (Ped pd : GetAllPeds())
	{
		if (GET_PED_CONFIG_FLAG(pd, 223, true))
		{
			SET_PED_CONFIG_FLAG(pd, 223, false);
		}
	}
}

static void OnTickMinions()
{
	for (Ped pd : GetAllPeds())
	{
		if (!GET_PED_CONFIG_FLAG(pd, 223, true))
		{
			SET_PED_CONFIG_FLAG(pd, 223, true);
		}
	}
}

static void OnTick()
{
	OnTickHighPitch();
}

static void OnStop()
{
	OnStopHighPitch();
}

static void OnStart()
{
	OnStartHighPitch();
}

static RegisterEffect registerEffect(EFFECT_CHIPMUNKS, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Chipmunks",
		.Id = "misc_chipmunks",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_GAMESPEED_X02, EFFECT_GAMESPEED_X05 }
	}
);