#include <stdafx.h>

static void OnTickMidas()
{
	Ped playerPed = PLAYER_PED_ID();
	int cE = playerPed; // COMPARE ENTITY
	
	static const auto model = GET_HASH_KEY("prop_money_bag_01");
	REQUEST_MODEL(model);

	//remove current vehicle
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);

		std::vector<Ped> vehPeds;

		Hash vehModel = GET_ENTITY_MODEL(playerVeh);
		int maxSeats = GET_VEHICLE_MODEL_NUMBER_OF_SEATS(vehModel);
		for (int i = -1; i < maxSeats - 1; i++)
		{
			if (IS_VEHICLE_SEAT_FREE(playerVeh, i, false))
			{
				continue;
			}

			Ped ped = GET_PED_IN_VEHICLE_SEAT(playerVeh, i, false);

			CLEAR_PED_TASKS_IMMEDIATELY(ped);

			SET_PED_TO_RAGDOLL(ped, 5000, 5000, 0, true, true, false);

			vehPeds.push_back(ped);
		}

		Vector3 vehVel = GET_ENTITY_VELOCITY(playerVeh);

		auto pos = GET_ENTITY_COORDS(playerVeh, false);
		CREATE_AMBIENT_PICKUP(GET_HASH_KEY("PICKUP_MONEY_SECURITY_CASE"), pos.x, pos.y, pos.z, 0, 1000, model, false, true);
		SET_ENTITY_COORDS(playerVeh, 0, 0, 0, 1, 0, 0, 1);

		SET_ENTITY_AS_MISSION_ENTITY(playerVeh, true, true);	

		DELETE_VEHICLE(&playerVeh);

		WAIT(0);

		for (Ped ped : vehPeds)
		{
			SET_ENTITY_VELOCITY(ped, vehVel.x, vehVel.y, vehVel.z);
		}
	}

	for (Vehicle veh : GetAllVehs())
	{
		if (IS_ENTITY_TOUCHING_ENTITY(cE, veh))
		{
			std::vector<Ped> vehPeds;

			Hash vehModel = GET_ENTITY_MODEL(veh);
			int maxSeats = GET_VEHICLE_MODEL_NUMBER_OF_SEATS(vehModel);
			for (int i = -1; i < maxSeats - 1; i++)
			{
				if (IS_VEHICLE_SEAT_FREE(veh, i, false))
				{
					continue;
				}

				Ped ped = GET_PED_IN_VEHICLE_SEAT(veh, i, false);

				CLEAR_PED_TASKS_IMMEDIATELY(ped);

				SET_PED_TO_RAGDOLL(ped, 5000, 5000, 0, true, true, false);

				vehPeds.push_back(ped);
			}

			Vector3 vehVel = GET_ENTITY_VELOCITY(veh);

			auto pos = GET_ENTITY_COORDS(veh, false);
			CREATE_AMBIENT_PICKUP(GET_HASH_KEY("PICKUP_MONEY_SECURITY_CASE"), pos.x, pos.y, pos.z, 0, 1000, model, false, true);
			SET_ENTITY_COORDS(veh, 0, 0, 0, 1, 0, 0, 1);

			DELETE_VEHICLE(&veh);

			WAIT(0);

			for (Ped ped : vehPeds)
			{
				SET_ENTITY_VELOCITY(ped, vehVel.x, vehVel.y, vehVel.z);
			}
		}
	}

	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped))
		{
			if (!IS_ENTITY_A_MISSION_ENTITY(ped) || IS_CUTSCENE_PLAYING()) // lets some fun happen in cutscenes, in theorey.
			{
				if (IS_ENTITY_TOUCHING_ENTITY(cE, ped))
				{
					auto pos = GET_ENTITY_COORDS(ped, false);
					CREATE_AMBIENT_PICKUP(GET_HASH_KEY("PICKUP_MONEY_SECURITY_CASE"), pos.x, pos.y, pos.z, 0, 1000, model, false, true);
					SET_ENTITY_COORDS(ped, 0, 0, 0, 1, 0, 0, 1);
					SET_PED_AS_NO_LONGER_NEEDED(&ped);
					DELETE_PED(&ped);
				}
			}
		}
	}

	for (int prop : GetAllProps())
	{
		if (IS_ENTITY_TOUCHING_ENTITY(cE, prop) && !IS_PED_CLIMBING(cE))
		{
			if (!IS_ENTITY_A_MISSION_ENTITY(prop) || IS_CUTSCENE_PLAYING())
			{
				if (GET_ENTITY_ATTACHED_TO(prop) == false)
				{
					auto pos = GET_ENTITY_COORDS(prop, false);
					CREATE_AMBIENT_PICKUP(GET_HASH_KEY("PICKUP_MONEY_SECURITY_CASE"), pos.x, pos.y, pos.z, 0, 1000, model, false, true);
					SET_ENTITY_COORDS(prop, 0, 0, 0, 1, 0, 0, 1);
					SET_ENTITY_AS_NO_LONGER_NEEDED(&prop);
					DELETE_ENTITY(&prop);
				}
			}
		}
	}

	if (IS_PED_ARMED(playerPed, 7))
	{
		Hash weaponHash;
		GET_CURRENT_PED_WEAPON(playerPed, &weaponHash, false);
		SET_PED_WEAPON_TINT_INDEX(playerPed, weaponHash, 2);
	}

}


static void OnStartTpEverythingToPlayer()
{
	std::vector<Entity> entities;
	int maxEntities = 20;

	for (auto ped : GetAllPeds())
	{
		if (maxEntities == 10)
		{
			break;
		}

		maxEntities--;

		if (!IS_PED_A_PLAYER(ped) && !IS_ENTITY_A_MISSION_ENTITY(ped))
		{
			SET_ENTITY_AS_MISSION_ENTITY(ped, true, true);
			entities.push_back(ped);
		}
	}

	auto playerPed = PLAYER_PED_ID();

	for (auto veh : GetAllVehs())
	{
		if (maxEntities == 0)
		{
			break;
		}

		maxEntities--;

		if ((!IS_PED_IN_ANY_VEHICLE(playerPed, false) || veh != GET_VEHICLE_PED_IS_IN(playerPed, false)) && !IS_ENTITY_A_MISSION_ENTITY(veh))
		{
			SET_ENTITY_AS_MISSION_ENTITY(veh, true, true);
			entities.push_back(veh);
		}
	}

	auto playerPos = GET_ENTITY_COORDS(playerPed, false);

	for (auto entity : entities)
	{
		SET_ENTITY_COORDS(entity, playerPos.x, playerPos.y, playerPos.z, false, false, false, false);
	}

	WAIT(0);

	for (auto entity : entities)
	{
		SET_ENTITY_AS_MISSION_ENTITY(entity, false, false);
		SET_ENTITY_AS_NO_LONGER_NEEDED(&entity);
	}
}


static void OnTick()
{
	OnTickMidas();
}

static void OnStart()
{
	OnStartTpEverythingToPlayer();
}

static RegisterEffect registerEffect(EFFECT_MONEY_MAN, OnStart, nullptr, OnTick, EffectInfo
	{
		.Name = "Money Man",
		.Id = "misc_money_man",
		.IsTimed = true,
		.IsShortDuration = true
	}
);