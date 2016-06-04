/*
 * PlayerEntity.cpp
 *
 *  Created on: 16 May 2015
 *      Author: Jamie
 */

#include "Entities/PlayerEntity.h"
#include "GlobalDefinitions.h"

namespace Entities
{
	PlayerEntity::PlayerEntity(const Vector2D &location, u32 backgroundLevel, bool gender, u16* colourAddress) : NonPlayerCharacter(location, backgroundLevel, (gender) ? 30 : 0, 0xFF, colourAddress)
	{
		// TODO Auto-generated constructor stub
		_object->SetPosition(Vector2D(112, 56));
	}

	PlayerEntity::~PlayerEntity()
	{
		// TODO Auto-generated destructor stub
	}

	bool PlayerEntity::Initialise()
	{

	}

	bool PlayerEntity::LoadContent()
	{

	}

	bool PlayerEntity::Update()
	{
		return NonPlayerCharacter::Update();
	}

	void PlayerEntity::UnloadContent()
	{

	}
}