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

	for (Ped ped : GetAllPeds()) {
		RandomizeClothesForPed(ped);
	}
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

static void OnStart()
{
	OnStartRandomizeClothes();
}

static void OnStop() {
	OnStopDancing();
}

static void OnTick() {
	OnTickDancing();
}

static RegisterEffect registerEffect(EFFECT_MARDI_GRAS, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Mardi Gras",
		.Id = "misc_mardi_gras",
		.IsTimed = true,
		.IsShortDuration = true
	}
);