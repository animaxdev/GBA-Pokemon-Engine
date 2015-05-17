
#include "GlobalDefinitions.h"
#include "GBPSoundsEngine.h"
#include "SoundEngine.h"
#include "EntityManager.h"
#include "CallbackManager.h"
#include "IRQHandler.h"
#include "InputHandler.h"
#include "GameModeManager.h"
#include "Game.h"
#include "BackgroundFunctions.h"
#include "MemoryManagement.h"
#include "Pokemon.h"
#include "Maths.h"
#include "Mapping.h"
#include "RTC.h"
#include "TitleScreen.h"
#include "Allocator.h"

int Image$$ZI$$Limit = 0x02020000; // beginning of free exRAM

#define LATIN 0
#define CYRILLIC 1
#define JAPANESE 2
#define ARABIC 3
#define BRAILLE 4

#define M4A 0
#define GBP 1

#define TEXTSET LATIN
#define MUSICENGINE GBP

int main()
{
	Game::StartTimer(2);
	Game::StartTimer(3, 1);
	SoundEngine::Initialise(new GBPSoundsEngine());
	EntityManager::Initialise();
	//InitialiseTextEngine(TEXTSET);
	RTC::Enable();
	IRQHandler::Initialise();
	IRQHandler::PrimeIRQ(II_VBLANK);
	BackgroundFunctions::SetBackgroundsToDefault();
	GameModeManager::SetScreen(new TitleScreen());
	Allocator::Initialise((void*)0x06010000, 0x8000);
	Game::SetCurrentMap(Overworld::GetMapHeaderFromBankAndMapID(3, 0));
	while (true)
	{
		VBlankIntrWait();
		RTC::Update();
		InputHandler::KeyPoll();
		CallbackManager::Update();
		SoundEngine::Update();
		EntityManager::Update();
		EntityManager::Render();
		Game::Update();
		Maths::GetRandom32BitValue();
		GameModeManager::Update();
	}
	return 0;
}
