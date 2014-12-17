#include "layers.h"

/*******************************************************************************
* Beschreibung:
*	Hier sind alle Ebenen definiert. Hier werden alle Felder
*	und eine update Method um die GUI zu erstellen uf die Ebenen gemappt
*******************************************************************************/

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
