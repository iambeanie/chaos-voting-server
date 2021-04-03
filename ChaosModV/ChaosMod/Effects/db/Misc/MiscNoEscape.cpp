#include <stdafx.h>

static void OnStartPedsAttackPlayer()
{
	Hash groupHash;
	ADD_RELATIONSHIP_GROUP("_ATTACK_PLAYER", &groupHash);
}

static void OnTickPedsAttackPlayer()
{
	static const Hash enemyGroupHash = GET_HASH_KEY("_ATTACK_PLAYER");
	static const Hash playerGroupHash = GET_HASH_KEY("PLAYER");

	SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroupHash, playerGroupHash);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroupHash, enemyGroupHash);

	Player player = PLAYER_ID();
	Ped playerPed = PLAYER_PED_ID();
	int playerGroup = GET_PLAYER_GROUP(player);

	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped))
		{
			if (IS_PED_IN_GROUP(ped) && GET_PED_GROUP_INDEX(ped) == playerGroup)
			{
				REMOVE_PED_FROM_GROUP(ped);
			}

			SET_PED_RELATIONSHIP_GROUP_HASH(ped, enemyGroupHash);

			SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
			SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

			SET_PED_FLEE_ATTRIBUTES(ped, 2, true);

			TASK_COMBAT_PED(ped, playerPed, 0, 16);
		}
	}
}

static void OnTickExplosiveCombat()
{
	Player player = PLAYER_ID();

	SET_EXPLOSIVE_MELEE_THIS_FRAME(player);
	SET_EXPLOSIVE_AMMO_THIS_FRAME(player);

	Vector3 impactCoords;

	for (Ped ped : GetAllPeds())
	{
		if (GET_PED_LAST_WEAPON_IMPACT_COORD(ped, &impactCoords))
		{
			ADD_EXPLOSION(impactCoords.x, impactCoords.y, impactCoords.z, 4, 9999.f, true, false, 1.f, false);
		}
	}
}

static void OnStartMinigun()
{
	static const Hash minigunHash = GET_HASH_KEY("WEAPON_MINIGUN");

	for (Ped ped : GetAllPeds())
	{
		GIVE_WEAPON_TO_PED(ped, minigunHash, 9999, true, true);
		SET_CURRENT_PED_WEAPON(ped, minigunHash, true);
	}
}

static void OnStartPedsRiot()
{
	Hash groupHash;
	ADD_RELATIONSHIP_GROUP("_RIOT", &groupHash);
}

static void OnStopPedsRiot()
{
	SET_MAX_WANTED_LEVEL(5);
}

static void OnTickPedsRiot()
{
	static const Hash riotGroupHash = GET_HASH_KEY("_RIOT");
	static const Hash playerGroupHash = GET_HASH_KEY("PLAYER");

	SET_RELATIONSHIP_BETWEEN_GROUPS(5, riotGroupHash, riotGroupHash);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, riotGroupHash, playerGroupHash);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroupHash, riotGroupHash);

	SET_PLAYER_WANTED_LEVEL(PLAYER_ID(), 0, false);
	SET_MAX_WANTED_LEVEL(0);

	static std::list<Ped> goneThroughPeds;

	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped))
		{
			SET_PED_RELATIONSHIP_GROUP_HASH(ped, riotGroupHash);

			SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
			SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

			SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);

			if (std::find(goneThroughPeds.begin(), goneThroughPeds.end(), ped) == goneThroughPeds.end())
			{
				static const std::vector<Hash>& weps = Memory::GetAllWeapons();
				GIVE_WEAPON_TO_PED(ped, weps[g_random.GetRandomInt(0, weps.size() - 1)], 9999, false, true);

				goneThroughPeds.push_back(ped);
			}
		}
	}

	for (std::list<Ped>::iterator it = goneThroughPeds.begin(); it != goneThroughPeds.end(); )
	{
		if (!DOES_ENTITY_EXIST(*it))
		{
			it = goneThroughPeds.erase(it);
		}
		else
		{
			it++;
		}
	}
}

static void OnStopInvincible()
{
	SET_PLAYER_INVINCIBLE(PLAYER_ID(), false);
}

static void OnTickInvincible()
{
	SET_PLAYER_INVINCIBLE(PLAYER_ID(), true);
}


static void OnTick()
{
	OnTickPedsAttackPlayer();
	OnTickExplosiveCombat();
	OnTickPedsRiot();
	OnTickInvincible();
}

static void OnStop()
{
	OnStopPedsRiot();
	OnStopInvincible();
}

static void OnStart()
{
	OnStartPedsAttackPlayer();
	OnStartMinigun();
	OnStartPedsRiot();
}

static RegisterEffect registerEffect(EFFECT_NO_ESCAPE, OnStart, OnStop, OnTick, EffectInfo
	{
		.Name = "No escape",
		.Id = "misc_no_escape",
		.IsTimed = true,
		.IncompatibleWith = { EFFECT_PEDS_COPS }
	}
);