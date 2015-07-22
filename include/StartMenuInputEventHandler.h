/*
 * StartMenuInputEventHandler.h
 *
 *  Created on: 17 Jul 2015
 *      Author: Jamie
 */

#ifndef STARTMENUINPUTEVENTHANDLER_H_
#define STARTMENUINPUTEVENTHANDLER_H_

#include "InputEventHandler.h"
#include "GlobalDefinitions.h"

#define NumOptions 5
#define NumAdditionalOptions 3

class StartMenuInputEventHandler : public InputEventHandler
{
private:
	static StringAndFunctionPointerStruct baseMenuOptions[];
	static StringAndFunctionPointerWithFlagIDStruct additionalMenuOptions[NumAdditionalOptions];
	VoidFunctionPointerVoid* menu;
	u32 menuPosition;
	u32 numMenuItems;
public:
	StartMenuInputEventHandler();
	~StartMenuInputEventHandler();
	void OnPressA();
	void OnPressB();
	void OnPressStart();
	void OnPressUp();
	void OnPressDown();
};

#endif /* STARTMENUINPUTEVENTHANDLER_H_ */
