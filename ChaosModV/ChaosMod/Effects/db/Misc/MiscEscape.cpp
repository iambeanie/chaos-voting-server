#include <stdafx.h>

static DWORD64 m_anchorTick;

static void OnStartTotalChaos()
{
	m_anchorTick = GET_GAME_TIMER();

	SET_WEATHER_TYPE_OVERTIME_PERSIST("THUNDER", 2.f);
}

static void OnStopTotalChaos()
{
	CLEAR_WEATHER_TYPE_PERSIST();

	SET_WEATHER_TYPE_NOW("EXTRASUNNY");
}

static void OnTickTotalChaos()
{
	Ped playerPed = PLAYER_PED_ID();
	Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);

	for (Vehicle veh : GetAllVehs())
	{
		if (veh != playerVeh)
		{
			APPLY_FORCE_TO_ENTITY(veh, 3, 10.f, .1f, .1f, 0, 0, 0, 0, true, true, true, false, true);
		}
	}

	for (Object prop : GetAllProps())
	{
		APPLY_FORCE_TO_ENTITY(prop, 3, 10.f, 5.f, .1f, 0, 0, 0, 0, true, true, true, false, true);
	}

	DWORD64 curTick = GET_GAME_TIMER();

	static DWORD64 lastTick = GET_GAME_TIMER();
	if (lastTick < curTick - 100)
	{
		lastTick = curTick;

		SHAKE_GAMEPLAY_CAM("SMALL_EXPLOSION_SHAKE", .1f);
	}

	// Make sure weather is always set to thunder after the transition
	if (m_anchorTick < curTick - 2000)
	{
		SET_WEATHER_TYPE_NOW("THUNDER");
	}

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

				timeUntilSteer += g_random.GetRandomInt(50, 250);
			}
			else
			{
				// Take control from player

				steering = GET_RANDOM_FLOAT_IN_RANGE(-.7f, .7f);

				timeUntilSteer += g_random.GetRandomInt(50, 300);
			}

			enableDrunkSteering = !enableDrunkSteering;
		}
	}
}

static void OnTickFlyingCars()
{
	Ped player = PLAYER_PED_ID();
	if (IS_PED_IN_ANY_VEHICLE(player, 0))
	{
		Vehicle veh = GET_VEHICLE_PED_IS_IN(player, 0);
		int vehClass = GET_VEHICLE_CLASS(veh);
		if (vehClass == 15 || vehClass == 16)
		{
			return;
		}
		float currentSpeed = GET_ENTITY_SPEED(veh);
		float maxSpeed = GET_VEHICLE_MODEL_ESTIMATED_MAX_SPEED(GET_ENTITY_MODEL(veh));
		if (currentSpeed < maxSpeed * 0.6)
		{
			return;
		}
		if (IS_CONTROL_PRESSED(0, 71)) //Forward
		{
			SET_VEHICLE_FORWARD_SPEED(veh, min(maxSpeed * 3, currentSpeed + 0.3));
		}

		if (vehClass == 14 || !IS_VEHICLE_ON_ALL_WHEELS(veh)) // Allow Steering if not "on ground" or boat in water
		{
			Vector3 rot = GET_ENTITY_ROTATION(veh, 2);
			if (IS_CONTROL_PRESSED(0, 63)) //Turn Left
			{
				rot.z += 1.0;
			}
			if (IS_CONTROL_PRESSED(0, 64)) //Turn Right
			{
				rot.z -= 1.0;
			}

			if (IS_CONTROL_PRESSED(0, 108)) //Roll Left
			{
				rot.y -= 1.0;
			}

			if (IS_CONTROL_PRESSED(0, 109)) //Roll Right
			{
				rot.y += 1.0;
			}

			if (IS_CONTROL_PRESSED(0, 111)) //Tilt Down
			{
				rot.x -= 1.0;
			}

			if (IS_CONTROL_PRESSED(0, 112)) //Tilt Up
			{
				rot.x += 1.0;
			}
			SET_ENTITY_ROTATION(veh, rot.x, rot.y, rot.z, 2, 1);
		}
	}
}

static void OnStartSkyFall()
{
	TeleportPlayer(935.f, 3800.f, 2300.f);
}

static void OnStartRandomVehicle()
{
	Ped playerPed = PLAYER_PED_ID();
	bool playerPedInAnyVehicle = IS_PED_IN_ANY_VEHICLE(playerPed, false);
	Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);

	std::vector<Vehicle> vehs;

	for (Vehicle veh : GetAllVehs())
	{
		if (veh != playerVeh)
		{
			vehs.push_back(veh);
		}
	}

	if (!vehs.empty())
	{
		Hash playerVehModel = GET_ENTITY_MODEL(playerVeh);
		int playerVehMaxSeats = GET_VEHICLE_MODEL_NUMBER_OF_SEATS(playerVehModel);

		// Store all ped in current vehicle (if existant) to set them into chosen vehicle afterwards if possible

		std::vector<Ped> teleportPeds;
		teleportPeds.push_back(playerPed);

		if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
		{
			for (int i = -1; i < playerVehMaxSeats - 1; i++)
			{
				if (!IS_VEHICLE_SEAT_FREE(playerVeh, i, false))
				{
					Ped seatPed = GET_PED_IN_VEHICLE_SEAT(playerVeh, i, false);

					if (seatPed != playerPed)
					{
						teleportPeds.push_back(seatPed);
					}
				}
			}
		}

		Vehicle targetVeh = vehs[g_random.GetRandomInt(0, vehs.size() - 1)];
		Hash targetVehModel = GET_ENTITY_MODEL(targetVeh);
		int targetVehMaxSeats = GET_VEHICLE_MODEL_NUMBER_OF_SEATS(targetVehModel);

		for (int i = 0; i < teleportPeds.size(); i++)
		{
			if (i >= targetVehMaxSeats)
			{
				break;
			}

			Ped ped = teleportPeds[i];

			if (!IS_VEHICLE_SEAT_FREE(targetVeh, i - 1, false))
			{
				Ped seatPed = GET_PED_IN_VEHICLE_SEAT(targetVeh, i - 1, false);

				CLEAR_PED_TASKS_IMMEDIATELY(seatPed);

				WAIT(0);
			}

			SET_PED_INTO_VEHICLE(ped, targetVeh, i - 1);
		}
	}
}

static void OnTick()
{
	OnTickTotalChaos();
	OnTickFlyingCars();
}

static void OnStop()
{
	OnStopTotalChaos();
}

static void OnStart()
{
	OnStartSkyFall();
	OnStartTotalChaos();
	OnStartRandomVehicle();
}

static RegisterEffect registerEffect(EFFECT_ESCAPE, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "Escape",
		.Id = "misc_escape",
		.IsTimed = true,
		.IsShortDuration = true
	}
);