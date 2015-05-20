/*
 * Game.h
 *
 *  Created on: 14 May 2015
 *      Author: Jamie
 */

#ifndef GAME_H_
#define GAME_H_

#include "GlobalDefinitions.h"
#include "MappingDefinitions.h"
#include "Pokemon.h"

class NonPlayerCharacter;

typedef struct PokemonStorageBoxes {
	StorageBoxInfo info;
	AbridgedPokemon boxData[NUMBOXES][POKEMONPERBOX];
} PokemonStorageBoxes;

typedef struct NPCData {
	u16 xLocation;
	u16 yLocation;
	u8* scriptLocation;
	u8 spriteID;
	u8 directionFacing:4;
	u8 isActive:1;
	u8 isMoving:1;
	u8 unused:2;
	u8 oamStructID;
	u8 nextWalkingFrame;
	u8 previousWalkingFrame;
	u8 frameDelay;
	u8 pixelsMoved;
	u8 dataSpriteID;
} NPCData;

class Game
{
private:
	static Pokemon partyPokemon[];
	static Pokemon temporaryHoldingPokemon;
	static PokemonStorageBoxes storageBoxes;
	static Bag bag;
	static Player player;
	static MapHeader currentMap;
	static Options options;
	static char buffers[NUMBUFFERS][BUFFERLENGTH];
	static NPCData overworldData[];
	static bool doFade;
	static bool doFade2;
	static u32 fadeType;
	static u16* currentPalette;
	static u16* targetPalette;
	static u32 numFrames;
	static u16 blackPalette[];
	static u16 whitePalette[];
	Game();
	~Game();
	static u32 CountPokemon(Pokemon* location, u32 length);
	static u32 CountAbridgedPokemon(AbridgedPokemon* location, u32 length);
	static void DoFade();
	static void SetPalette(u16 *Palette);
	static u16* GetCurrentPalette();
public:
	static void Initialise();
	static void Update();
	static void SetCurrentMap(const MapHeader &mapHeader) { currentMap = mapHeader; }
	static Player & GetPlayer() { return player; }
	static const MapHeader & GetCurrentMap() { return currentMap; }
	static const Options & GetOptions() { return options; }
	static bool AddNewPokemon(const Pokemon &p);
	static u32 CountPartyPokemon();
	static u32 CountBoxPokemon(u32 boxID);
	static u32 CountAllBoxPokemon();
	static void StartTimer(int timerNum, int timerSetting = 0, u16 cascadeValue = 0);
	static bool AddNPC(NonPlayerCharacter* npc);
	static void FadeToPalette(const u16* newPalette, u32 FrameCount);
	static void FadeToGreyScale(u32 FrameCount);
	static void FadeToBlack(u32 FrameCount) { FadeToPalette((const u16*)&blackPalette, FrameCount); }
	static void FadeToWhite(u32 FrameCount) { FadeToPalette((const u16*)&whitePalette, FrameCount); }
	static u16* GetGreyScale(const u16* original);
};

#endif /* GAME_H_ */
