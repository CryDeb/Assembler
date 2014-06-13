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
	1, updateLayerSendFrame, {&goToLeftLayerSettings, &goToRightLayerSettings}
};


#endif
