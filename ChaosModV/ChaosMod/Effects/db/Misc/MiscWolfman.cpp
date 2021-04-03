#include <stdafx.h>

static void OnStartSpawnChop()
{
	static const Hash modelHash = GET_HASH_KEY("a_c_chop");

	Hash relationshipGroup;
	ADD_RELATIONSHIP_GROUP("_COMPANION_CHOP", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, relationshipGroup, GET_HASH_KEY("PLAYER"));
	SET_RELATIONSHIP_BETWEEN_GROUPS(0, GET_HASH_KEY("PLAYER"), relationshipGroup);

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	Ped ped = CreatePoolPed(28, modelHash, playerPos.x, playerPos.y, playerPos.z, GET_ENTITY_HEADING(playerPed));
	SET_PED_COMBAT_ATTRIBUTES(ped, 0, false);
	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
	SET_PED_HEARING_RANGE(ped, 9999.f);

	SET_PED_AS_GROUP_MEMBER(ped, GET_PLAYER_GROUP(PLAYER_ID()));
}

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


static void OnTick()
{
	OnTickPedsAttackPlayer();
}

static void OnStart()
{
	for (int i = 0; i < 50; i++) {
		OnStartSpawnChop();
	}

	OnStartPedsAttackPlayer();
}

static RegisterEffect registerEffect(EFFECT_WOLFMAN, OnStart, nullptr, OnTick, EffectInfo
	{
		.Name = "Wolfman",
		.Id = "misc_wolfman",
		.IsTimed = true
	}
);