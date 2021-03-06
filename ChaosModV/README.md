# Chaos Mod V

NOTE: This is a copy of the existing chaos mod V, changed slightly to accommodate the voting server.

A replica of the chaos mods found on previous GTA games for GTA V.

See the [GTA5-Mods mod page](https://www.gta5-mods.com/scripts/chaos-mod-v-beta) for more information and instructions on how to install it.

Feel free to join the Discord Server for community support or simply to stay up to date on this (and other) mods.

Also make sure to check the [Wiki](https://github.com/gta-chaos-mod/ChaosModV/wiki)!

[![](https://discord.com/api/guilds/785656433529716757/widget.png)](https://discord.gg/w2tDeKVaF9)

## Building

1. Clone the repo `git clone https://github.com/gta-chaos-mod/ChaosModV.git`

2. `cd ChaosModV`

3. Initialize all submodules

```
git submodule init
git submodule update --recursive
```

4. Open `vendor/minhook/build/VC15/MinHookVC15.sln` in Visual Studio

5. Compile libMinHook as x64 Release build

6. Open `ChaosMod.sln` in the root folder in Visual Studio

7. Compiling should work now. If there's an error referencing `MsBuildMajorVersion` when building either the ConfigApp or TwitchChatProxy projects, close and open Visual Studio again.

## Adding new effects

1. Add your new effect info to `ChaosMod/Effects/EffectsInfo.h`

2. Add the same info to `ConfigApp/Effects.cs`

3. Create a new .cpp file in the appropriate folder under `ChaosMod/Effects/db/` with a fitting name

Layout of the file should look like this:

```cpp
/*
	Effect by <Your Name>
*/

#include <stdafx.h>

static void OnStart()
{
	
}

static void OnStop()
{
	
}

static void OnTick()
{
	
}

static RegisterEffect registerEffect(EFFECT_ENUM_ENTRY, OnStart, OnStop, OnTick);
```

Any of these functions can be omitted and either replaced with a `nullptr` or completely left out (default parameter values) in the `RegisterEffect` declaration.
