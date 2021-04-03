#include <stdafx.h>

static void ForceFieldTick() {
	Ped player = PLAYER_PED_ID();
	std::vector<Entity> entities;

	for (Ped ped : GetAllPeds())
	{
		if (ped != player) {
			entities.push_back(ped);
		}
	}

	for (Vehicle veh : GetAllVehs())
	{
		if (!IS_PED_IN_VEHICLE(player, veh, false))
		{
			entities.push_back(veh);
		}
	}

	for (Entity prop : GetAllProps())
	{
		entities.push_back(prop);
	}

	Vector3 playerCoord = GET_ENTITY_COORDS(player, false);
	for (Entity entity : entities)
	{
		static float startDistance = 15;
		static float maxForceDistance = 10;
		static float maxForce = 100;
		Vector3 entityCoord = GET_ENTITY_COORDS(entity, false);
		float distance = GET_DISTANCE_BETWEEN_COORDS(playerCoord.x, playerCoord.y, playerCoord.z, entityCoord.x, entityCoord.y, entityCoord.z, true);
		if (distance < startDistance)
		{
			if (IS_ENTITY_A_PED(entity) && !IS_PED_RAGDOLL(entity))
			{
				SET_PED_TO_RAGDOLL(entity, 5000, 5000, 0, true, true, false);
			}
			float forceDistance = min(max(0.f, (startDistance - distance)), maxForceDistance);
			float force = (forceDistance / maxForceDistance) * maxForce;
			APPLY_FORCE_TO_ENTITY(entity, 3, entityCoord.x - playerCoord.x, entityCoord.y - playerCoord.y, entityCoord.z - playerCoord.z, 0, 0, 0, false, false, true, true, false, true);
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

static void OnTickPlayerKeepRunning()
{
	SIMULATE_PLAYER_INPUT_GAIT(PLAYER_ID(), 5.f, 100, 1.f, true, false);

	_SET_CONTROL_NORMAL(0, 32, 1);
	_SET_CONTROL_NORMAL(0, 71, 1);
	_SET_CONTROL_NORMAL(0, 77, 1);
	_SET_CONTROL_NORMAL(0, 87, 1);
	_SET_CONTROL_NORMAL(0, 129, 1);
	_SET_CONTROL_NORMAL(0, 136, 1);
	_SET_CONTROL_NORMAL(0, 150, 1);
	_SET_CONTROL_NORMAL(0, 232, 1);
	_SET_CONTROL_NORMAL(0, 280, 1);

	// Disable all brake and descend actions
	DISABLE_CONTROL_ACTION(0, 72, true);
	DISABLE_CONTROL_ACTION(0, 76, true);
	DISABLE_CONTROL_ACTION(0, 88, true);
	DISABLE_CONTROL_ACTION(0, 138, true);
	DISABLE_CONTROL_ACTION(0, 139, true);
	DISABLE_CONTROL_ACTION(0, 152, true);
	DISABLE_CONTROL_ACTION(0, 153, true);

	// Disable aiming actions, would cancel forward movement
	DISABLE_CONTROL_ACTION(0, 25, true);  //	INPUT_AIM
	DISABLE_CONTROL_ACTION(0, 44, true);  //  	INPUT_COVER
	DISABLE_CONTROL_ACTION(0, 50, true);  //  	INPUT_ACCURATE_AIM
	DISABLE_CONTROL_ACTION(0, 68, true);  //  	INPUT_VEH_AIM
}


static void OnTick()
{
	ForceFieldTick();
	OnTickX10();
	OnTickPlayerKeepRunning();
}

static RegisterEffect registerEffect(EFFECT_MR_IMPORTANT, nullptr, nullptr, OnTick, EffectInfo
	{
		.Name = "Mr Important",
		.Id = "misc_mr_important",
		.IsTimed = true,
		.IsShortDuration = true
	}
);