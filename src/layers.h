#ifndef _LAYER_H__
#define __LAYER_H__

#include <stddef.h>
#include "menu.h"
#include "fields.h"
#include "layercontent.h"

extern field editText, gotToLeftLayer, goToRightLayer;

layer editTextLayer = {
    1, updateLayerEditText, {&editText, &goToLeftLayer, &goToRightLayer}
};
#endif
