#pragma once

#include <unordered_map>
#include <Effects/EffectData.h>
#include <Effects/EffectData.h>
#include <Effects/EnabledEffectsMap.h>
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

class FileOutput
{
public:
	static void outputRandomEffectsToFile(float totalEffectsWeight, std::unordered_map<EffectIdentifier, EffectData, EffectsIdentifierHasher> choosableEffects);
	static const EffectIdentifier* readVoteResultFromVoteFile(std::unordered_map<EffectIdentifier, EffectData, EffectsIdentifierHasher>& choosableEffects);
	static std::string readRoomCodeFromFile();
private:
	FileOutput() {}
};