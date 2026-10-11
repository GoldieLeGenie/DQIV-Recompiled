#include "main/Commands/CommonCommand.hpp"
#include "main/CommandParameter/CommandParameter.hpp"
#include "main/script/ScriptBaseCommand.hpp"
#include "main/script/sys/PlacementParameter.hpp"
#include "ov000/town/TownCharacterManager.hpp"

ARM int ScriptCommand::exec(CommandParameter* command)
{
    if (command->flag_ == 0) {
        execute();
    }
    if (command->flag_ & 1) {
        if (!(command->flag_ & 0x10)) {
            command->flag_ |= 0x10;
            initialize((char*)command->param_);
        }
        if (!(command->flag_ & 0x40)) {
            execute();
            if (isEnd()) {
                command->flag_ |= 0x40;
                terminate();
            }
        }
    }
    return (command->flag_ & 0x40) != 0;
}

static int ctrlId_;
static int objectCount;
static int scriptObjectCtrl[48];
static int scriptObjectEnable[48];

ARM int StartFunction(void* startParam)
{
    Param param = *(Param*)startParam;
    TOWN_CHARACTER chara;
    chara.flag.flag_ = 0;
    chara.index = param.ctrlId_;
    chara.charaIndex = param.index_;
    chara.dir = param.dir_ << 14;
    chara.position.vx.value = param.x_;
    chara.position.vy.value = param.y_;
    chara.position.vz.value = param.z_;
    int ctrl = TownCharacterManager::getSingleton()->setup(chara);
    return ctrl;
}

ARM void initializeScriptObjectEnable()
{
    for (int i = 0; i < 48; i++) {
        scriptObjectEnable[i] = 0;
    }
}

ARM void setScriptObjectEnable(int index, bool flag)
{
    scriptObjectEnable[index] = flag;
}

ARM int isScriptObjectEnable(int index)
{
    return scriptObjectEnable[index];
}

ARM int getObjectCount()
{
    return objectCount;
}

ARM void initializeScriptObjectCtrl()
{
    for (int i = 0; i < 48; i++) {
        scriptObjectCtrl[i] = 0;
    }
    objectCount = 0;
}

ARM void setScriptObjectCtrl(int ctrl, int index)
{
    scriptObjectCtrl[ctrl] = index;
    objectCount++;
}

ARM int getScriptObjectCtrl(int index)
{
    for (int i = 0; i < 48; i++) {
        if (scriptObjectCtrl[i] == index) {
            return i;
        }
    }
    return -1;
}

ARM int getScriptObjectIndex(int ctrl)
{
    return scriptObjectCtrl[ctrl];
}

ARM void setPlacementCtrlId(int ctrl)
{
    ctrlId_ = ctrl;
}

ARM int getPlacementCtrlId()
{
    return ctrlId_;
}

ARM int getPlacementCtrlId(int index)
{
    return getScriptObjectCtrl(index);
}

ARM int getPlacementIndex(int ctrl)
{
    return getScriptObjectIndex(ctrl);
}
