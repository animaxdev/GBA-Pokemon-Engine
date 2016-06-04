/*
 * SceneManager.cpp
 *
 *  Created on: 13 May 2015
 *      Author: Jamie
 */

#include "GlobalDefinitions.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Scene.h"

namespace Scenes
{
	EWRAM_LOCATION ALIGN(4) Scene* SceneManager::scene = NULL;
	EWRAM_LOCATION ALIGN(4) Scene* SceneManager::newScene = NULL;

	SceneManager::SceneManager()
	{
		// TODO Auto-generated constructor stub

	}

	SceneManager::~SceneManager()
	{
		// TODO Auto-generated destructor stub
	}

	void SceneManager::SetScene(Scene* newscene)
	{
		if (newscene)
		{
			newScene = newscene;
		}
	}

	void SceneManager::Update()
	{
		if (newScene)
		{
			if (scene)
			{
				delete scene;
			}
			scene = newScene;
			newScene = NULL;
		}
		if (scene)
		{
			scene->Update();
		}
	}

	void SceneManager::RunExitCallback()
	{
		if (scene)
		{
			scene->OnExit();
		}
	}

	void SceneManager::RunEnterCallback()
	{
		if (scene)
		{
			scene->OnEnter();
		}
	}
}