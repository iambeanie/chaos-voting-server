#include <stdafx.h>

static struct VehEntry
{
	VehEntry(Vehicle veh) : Veh(veh)
	{

	}

	bool operator==(Vehicle veh)
	{
		return Veh == veh;
	}

	const Vehicle Veh;
	DWORD64 LastTimestamp = 0;
};

static void OnTick()
{
	int count = 5;

	float force = 100;
	float velocityMultiplier = 3;

	float velFactor;
	for (auto veh : GetAllVehs())
	{
		SET_ENTITY_INVINCIBLE(veh, true);
		//SET_VEHICLE_FIXED(veh);
		if (HAS_ENTITY_COLLIDED_WITH_ANYTHING(veh))
		{
			Vector3 vel = GET_ENTITY_VELOCITY(veh);
			if ((vel.x < 10) && (vel.y < 10) && (vel.z < 10))
			{
				velFactor = 300.f;
			}
			else
			{
				velFactor = 60.f;
			}
			APPLY_FORCE_TO_ENTITY(veh, 0, vel.x * -velFactor, vel.y * -velFactor, vel.z * -velFactor, .0f, .0f, .0f, 0, true, true, true, false, true);

		}

		bool doBeyblade = IS_VEHICLE_SEAT_FREE(veh, -1, false) ? true : !IS_PED_A_PLAYER(GET_PED_IN_VEHICLE_SEAT(veh, -1, false));

		if (doBeyblade)
		{
			APPLY_FORCE_TO_ENTITY(veh, 3, force, 0, 0, 0, 4, 0, 0, true, true, true, true, true);
			APPLY_FORCE_TO_ENTITY(veh, 3, -force, 0, 0, 0, -4, 0, 0, true, true, true, true, true);
			SET_ENTITY_INVINCIBLE(veh, true);
			SET_VEHICLE_REDUCE_GRIP(veh, true);
			if (GET_ENTITY_SPEED(veh) < 10)
			{
				Vector3 velocity = GET_ENTITY_SPEED_VECTOR(veh, true);
				velocity.x *= velocityMultiplier;
				velocity.y *= velocityMultiplier;
				SET_ENTITY_VELOCITY(veh, velocity.x, velocity.y, velocity.z);
			}
		}

		if (--count == 0)
		{
			count = 5;

			WAIT(0);
		}
	}
}

static void OnStop()
{
	for (auto veh : GetAllVehs())
	{
		SET_ENTITY_INVINCIBLE(veh, false);
		SET_VEHICLE_REDUCE_GRIP(veh, false);
	}
}

static RegisterEffect registerEffect(EFFECT_COMBO_TWIRLING, nullptr, OnStop, OnTick);