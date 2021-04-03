#include <stdafx.h>

static void OnStartKillEngine()
{
	for (Vehicle veh : GetAllVehs())
	{
		SET_VEHICLE_ENGINE_HEALTH(veh, 0.f);
	}
}

static void OnStopTirePop()
{
	for (Vehicle veh : GetAllVehs())
	{
		for (int i = 0; i < 47; i++)
		{
			SET_VEHICLE_TYRE_FIXED(veh, i);
		}
	}
}

static void OnTickTirePop()
{
	for (Vehicle veh : GetAllVehs())
	{
		for (int i = 0; i < 48; i++)
		{
			SET_VEHICLE_TYRES_CAN_BURST(veh, true);
			SET_VEHICLE_TYRE_BURST(veh, i, true, 1000.f);
		}
	}
}

static void OnTickX10()
{
	for (auto veh : GetAllVehs())
	{
		MODIFY_VEHICLE_TOP_SPEED(veh, 10.f);
		SET_VEHICLE_CHEAT_POWER_INCREASE(veh, 10.f);
	}
}

static void OnStopX10()
{
	for (auto veh : GetAllVehs())
	{
		MODIFY_VEHICLE_TOP_SPEED(veh, 1.f);
		SET_VEHICLE_CHEAT_POWER_INCREASE(veh, 1.f);
	}
}

static void OnTickSlidyPeds()
{
	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped) && !IS_PED_IN_ANY_VEHICLE(ped, false))
		{
			TASK_JUMP(ped, false, false, false);
		}
	}
}

static void OnTickOilSlicks()
{
	std::vector<Entity> entities;

	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_IN_ANY_VEHICLE(ped, false) && IS_PED_HUMAN(ped) && !IS_PED_SWIMMING(ped) && !IS_PED_SWIMMING_UNDER_WATER(ped) && !IS_PED_DEAD_OR_DYING(ped, true))
		{
			entities.push_back(ped);
		}
	}

	for (Vehicle veh : GetAllVehs())
	{
		Hash model = GET_ENTITY_MODEL(veh);
		if (GET_IS_VEHICLE_ENGINE_RUNNING(veh) && !IS_THIS_MODEL_A_HELI(model) && !IS_THIS_MODEL_A_PLANE(model) && !IS_THIS_MODEL_A_BOAT(model))
		{
			entities.push_back(veh);
		}
	}

	int count = 25;
	for (Entity entity : entities)
	{
		if (!DOES_ENTITY_EXIST(entity))
		{
			continue;
		}
		Vector3 entityPos = GET_ENTITY_COORDS(entity, false);
		float groundZ = 0;
		if (GET_GROUND_Z_FOR_3D_COORD(entityPos.x, entityPos.y, entityPos.z, &groundZ, false, false))
		{
			ADD_PETROL_DECAL(entityPos.x, entityPos.y, groundZ, 2, 2, 1);
		}

		if (--count == 0)
		{
			count = 25;

			WAIT(0);
		}
	}
}

static void OnStopSnow()
{
	Memory::SetSnow(false);

	SET_WEATHER_TYPE_NOW("EXTRASUNNY");
}

static void OnTickSnow()
{
	Memory::SetSnow(true);

	SET_WEATHER_TYPE_NOW("XMAS");
}

static void OnTick()
{
	OnTickTirePop();
	OnTickX10();
	OnTickSlidyPeds();
	OnTickOilSlicks();
	OnTickSnow();
}

static void OnStop()
{
	OnStopTirePop();
	OnStopX10();
	OnStopSnow();
}

static void OnStart()
{
	OnStartKillEngine();
}

static RegisterEffect registerEffect(EFFECT_ICE_TO_MEET_YOU, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Ice to meet you",
		.Id = "misc_ice_to_meet_you",
		.IsTimed = true,
		.IsShortDuration = true,
		.IncompatibleWith = { EFFECT_2XENGINE_VEHS, EFFECT_05XENGINE_VEHS }
	}
);