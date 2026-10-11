#pragma once
#include "globaldefs.h"

struct ScriptObjectId {
    int id_;
};


int StartFunction(void* startParam);
void initializeScriptObjectEnable();
void setScriptObjectEnable(int index, bool flag);
int isScriptObjectEnable(int index);
int getObjectCount();
void initializeScriptObjectCtrl();
void setScriptObjectCtrl(int ctrl, int index);
int getScriptObjectCtrl(int index);
int getScriptObjectIndex(int ctrl);
void setPlacementCtrlId(int ctrl);
int getPlacementCtrlId();
int getPlacementCtrlId(int index);
int getPlacementIndex(int ctrl);
