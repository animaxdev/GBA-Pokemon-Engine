/*
 * Game.cpp
 *
 *  Created on: 14 May 2015
 *      Author: Jamie
 */

#include "Core/Game.h"
#include "Core/Pokemon/Pokemon.h"
#include "Entities/NonPlayerCharacter.h"
#include "Core/Maths.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Scene.h"
#include <tonc.h>
#include "Core/FlashFunctions.h"
#include "Scenes/Overworld/PrimaryOverworld.h"
#include "Core/Data/Items.h"

#define GEN123STYLEPOISON

using namespace Core::Data;
using namespace Entities;
using namespace Scenes;

namespace Core
{
	EWRAM_LOCATION ALIGN(4) Core::Pokemon::Pokemon Game::partyPokemon[PartyLength];
	EWRAM_LOCATION ALIGN(4) PokemonStorageBoxes Game::storageBoxes;
	EWRAM_LOCATION ALIGN(4) Core::Pokemon::Pokemon Game::dayCarePokemon[3];
	EWRAM_LOCATION ALIGN(2) u16 Game::dayCareStatus = 0;
	EWRAM_LOCATION ALIGN(4) Core::Pokemon::Pokemon Game::temporaryHoldingPokemon;
	EWRAM_LOCATION ALIGN(4) Bag Game::bag;
	EWRAM_LOCATION ALIGN(4) Bag Game::PCItemStorage;
	EWRAM_LOCATION ALIGN(4) Player Game::player;
	EWRAM_LOCATION ALIGN(4) MapHeader Game::currentMap;
	EWRAM_LOCATION ALIGN(4) OptionsStruct Game::options;
	EWRAM_LOCATION ALIGN(4) NPCData Game::overworldData[NumberOfOverworlds];
	EWRAM_LOCATION ALIGN(2) u16 Game::eggCycle = EggCycleLength;
	EWRAM_LOCATION ALIGN(1) u8 Game::happinessCycle = HappinessCycleLength;
	EWRAM_LOCATION ALIGN(1) u8 Game::poisonCycle = PoisonCycleLength;
	EWRAM_LOCATION ALIGN(1) u8 Game::menuPosition = 0;
	EWRAM_LOCATION ALIGN(2) u16 Game::repelCounter = 0;
	EWRAM_LOCATION ALIGN(2) u16 Game::repelStrength = 0;
	EWRAM_LOCATION ALIGN(1) char Game::buffers[NUMBUFFERS][BUFFERLENGTH];
	EWRAM_LOCATION ALIGN(1) bool Game::inMainGame = false;
	EWRAM_LOCATION ALIGN(1) bool Game::cameraUpdate = false;
	EWRAM_LOCATION ALIGN(1) bool Game::layer0Included = false;
	EWRAM_LOCATION ALIGN(1) u8 Game::validGameSave = 0;
	EWRAM_LOCATION ALIGN(1) u8 Game::soundEngineID = GBPSoundsEngineID;
	EWRAM_LOCATION ALIGN(4) Vector2D Game::cameraPos = Vector2D(0, 8);
	EWRAM_LOCATION ALIGN(4) HealingPlace Game::currentHealingPlace;
	EWRAM_LOCATION ALIGN(1) u8 Game::layer0ID;
	EWRAM_LOCATION ALIGN(1) u8 Game::layer1ID;
	EWRAM_LOCATION ALIGN(1) u8 Game::layer2ID;
	EWRAM_LOCATION ALIGN(1) u8 Game::layer3ID;
	EWRAM_LOCATION ALIGN(4) Collections::Queues::LinkedQueue<Callbacks::Callback*> Game::actions = Collections::Queues::LinkedQueue<Callbacks::Callback*>();

	#define SizeOfPokemonBoxStructure sizeof(PokemonStorageBoxes)
	#define BaseBlocks (SizeOfPokemonBoxStructure / SaveBlockMaxLength)
	#define LengthOfLastStorageBoxBlock (SizeOfPokemonBoxStructure - (BaseBlocks * SaveBlockMaxLength))
	#define BaseSaveAddress ((0x1000 * BaseBlocks) + LengthOfLastStorageBoxBlock)

	RODATA_LOCATION ALIGN(4) SaveLocationStruct Game::saveData[] = {
			{ (u8*)0x0000, (u8*)&Game::storageBoxes, SaveBlockMaxLength },
			{ (u8*)0x1000, (u8*)((u32)&Game::storageBoxes + SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0x2000, (u8*)((u32)&Game::storageBoxes + 2 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0x3000, (u8*)((u32)&Game::storageBoxes + 3 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0x4000, (u8*)((u32)&Game::storageBoxes + 4 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0x5000, (u8*)((u32)&Game::storageBoxes + 5 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0x6000, (u8*)((u32)&Game::storageBoxes + 6 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0x7000, (u8*)((u32)&Game::storageBoxes + 7 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0x8000, (u8*)((u32)&Game::storageBoxes + 8 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0x9000, (u8*)((u32)&Game::storageBoxes + 9 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0xA000, (u8*)((u32)&Game::storageBoxes + 10 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0xB000, (u8*)((u32)&Game::storageBoxes + 11 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0xC000, (u8*)((u32)&Game::storageBoxes + 12 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0xD000, (u8*)((u32)&Game::storageBoxes + 13 * SaveBlockMaxLength), SaveBlockMaxLength },
			{ (u8*)0xE000, (u8*)((u32)&Game::storageBoxes + 14 * SaveBlockMaxLength), LengthOfLastStorageBoxBlock },
			{ (u8*)BaseSaveAddress, (u8*)&Game::partyPokemon, sizeof(Core::Pokemon::Pokemon) * PartyLength },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength), (u8*)&Game::player, sizeof(Player) },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player)), (u8*)&Game::currentMap.mapLocation, sizeof(MapBankMapCombo) },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player) + sizeof(MapBankMapCombo)), (u8*)&Game::options, sizeof(Options) },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player) + sizeof(MapBankMapCombo) + sizeof(Options)), (u8*)&Game::overworldData, sizeof(NPCData) * NumberOfOverworlds },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player) + sizeof(MapBankMapCombo) + sizeof(Options) + (sizeof(NPCData) * NumberOfOverworlds)), (u8*)&Game::validGameSave, 1 },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player) + sizeof(MapBankMapCombo) + sizeof(Options) + (sizeof(NPCData) * NumberOfOverworlds) + sizeof(u8)), (u8*)&Game::soundEngineID, 1 },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player) + sizeof(MapBankMapCombo) + sizeof(Options) + (sizeof(NPCData) * NumberOfOverworlds) + sizeof(u8) * 2), (u8*)&Game::eggCycle, 2 },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player) + sizeof(MapBankMapCombo) + sizeof(Options) + (sizeof(NPCData) * NumberOfOverworlds) + sizeof(u8) * 2 + sizeof(u16) * 1), (u8*)&Game::repelCounter, 2 },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player) + sizeof(MapBankMapCombo) + sizeof(Options) + (sizeof(NPCData) * NumberOfOverworlds) + sizeof(u8) * 2 + sizeof(u16) * 2), (u8*)&Game::repelStrength, 2 },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player) + sizeof(MapBankMapCombo) + sizeof(Options) + (sizeof(NPCData) * NumberOfOverworlds) + sizeof(u8) * 2 + sizeof(u16) * 3), (u8*)&Game::happinessCycle, 1 },
			{ (u8*)(BaseSaveAddress + sizeof(Core::Pokemon::Pokemon) * PartyLength + sizeof(Player) + sizeof(MapBankMapCombo) + sizeof(Options) + (sizeof(NPCData) * NumberOfOverworlds) + sizeof(u8) * 2 + sizeof(u16) * 4 + sizeof(u8)), (u8*)&Game::poisonCycle, 1 },
			{ (u8*)(0x1000 * (BaseBlocks + 1)), (u8*)&Game::bag, sizeof(Bag) },
			{ (u8*)0xFFFFFFFF, 0, 0 }
	};

	Game::Game()
	{
	}

	Game::~Game()
	{
	}

	void Game::Initialise()
	{
		if (!options.options.optionsSet)
		{
			options.allOptions = 0;
			options.options.stereoSound = 1;
			options.options.textSpeed = 2;
			options.options.useImperialUnits = 1;
		}
		options.options.optionsSet = 0;
		memset32(&currentHealingPlace, 0, sizeof(HealingPlace) >> 2);
		memset32(&storageBoxes, 0, sizeof(PokemonStorageBoxes) >> 2);
		memset32(&partyPokemon, 0, (sizeof(Core::Pokemon::Pokemon) * 6) >> 2);
		memset32(&bag, 0, sizeof(Bag) >> 2);
		memset32(&player, 0, sizeof(Player) >> 2);
		memset32(&overworldData, 0, (sizeof(NPCData) * NumberOfOverworlds) >> 2);
		soundEngineID = GBPSoundsEngineID;
		eggCycle = EggCycleLength;
		happinessCycle = HappinessCycleLength;
		poisonCycle = PoisonCycleLength;
		repelCounter = 0;
		repelStrength = 0;
		cameraPos = Vector2D(0, 8);
	}

	void Game::OnTakeStep()
	{
		if (eggCycle == 0)
		{
			eggCycle = EggCycleLength;
			OnEggCycleExpire();
		}
		else
		{
			eggCycle--;
		}
		if (happinessCycle == 0)
		{
			happinessCycle = HappinessCycleLength;
			OnHappinessCycleExpire();
		}
		else
		{
			happinessCycle--;
		}
		if (poisonCycle == 0)
		{
			poisonCycle = PoisonCycleLength;
			OnPoisonCycleExpire();
		}
		else
		{
			poisonCycle--;
		}
		if (repelCounter)
		{
			repelCounter--;
			if (!repelCounter)
			{
				// End repel
			}
		}
	}

	void Game::OnEggCycleExpire()
	{
		// Egg cycle expiry code
		bool doubleHatching = false;
		for (int i = 0; i < 6; i++)
		{
			if (partyPokemon[i].Decrypt(PersonalityID))
			{
				u8 ability = partyPokemon[i].Decrypt(Ability);
				if (!partyPokemon[i].Decrypt(IsEgg) && (ability == Magma_Armour || ability == Flame_Body))
				{
					doubleHatching = true;
					break;
				}
			}
		}
		for (int i = 0; i < 6; i++)
		{
			if (partyPokemon[i].Decrypt(PersonalityID) && partyPokemon[i].Decrypt(IsEgg))
			{
				u32 value = partyPokemon[i].Decrypt(Friendship);
				if (doubleHatching && value >= 2)
				{
					value -= 2 ;
					partyPokemon[i].Encrypt(Friendship, value);
				}
				else
				{
					value--;
					partyPokemon[i].Encrypt(Friendship, value);
				}
				if (value == 0)
				{
					// Make note of need to hatch
				}
			}
		}
	}

	void Game::OnHappinessCycleExpire()
	{
		for (int i = 0; i < 6; i++)
		{
			if (partyPokemon[i].Decrypt(PersonalityID) && !partyPokemon[i].Decrypt(IsEgg))
			{
				u32 itemEffect = Items::GetItemEffect(partyPokemon[i].Decrypt(HeldItem));
				u32 value = partyPokemon[i].Decrypt(Friendship);
				if (itemEffect == Item_Effect_Increase_Happiness_Gain)
				{
					value += 1 + Items::GetItemStrength(partyPokemon[i].Decrypt(HeldItem));
				}
				else
				{
					value++;
				}
				if (value > MaxHappinessAllowed)
				{
					value = MaxHappinessAllowed;
				}
				partyPokemon[i].Encrypt(Friendship, value);
			}
		}
	}

	void Game::OnPoisonCycleExpire()
	{
		for (int i = 0; i < 6; i++)
		{
			PrimaryStatusStruct status = partyPokemon[i].GetStatus();
			if (partyPokemon[i].Decrypt(PersonalityID) && (status.badlyPoisoned || status.poisoned))
			{
	#ifdef GEN123STYLEPOISON
				u32 value = partyPokemon[i].Decrypt(CurrentHP);
				if (value > 0)
				{
					value--;
					partyPokemon[i].Encrypt(CurrentHP, value);
					if (value == 0)
					{
						status.badlyPoisoned = 0;
						status.poisoned = 0;
						status.badlyPoisonedCounter = 0;
						// Fainted script
					}
				}
	#else
				u32 value = partyPokemon[i].Decrypt(CurrentHP);
				if (value > 1)
				{
					value--;
					partyPokemon[i].Encrypt(CurrentHP, value);
					if (value == 1)
					{
						status.badlyPoisoned = 0;
						status.poisoned = 0;
						status.badlyPoisonedCounter = 0;
						// Cured of poison script
					}
				}
	#endif
			}
		}
	}

	void Game::Update()
	{
		if (cameraUpdate)
		{
			s16 x = Maths::UnsignedModulus(cameraPos.GetX(), 0x200);
			s16 y = Maths::UnsignedModulus(cameraPos.GetY(), 0x200);
			*BG3HOFS = x;
			*BG3VOFS = y;
			*BG1HOFS = x;
			*BG1VOFS = y;
			*BG2HOFS = x;
			*BG2VOFS = y;
			if (layer0Included)
			{
				*BG0HOFS = x;
				*BG0VOFS = y;
			}
		}
	}

	bool Game::AddNewPokemonToParty(const Core::Pokemon::Pokemon &p)
	{
		for (int i = 0; i < 6; i++)
		{
			if (partyPokemon[i].Decrypt(PersonalityID) == 0)
			{
				partyPokemon[i] = p;
				return true;
			}
		}
		return false;
	}

	bool Game::AddNewPokemon(const Core::Pokemon::Pokemon &p)
	{
		if (!AddNewPokemonToParty(p))
		{
			if (storageBoxes.info.numPokemon == POKEMONPERBOX * NUMBOXES)
			{
				return false;
			}
			for (int i = 0; i < POKEMONPERBOX; i++)
			{
				if (storageBoxes.boxData[storageBoxes.info.currentBoxID][i].Decrypt(PersonalityID) == 0)
				{
					storageBoxes.boxData[storageBoxes.info.currentBoxID][i] = p.GetMainData();
					storageBoxes.info.numPokemon++;
					return true;
				}
			}
			for (int i = 0; i < NUMBOXES; i++)
			{
				if (i == storageBoxes.info.currentBoxID)
				{
					continue;
				}
				for (int j = 0; j < POKEMONPERBOX; j++)
				{
					if (storageBoxes.boxData[i][j].Decrypt(PersonalityID) == 0)
					{
						storageBoxes.boxData[i][j] = p.GetMainData();
						storageBoxes.info.numPokemon++;
						storageBoxes.info.currentBoxID = i;
						return true;
					}
				}
			}
		}
		return false;
	}

	u32 Game::CountPokemon(Core::Pokemon::Pokemon* location, u32 length)
	{
		u32 counter = 0;
		u32 i;
		for (i = 0; i < length; i++)
		{
			if (location[i].Decrypt(PersonalityID) != 0)
			{
				counter++;
			}
		}
		return counter;
	}

	u32 Game::CountAbridgedPokemon(Core::Pokemon::AbridgedPokemon* location, u32 length)
	{
		u32 counter = 0;
		u32 i;
		for (i = 0; i < length; i++)
		{
			if (location[i].Decrypt(PersonalityID) != 0)
			{
				counter++;
			}
		}
		return counter;
	}

	u32 Game::CountPartyPokemon()
	{
		return CountPokemon((Core::Pokemon::Pokemon*)&partyPokemon, 6);
	}

	u32 Game::CountBoxPokemon(u32 boxID)
	{
		u32 address = (u32)&storageBoxes.boxData;
		address += POKEMONPERBOX * boxID * sizeof(Core::Pokemon::AbridgedPokemon);
		return CountAbridgedPokemon((Core::Pokemon::AbridgedPokemon*)address, POKEMONPERBOX);
	}

	u32 Game::CountAllBoxPokemon()
	{
		return storageBoxes.info.numPokemon;
	}

	void Game::StartTimer(int timerNum, int timerSetting, u16 cascadeValue)
	{
		switch(timerNum)
		{
		case 0:
			REG_TM0D = cascadeValue;
			REG_TM0CNT = 0x80 | timerSetting;
			break;
		case 1:
			REG_TM1D = cascadeValue;
			REG_TM1CNT = 0x80 | timerSetting;
			break;
		case 2:
			REG_TM2D = cascadeValue;
			REG_TM2CNT = 0x80 | timerSetting;
			break;
		case 3:
			REG_TM3D = cascadeValue;
			REG_TM3CNT = 0x80 | timerSetting;
			break;
		}
	}

	bool Game::AddNPC(NonPlayerCharacter* npc)
	{
		for (int i = 1; i < NumberOfOverworlds; i++)
		{
			if (npc->GetObjectID() == overworldData[i].dataSpriteID)
			{
				return false;
			}
		}
		for (int i = 0; i < NumberOfOverworlds; i++)
		{
			if (!overworldData[i].isActive)
			{
				overworldData[i].dataPointer = npc;
				overworldData[i].isActive = 1;
				overworldData[i].xLocation = npc->GetPosition().GetX();
				overworldData[i].yLocation = npc->GetPosition().GetY();
				overworldData[i].prevXLocation = npc->GetPosition().GetX();
				overworldData[i].prevYLocation = npc->GetPosition().GetY();
				overworldData[i].dataSpriteID = npc->GetObjectID();
				overworldData[i].isMoving = 0;
				overworldData[i].frameID = 0;
				overworldData[i].frameDelay = 0;
				overworldData[i].isMoving = 0;
				overworldData[i].nextWalkingFrame = 0;
				overworldData[i].oamStructID = 0;
				overworldData[i].pixelsMoved = 0;
				overworldData[i].previousWalkingFrame = 0;
				overworldData[i].scriptLocation = 0;
				overworldData[i].spriteID = npc->GetSpriteIndex();
				return true;
			}
		}
		return false;
	}

	void Game::OverwriteNPC(NonPlayerCharacter* npc, u32 position)
	{
		if (position < NumberOfOverworlds)
		{
			overworldData[position].dataPointer = npc;
			overworldData[position].isActive = 1;
			overworldData[position].xLocation = npc->GetPosition().GetX();
			overworldData[position].yLocation = npc->GetPosition().GetY();
			overworldData[position].prevXLocation = npc->GetPosition().GetX();
			overworldData[position].prevYLocation = npc->GetPosition().GetY();
			overworldData[position].dataSpriteID = npc->GetObjectID();
			overworldData[position].isMoving = 0;
			overworldData[position].frameID = 0;
			overworldData[position].frameDelay = 0;
			overworldData[position].isMoving = 0;
			overworldData[position].nextWalkingFrame = 0;
			overworldData[position].oamStructID = 0;
			overworldData[position].pixelsMoved = 0;
			overworldData[position].previousWalkingFrame = 0;
			overworldData[position].scriptLocation = 0;
			overworldData[position].spriteID = npc->GetSpriteIndex();
		}
	}

	void Game::Save()
	{
		validGameSave = true;
		FlashFunctions::WriteToFlash((SaveLocationStruct*)&saveData);
	}

	void Game::Load()
	{
		FlashFunctions::ReadFromFlash((SaveLocationStruct*)&saveData);
		if (validGameSave != 1)
		{
			Initialise();
		}
	}

	void Game::MoveCamera(const Vector2D &delta)
	{
		cameraPos += delta;
	}

	void Game::SetCamera(const Vector2D &position)
	{
		cameraPos = position;
	}

	bool Game::AddItemsToBagLocation(u16 itemID, u16 numberOfItems, bool doIt, Bag &bagLoc)
	{
		u32 category = Items::GetItemCategory(itemID);
		u32 slotID = U32Max;
		switch (category)
		{
			case Item_Category_Items:
				for (u32 i = 0; i < NumBagItems; i++)
				{
					if ((bagLoc.bagItemsData[i].itemID == itemID && bagLoc.bagItemsData[i].quantity + numberOfItems <= 0xFFFF) || bagLoc.bagItemsData[i].itemID == 0)
					{
						slotID = i;
						break;
					}
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.bagItemsData[slotID].quantity += numberOfItems;
						bagLoc.bagItemsData[slotID].itemID = itemID;
					}
				}
				break;
			case Item_Category_Balls:
				for (u32 i = 0; i < NumBallTypes; i++)
				{
					if ((bagLoc.ballItemsData[i].itemID == itemID && bagLoc.ballItemsData[i].quantity + numberOfItems <= 0xFFFF) || bagLoc.ballItemsData[i].itemID == 0)
					{
						slotID = i;
						break;
					}
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.ballItemsData[slotID].quantity += numberOfItems;
						bagLoc.ballItemsData[slotID].itemID = itemID;
					}
				}
				break;
			case Item_Category_KeyItems:
				for (u32 i = 0; i < NumKeyItems; i++)
				{
					if ((bagLoc.keyItemsData[i].itemID == itemID && bagLoc.keyItemsData[i].quantity + numberOfItems <= 0xFFFF) || bagLoc.keyItemsData[i].itemID == 0)
					{
						slotID = i;
						break;
					}
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.keyItemsData[slotID].quantity += numberOfItems;
						bagLoc.keyItemsData[slotID].itemID = itemID;
					}
				}
				break;
			case Item_Category_Berries:
				for (u32 i = 0; i < NumBerryTypes; i++)
				{
					if ((bagLoc.berryItemsData[i].itemID == itemID && bagLoc.berryItemsData[i].quantity + numberOfItems <= 0xFFFF) || bagLoc.berryItemsData[i].itemID == 0)
					{
						slotID = i;
						break;
					}
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.berryItemsData[slotID].quantity += numberOfItems;
						bagLoc.berryItemsData[slotID].itemID = itemID;
					}
				}
				break;
			case Item_Category_TMsAndHMs:
				if (bagLoc.TMData[itemID].quantity + numberOfItems <= 0xFFFF)
				{
					slotID = itemID;
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.bagItemsData[slotID].quantity += numberOfItems;
						bagLoc.bagItemsData[slotID].itemID = itemID;
					}
				}
				break;
		}
		return slotID != U32Max;
	}

	bool Game::FindItemsInBagLocation(u16 itemID, u16 numberOfItems, const Bag &bagLoc)
	{
		u32 category = Items::GetItemCategory(itemID);
		u32 slotID = U32Max;
		switch (category)
		{
			case Item_Category_Items:
				for (u32 i = 0; i < NumBagItems; i++)
				{
					if (bagLoc.bagItemsData[i].itemID == itemID && bagLoc.bagItemsData[i].quantity >= numberOfItems)
					{
						slotID = i;
						break;
					}
				}
				break;
			case Item_Category_Balls:
				for (u32 i = 0; i < NumBallTypes; i++)
				{
					if (bagLoc.ballItemsData[i].itemID == itemID && bagLoc.ballItemsData[i].quantity >= numberOfItems)
					{
						slotID = i;
						break;
					}
				}
				break;
			case Item_Category_KeyItems:
				for (u32 i = 0; i < NumKeyItems; i++)
				{
					if (bagLoc.keyItemsData[i].itemID == itemID && bagLoc.keyItemsData[i].quantity >= numberOfItems)
					{
						slotID = i;
						break;
					}
				}
				break;
			case Item_Category_Berries:
				for (u32 i = 0; i < NumBerryTypes; i++)
				{
					if (bagLoc.berryItemsData[i].itemID == itemID && bagLoc.berryItemsData[i].quantity >= numberOfItems)
					{
						slotID = i;
						break;
					}
				}
				break;
			case Item_Category_TMsAndHMs:
				if (bagLoc.TMData[itemID].quantity >= numberOfItems)
				{
					slotID = Items::GetItemIndexNumber(itemID);
				}
				break;
		}
		return slotID != U32Max;
	}


	bool Game::RemoveItemsFromBagLocation(u16 itemID, u16 numberOfItems, bool doIt, Bag &bagLoc)
	{
		u32 category = Items::GetItemCategory(itemID);
		u32 slotID = U32Max;
		switch (category)
		{
			case Item_Category_Items:
				for (u32 i = 0; i < NumBagItems; i++)
				{
					if (bagLoc.bagItemsData[i].itemID == itemID && bagLoc.bagItemsData[i].quantity - numberOfItems >= 0)
					{
						slotID = i;
						break;
					}
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.bagItemsData[slotID].quantity -= numberOfItems;
						if (!bagLoc.bagItemsData[slotID].quantity)
						{
							bagLoc.bagItemsData[slotID].itemID = 0;
						}
					}
				}
				break;
			case Item_Category_Balls:
				for (u32 i = 0; i < NumBallTypes; i++)
				{
					if (bagLoc.ballItemsData[i].itemID == itemID && bagLoc.ballItemsData[i].quantity - numberOfItems >= 0)
					{
						slotID = i;
						break;
					}
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.ballItemsData[slotID].quantity -= numberOfItems;
						if (!bagLoc.ballItemsData[slotID].quantity)
						{
							bagLoc.ballItemsData[slotID].itemID = 0;
						}
					}
				}
				break;
			case Item_Category_KeyItems:
				for (u32 i = 0; i < NumKeyItems; i++)
				{
					if (bagLoc.keyItemsData[i].itemID == itemID && bagLoc.keyItemsData[i].quantity - numberOfItems >= 0)
					{
						slotID = i;
						break;
					}
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.keyItemsData[slotID].quantity -= numberOfItems;
						if (!bagLoc.keyItemsData[slotID].quantity)
						{
							bagLoc.keyItemsData[slotID].itemID = 0;
						}
					}
				}
				break;
			case Item_Category_Berries:
				for (u32 i = 0; i < NumBerryTypes; i++)
				{
					if (bagLoc.berryItemsData[i].itemID == itemID && bagLoc.berryItemsData[i].quantity - numberOfItems >= 0)
					{
						slotID = i;
						break;
					}
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.berryItemsData[slotID].quantity -= numberOfItems;
						if (!bagLoc.keyItemsData[slotID].quantity)
						{
							bagLoc.berryItemsData[slotID].itemID = 0;
						}
					}
				}
				break;
			case Item_Category_TMsAndHMs:
				if (bagLoc.TMData[itemID].quantity - numberOfItems >= 0)
				{
					slotID = Items::GetItemIndexNumber(itemID);
				}
				if (slotID != U32Max)
				{
					if (doIt)
					{
						bagLoc.bagItemsData[slotID].quantity -= numberOfItems;
					}
				}
				break;
		}
		return slotID != U32Max;
	}

	bool Game::RemoveItemsFromBag(u16 itemID, u16 numberOfItems, bool doIt)
	{
		return RemoveItemsFromBagLocation(itemID, numberOfItems, doIt);
	}

	bool Game::AddItemsToBag(u16 itemID, u16 numberOfItems, bool doIt)
	{
		return AddItemsToBagLocation(itemID, numberOfItems, doIt);
	}

	bool Game::FindItemsInBag(u16 itemID, u16 numberOfItems)
	{
		return FindItemsInBagLocation(itemID, numberOfItems);
	}

	bool Game::RemoveItemsFromPC(u16 itemID, u16 numberOfItems, bool doIt)
	{
		return RemoveItemsFromBagLocation(itemID, numberOfItems, doIt, PCItemStorage);
	}

	bool Game::AddItemsToPC(u16 itemID, u16 numberOfItems, bool doIt)
	{
		return AddItemsToBagLocation(itemID, numberOfItems, doIt, PCItemStorage);
	}

	bool Game::FindItemsInPC(u16 itemID, u16 numberOfItems)
	{
		return FindItemsInBagLocation(itemID, numberOfItems, PCItemStorage);
	}

	bool Game::GivePlayerMoney(u32 cashAwarded)
	{
		if (player.balance + cashAwarded > MaxPlayerCash)
		{
			player.balance = MaxPlayerCash;
			return false;
		}
		player.balance += cashAwarded;
		return true;
	}

	bool Game::RemovePlayerMoney(u32 cashRemoved)
	{
		if (player.balance < cashRemoved)
		{
			return false;
		}
		player.balance -= cashRemoved;
		return true;
	}

	bool Game::GivePlayerMumMoney(u32 cashAwarded)
	{
		if (player.mumBalance + cashAwarded > MaxPlayerCash)
		{
			player.mumBalance = MaxPlayerCash;
			return false;
		}
		player.mumBalance += cashAwarded;
		return true;
	}

	bool Game::RemovePlayerMumMoney(u32 cashRemoved)
	{
		if (player.mumBalance < cashRemoved)
		{
			return false;
		}
		player.mumBalance -= cashRemoved;
		return true;
	}

	void Game::ClearParty()
	{
		memset32((void*)&partyPokemon, 0, (sizeof(Core::Pokemon::Pokemon) * 6) >> 2);
	}
}