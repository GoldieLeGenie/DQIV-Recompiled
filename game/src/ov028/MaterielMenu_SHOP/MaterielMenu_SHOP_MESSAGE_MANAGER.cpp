#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"

THUMB MaterielMenu_SHOP_MESSAGE_MANAGER* MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()
{
    static MaterielMenu_SHOP_MESSAGE_MANAGER shopMessage;
    return &shopMessage;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::idle()
{
    switch (MaterielMenu_SHOP_MANAGER::getSingleton()->getShopType()) {
    case 8:
        shopType_ = 2;
        break;
    case 9:
        shopType_ = 3;
        break;
    case 10:
        shopType_ = 4;
        break;
    default:
        shopType_ = MaterielMenu_SHOP_MANAGER::getSingleton()->getShopType();
        break;
    }
    if (shopType_ == 2) {
        return 0xc5ffa;
    }
    if (shopType_ == 3) {
        return 0xc63e2;
    }
    if (shopType_ == 4) {
        return 0xc5c12;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::buy()
{
    if (shopType_ == 2) {
        return 0xc5ffe;
    }
    if (shopType_ == 3) {
        return 0xc63e6;
    }
    if (shopType_ == 4) {
        return 0xc5c16;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::sell()
{
    if (shopType_ == 2) {
        return 0xc6038;
    }
    if (shopType_ == 3) {
        return 0xc6420;
    }
    if (shopType_ == 4) {
        return 0xc5c50;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::yameru()
{
    if (shopType_ == 2) {
        return 0xc6060;
    }
    if (shopType_ == 3) {
        return 0xc6449;
    }
    if (shopType_ == 4) {
        return 0xc5c78;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::cancel()
{
    if (shopType_ == 2) {
        return 0xc605d;
    }
    if (shopType_ == 3) {
        return 0xc6446;
    }
    if (shopType_ == 4) {
        return 0xc5c75;
    }
    return 0;
}

THUMB void MaterielMenu_SHOP_MESSAGE_MANAGER::noMoney(int* mes)
{
    if (shopType_ == 2) {
        mes[0] = 0xc6002;
        mes[1] = 0xc605d;
    }
    if (shopType_ == 3) {
        mes[0] = 0xc63ea;
        mes[1] = 0xc6446;
    }
    if (shopType_ == 4) {
        mes[0] = 0xc5c1a;
        mes[1] = 0xc5c75;
    }
}

THUMB void MaterielMenu_SHOP_MESSAGE_MANAGER::buyItem(bool plural, bool battleUse, int* mes)
{
    if (plural && battleUse) {
        if (shopType_ == 2) {
            mes[0] = 0xc6008;
            mes[1] = 0xc600b;
            mes[2] = 0xc600c;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc63f0;
            mes[1] = 0xc63f3;
            mes[2] = 0xc63f4;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c20;
            mes[1] = 0xc5c23;
            mes[2] = 0xc5c24;
        }
    } else if (battleUse) {
        if (shopType_ == 2) {
            mes[0] = 0xc6005;
            mes[1] = 0xc600b;
            mes[2] = 0xc600c;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc63ed;
            mes[1] = 0xc63f3;
            mes[2] = 0xc63f4;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c1d;
            mes[1] = 0xc5c23;
            mes[2] = 0xc5c24;
        }
    } else if (plural) {
        if (shopType_ == 2) {
            mes[0] = 0xc6008;
            mes[1] = 0xc600c;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc63f0;
            mes[1] = 0xc63f4;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c20;
            mes[1] = 0xc5c24;
        }
    } else {
        if (shopType_ == 2) {
            mes[0] = 0xc6005;
            mes[1] = 0xc600c;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc63ed;
            mes[1] = 0xc63f4;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c1d;
            mes[1] = 0xc5c24;
        }
    }
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::haveItemMax()
{
    if (shopType_ == 2) {
        return 0xc600f;
    }
    if (shopType_ == 3) {
        return 0xc63f7;
    }
    if (shopType_ == 4) {
        return 0xc5c27;
    }
    return 0;
}

THUMB void MaterielMenu_SHOP_MESSAGE_MANAGER::buyToSack(bool haveMoney, int* mes)
{
    if (haveMoney) {
        if (shopType_ == 2) {
            mes[0] = 0xc6019;
            mes[1] = 0xc6035;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc6401;
            mes[1] = 0xc641d;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c31;
            mes[1] = 0xc5c4d;
        }
    } else {
        if (shopType_ == 2) {
            mes[0] = 0xc6019;
            mes[1] = 0xc605d;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc6401;
            mes[1] = 0xc6446;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c31;
            mes[1] = 0xc5c75;
        }
    }
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::getItem(bool isBasha, bool isDeath)
{
    if (isBasha) {
        if (shopType_ == 2) {
            return 0xc602f;
        }
        if (shopType_ == 3) {
            return 0xc6417;
        }
        if (shopType_ == 4) {
            return 0xc5c47;
        }
    } else if (isDeath) {
        if (shopType_ == 2) {
            return 0xc602c;
        }
        if (shopType_ == 3) {
            return 0xc6414;
        }
        if (shopType_ == 4) {
            return 0xc5c44;
        }
    } else {
        if (shopType_ == 2) {
            return 0xc6029;
        }
        if (shopType_ == 3) {
            return 0xc6411;
        }
        if (shopType_ == 4) {
            return 0xc5c41;
        }
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::checkMoney(int overItem, bool haveNoMoney, int* mes)
{
    int mesCount;
    if (overItem && haveNoMoney) {
        if (shopType_ == 2) {
            mes[0] = 0xc6032;
            mes[1] = 0xc605d;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc641a;
            mes[1] = 0xc6446;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c4a;
            mes[1] = 0xc5c75;
        }
        mesCount = 2;
    } else if (overItem) {
        if (shopType_ == 2) {
            mes[0] = 0xc6032;
            mes[1] = 0xc6035;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc641a;
            mes[1] = 0xc641d;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c4a;
            mes[1] = 0xc5c4d;
        }
        mesCount = 2;
    } else if (haveNoMoney) {
        if (shopType_ == 2) {
            mes[0] = 0xc605d;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc6446;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c75;
        }
        mesCount = 1;
    } else {
        if (shopType_ == 2) {
            mes[0] = 0xc6035;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc641d;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c4d;
        }
        mesCount = 1;
    }
    return mesCount;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::haveOther()
{
    if (shopType_ == 2) {
        return 0xc6016;
    }
    if (shopType_ == 3) {
        return 0xc63fe;
    }
    if (shopType_ == 4) {
        return 0xc5c2e;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::haveWhose()
{
    if (shopType_ == 2) {
        return 0xc600c;
    }
    if (shopType_ == 3) {
        return 0xc63f4;
    }
    if (shopType_ == 4) {
        return 0xc5c24;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::haveSomething()
{
    if (shopType_ == 2) {
        return 0xc6015;
    }
    if (shopType_ == 3) {
        return 0xc63fd;
    }
    if (shopType_ == 4) {
        return 0xc5c2d;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::sortEnd()
{
    if (shopType_ == 2) {
        return 0xc6012;
    }
    if (shopType_ == 3) {
        return 0xc63fa;
    }
    if (shopType_ == 4) {
        return 0xc5c2a;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::checkEquip(bool equip)
{
    if (equip) {
        if (shopType_ == 2) {
            return 0xc601f;
        }
        if (shopType_ == 3) {
            return 0xc6407;
        }
        if (shopType_ == 4) {
            return 0xc5c37;
        }
    } else {
        if (shopType_ == 2) {
            return 0xc601c;
        }
        if (shopType_ == 3) {
            return 0xc6404;
        }
        if (shopType_ == 4) {
            return 0xc5c34;
        }
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::equipCurseItem(bool first)
{
    if (first) {
        if (shopType_ == 2) {
            return 0xc6025;
        }
        if (shopType_ == 3) {
            return 0xc640d;
        }
        if (shopType_ == 4) {
            return 0xc5c3d;
        }
    } else {
        if (shopType_ == 2) {
            return 0xc6026;
        }
        if (shopType_ == 3) {
            return 0xc640e;
        }
        if (shopType_ == 4) {
            return 0xc5c3e;
        }
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::equipItem()
{
    if (shopType_ == 2) {
        return 0xc6022;
    }
    if (shopType_ == 3) {
        return 0xc640a;
    }
    if (shopType_ == 4) {
        return 0xc5c3a;
    }
    return 0;
}

THUMB void MaterielMenu_SHOP_MESSAGE_MANAGER::haveNoItem(bool fukuro, int* mes)
{
    if (fukuro) {
        if (shopType_ == 2) {
            mes[0] = 0xc6041;
            mes[1] = 0xc605d;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc6429;
            mes[1] = 0xc6446;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c59;
            mes[1] = 0xc5c75;
        }
    } else {
        if (shopType_ == 2) {
            mes[0] = 0xc603e;
            mes[1] = 0xc605d;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc6426;
            mes[1] = 0xc6446;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c56;
            mes[1] = 0xc5c75;
        }
    }
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::sellOK()
{
    if (shopType_ == 2) {
        return 0xc5c7d;
    }
    if (shopType_ == 3) {
        return 0xc5c7d;
    }
    if (shopType_ == 4) {
        return 0xc5c7d;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::sellHowMany()
{
    if (shopType_ == 2) {
        return 0xc6064;
    }
    if (shopType_ == 3) {
        return 0xc644d;
    }
    if (shopType_ == 4) {
        return 0xc5c7a;
    }
    return 0;
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::sellPluralSellOK()
{
    if (shopType_ == 2) {
        return 0xc6067;
    }
    if (shopType_ == 3) {
        return 0xc6450;
    }
    if (shopType_ == 4) {
        return 0xc5c7d;
    }
    return 0;
}

THUMB void MaterielMenu_SHOP_MESSAGE_MANAGER::sellNG(bool sellYet, int* mes)
{
    if (sellYet) {
        if (shopType_ == 2) {
            mes[0] = 0xc6044;
            mes[1] = 0xc605d;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc642c;
            mes[1] = 0xc642d;
            mes[2] = 0xc6446;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c5c;
            mes[1] = 0xc5c75;
        }
    } else {
        if (shopType_ == 2) {
            mes[0] = 0xc6044;
            mes[1] = 0xc605a;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc642c;
            mes[1] = 0xc642d;
            mes[2] = 0xc6443;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c5c;
            mes[1] = 0xc5c72;
        }
    }
}

THUMB int MaterielMenu_SHOP_MESSAGE_MANAGER::sellDifficult()
{
    if (shopType_ == 2) {
        return 0xc6047;
    }
    if (shopType_ == 3) {
        return 0xc6430;
    }
    if (shopType_ == 4) {
        return 0xc5c5f;
    }
    return 0;
}

THUMB void MaterielMenu_SHOP_MESSAGE_MANAGER::celectNo(bool sellYet, int* mes)
{
    if (sellYet) {
        if (shopType_ == 2) {
            mes[0] = 0xc604d;
            mes[1] = 0xc605d;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc6436;
            mes[1] = 0xc6446;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c65;
            mes[1] = 0xc5c75;
        }
    } else {
        if (shopType_ == 2) {
            mes[0] = 0xc604d;
            mes[1] = 0xc605a;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc6436;
            mes[1] = 0xc6443;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c65;
            mes[1] = 0xc5c72;
        }
    }
}

THUMB void MaterielMenu_SHOP_MESSAGE_MANAGER::sellCurse(bool sellYet, int* mes)
{
    if (sellYet) {
        if (shopType_ == 2) {
            mes[0] = 0xc6057;
            mes[1] = 0xc605d;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc6440;
            mes[1] = 0xc6446;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c6f;
            mes[1] = 0xc5c75;
        }
    } else {
        if (shopType_ == 2) {
            mes[0] = 0xc6057;
            mes[1] = 0xc605a;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc6440;
            mes[1] = 0xc6443;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c6f;
            mes[1] = 0xc5c72;
        }
    }
}

THUMB void MaterielMenu_SHOP_MESSAGE_MANAGER::sellEnd(bool sellYet, int* mes)
{
    if (sellYet) {
        if (shopType_ == 2) {
            mes[0] = 0xc6054;
            mes[1] = 0xc605d;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc643d;
            mes[1] = 0xc6446;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c6c;
            mes[1] = 0xc5c75;
        }
    } else {
        if (shopType_ == 2) {
            mes[0] = 0xc6054;
            mes[1] = 0xc605a;
        }
        if (shopType_ == 3) {
            mes[0] = 0xc643d;
            mes[1] = 0xc6443;
        }
        if (shopType_ == 4) {
            mes[0] = 0xc5c6c;
            mes[1] = 0xc5c72;
        }
    }
}

THUMB void MaterielMenu_SHOP_MESSAGE_MANAGER::overMoney(int* mes)
{
    if (shopType_ == 2) {
        mes[0] = 0xc6050;
        mes[1] = 0xc6051;
        mes[2] = 0xc605d;
    }
    if (shopType_ == 3) {
        mes[0] = 0xc6439;
        mes[1] = 0xc643a;
        mes[2] = 0xc6446;
    }
    if (shopType_ == 4) {
        mes[0] = 0xc5c68;
        mes[1] = 0xc5c69;
        mes[2] = 0xc5c75;
    }
}
