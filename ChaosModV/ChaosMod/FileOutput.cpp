#pragma once

#include <unordered_map>
#include <Effects/EffectData.h>
#include <Effects/EffectData.h>
#include <Effects/EnabledEffectsMap.h>
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "FileOutput.h"

using namespace std;

void FileOutput::outputRandomEffectsToFile(float totalEffectsWeight, std::unordered_map<EffectIdentifier, EffectData, EffectsIdentifierHasher> choosableEffects) {
	std::unordered_map<EffectIdentifier, EffectData, EffectsIdentifierHasher> randomlyChosenEffects;

	while (randomlyChosenEffects.size() < 4) {
		int index = g_random.GetRandomInt(0, totalEffectsWeight);

		int addedUpWeight = 0;
		const std::pair<const EffectIdentifier, EffectData>* targetEffectIdentifier = nullptr;
		for (const auto& pair : choosableEffects)
		{
			if (pair.second.TimedType == EffectTimedType::TIMED_PERMANENT)
			{
				continue;
			}

			addedUpWeight += pair.second.Weight;

			if (index <= addedUpWeight)
			{
				targetEffectIdentifier = &pair;
				break;
			}
		}

		EffectData& targetEffectData = g_enabledEffects.at((*targetEffectIdentifier).first);

		bool effectExists = false;

		for (const auto& pair : randomlyChosenEffects)
		{
			if (pair.second.Id == targetEffectData.Id) {
				effectExists = true;
				break;
			}
		}

		if (!effectExists && targetEffectIdentifier != nullptr) {
			randomlyChosenEffects.emplace(targetEffectIdentifier->first, targetEffectIdentifier->second);
		}
	}

	std::fstream voteOptionsStream;

	voteOptionsStream.open("C:\\chaos\\voteoptions.txt", std::ios_base::out);

	for (const auto& pair : randomlyChosenEffects)
	{
		voteOptionsStream << pair.second.Id << "," << pair.second.Name << endl;
	}

	voteOptionsStream.flush();
	voteOptionsStream.close();
}

const EffectIdentifier* FileOutput::readVoteResultFromVoteFile(std::unordered_map<EffectIdentifier, EffectData, EffectsIdentifierHasher>& choosableEffects) {

	std::fstream votedEffectsStream;

	votedEffectsStream.open("C:\\chaos\\vote.txt", ios::in);

	if (!votedEffectsStream.is_open()) {
		return nullptr;
	}

	std::string votedEffectsContents;
	while (getline(votedEffectsStream, votedEffectsContents));

	votedEffectsStream.close();

	const EffectIdentifier* votedEffectIdentifier = nullptr;
	for (const auto& pair : choosableEffects)
	{
		if (pair.second.Id == votedEffectsContents) {
			LOG("Found a matching contents bucket");
			votedEffectIdentifier = &pair.first;
		}
	}

	return votedEffectIdentifier;
}

std::string FileOutput::readRoomCodeFromFile() {

	std::fstream roomCodeStream;

	roomCodeStream.open("C:\\chaos\\roomCode.txt", ios::in);

	if (!roomCodeStream.is_open()) {
		return "";
	}

	std::string roomCode;
	while (getline(roomCodeStream, roomCode));

	roomCodeStream.close();

	return roomCode;
}