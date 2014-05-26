#ifndef _LAYER_H__
#define __LAYER_H__

#include <stddef.h>
#include "menu.h"
#include "fields.h"
#include "layercontent.h"

extern field goToLeftLayerTime, goToRightLayerTime, goToEdit;
extern field changeHour, changeMinute, changeSecond, safeChange, discardChange;
extern field goToLeftLayerSettings, goToRightLayerSettings;

layer SettingsLayer = {
	1, updateLayerSettings, {&goToLeftLayerSettings, &goToRightLayerSettings}
};

layer ShowTimeLayer = {
	2, updateLayerShowTime, {&goToLeftLayerTime, &goToRightLayerTime, &goToEdit}
};

layer ChangeTimeLayer = {
	3, updateLayerEditTime, {&changeHour, &changeMinute, &changeSecond, &safeChange, &discardChange}
};


#endif
