#include "ov016/TownMenu_ITEM/TownMenuItemTarotMessage.hpp"
#include "main/dss/Random.hpp"

THUMB void TownMenuItemTarotMessage::getTarotMessage(int* message, int index)
{
    if (index == -1) {
        index = dssrand::rand(0x30);
    }
    switch (index) {
        case 0:
            message[0] = 0xc5060;
            message[1] = 0xc5061;
            message[2] = 0xc5062;
            break;
        case 1:
            message[0] = 0xc5065;
            message[1] = 0xc5066;
            message[2] = 0xc5067;
            break;
        case 2:
            message[0] = 0xc506a;
            message[1] = 0xc506b;
            message[2] = 0xc506c;
            break;
        case 3:
            message[0] = 0xc506f;
            message[1] = 0xc5070;
            message[2] = 0xc5071;
            break;
        case 4:
            message[0] = 0xc5074;
            message[1] = 0xc5075;
            message[2] = 0xc5076;
            break;
        case 5:
            message[0] = 0xc5079;
            message[1] = 0xc507a;
            message[2] = 0xc507b;
            break;
        case 6:
            message[0] = 0xc507e;
            message[1] = 0xc507f;
            message[2] = 0xc5080;
            break;
        case 7:
            message[0] = 0xc5083;
            message[1] = 0xc5084;
            message[2] = 0xc5085;
            break;
        case 8:
            message[0] = 0xc5088;
            message[1] = 0xc5089;
            message[2] = 0xc508a;
            break;
        case 9:
            message[0] = 0xc508d;
            message[1] = 0xc508e;
            message[2] = 0xc508f;
            break;
        case 10:
            message[0] = 0xc5092;
            message[1] = 0xc5093;
            message[2] = 0xc5094;
            break;
        case 11:
            message[0] = 0xc5097;
            message[1] = 0xc5098;
            message[2] = 0xc5099;
            break;
        case 12:
            message[0] = 0xc509c;
            message[1] = 0xc509d;
            message[2] = 0xc509e;
            break;
        case 13:
            message[0] = 0xc50a1;
            message[1] = 0xc50a2;
            message[2] = 0xc50a3;
            break;
        case 14:
            message[0] = 0xc50a6;
            message[1] = 0xc50a7;
            message[2] = 0xc50a8;
            break;
        case 15:
            message[0] = 0xc50ab;
            message[1] = 0xc50ac;
            message[2] = 0xc50ad;
            break;
        case 16:
            message[0] = 0xc50b0;
            message[1] = 0xc50b1;
            message[2] = 0xc50b2;
            break;
        case 17:
            message[0] = 0xc50b5;
            message[1] = 0xc50b6;
            message[2] = 0xc50b7;
            break;
        case 18:
            message[0] = 0xc50ba;
            message[1] = 0xc50bb;
            message[2] = 0xc50bc;
            break;
        case 19:
            message[0] = 0xc50bf;
            message[1] = 0xc50c0;
            message[2] = 0xc50c1;
            break;
        case 20:
            message[0] = 0xc50c4;
            message[1] = 0xc50c5;
            message[2] = 0xc50c6;
            break;
        case 21:
            message[0] = 0xc50c9;
            message[1] = 0xc50ca;
            message[2] = 0xc50cb;
            break;
        case 22:
            message[0] = 0xc50ce;
            message[1] = 0xc50cf;
            message[2] = 0xc50d0;
            break;
        case 23:
            message[0] = 0xc50d3;
            message[1] = 0xc50d4;
            message[2] = 0xc50d5;
            break;
        case 24:
            message[0] = 0xc50d8;
            message[1] = 0xc50d9;
            message[2] = 0xc50da;
            break;
        case 25:
            message[0] = 0xc50dd;
            message[1] = 0xc50de;
            message[2] = 0xc50df;
            break;
        case 26:
            message[0] = 0xc50e2;
            message[1] = 0xc50e3;
            message[2] = 0xc50e4;
            break;
        case 27:
            message[0] = 0xc50e7;
            message[1] = 0xc50e8;
            message[2] = 0xc50e9;
            break;
        case 28:
            message[0] = 0xc50ec;
            message[1] = 0xc50ed;
            message[2] = 0xc50ee;
            break;
        case 29:
            message[0] = 0xc50f1;
            message[1] = 0xc50f2;
            message[2] = 0xc50f3;
            break;
        case 30:
            message[0] = 0xc50f6;
            message[1] = 0xc50f7;
            message[2] = 0xc50f8;
            break;
        case 31:
            message[0] = 0xc50fb;
            message[1] = 0xc50fc;
            message[2] = 0xc50fd;
            break;
        case 32:
            message[0] = 0xc5100;
            message[1] = 0xc5101;
            message[2] = 0xc5102;
            break;
        case 33:
            message[0] = 0xc5105;
            message[1] = 0xc5106;
            message[2] = 0xc5107;
            break;
        case 34:
            message[0] = 0xc510a;
            message[1] = 0xc510b;
            message[2] = 0xc510c;
            break;
        case 35:
            message[0] = 0xc510f;
            message[1] = 0xc5110;
            message[2] = 0xc5111;
            break;
        case 36:
            message[0] = 0xc5114;
            message[1] = 0xc5115;
            message[2] = 0xc5116;
            break;
        case 37:
            message[0] = 0xc5119;
            message[1] = 0xc511a;
            message[2] = 0xc511b;
            break;
        case 38:
            message[0] = 0xc511e;
            message[1] = 0xc511f;
            message[2] = 0xc5120;
            break;
        case 39:
            message[0] = 0xc5123;
            message[1] = 0xc5124;
            message[2] = 0xc5125;
            break;
        case 40:
            message[0] = 0xc5128;
            message[1] = 0xc5129;
            message[2] = 0xc512a;
            break;
        case 41:
            message[0] = 0xc512d;
            message[1] = 0xc512e;
            message[2] = 0xc512f;
            break;
        case 42:
            message[0] = 0xc5132;
            message[1] = 0xc5133;
            message[2] = 0xc5134;
            break;
        case 43:
            message[0] = 0xc5137;
            message[1] = 0xc5138;
            message[2] = 0xc5139;
            break;
        case 44:
            message[0] = 0xc513c;
            message[1] = 0xc513d;
            message[2] = 0xc513e;
            break;
        case 45:
            message[0] = 0xc5141;
            message[1] = 0xc5142;
            message[2] = 0xc5143;
            break;
        case 46:
            message[0] = 0xc5146;
            message[1] = 0xc5147;
            message[2] = 0xc5148;
            break;
        case 47:
            message[0] = 0xc514b;
            message[1] = 0xc514c;
            message[2] = 0xc514d;
            message[3] = 0xc514e;
            message[4] = 0xc514f;
            break;
        case 48:
            message[0] = 0xc505a;
            message[1] = 0xc505b;
            message[2] = 0xc505c;
            message[3] = 0xc505d;
            break;
        case 49:
            message[0] = 0xc5152;
            message[1] = 0xc5153;
            break;
        case 50:
            message[0] = 0xc5159;
            message[1] = 0xc515a;
            break;
        case 51:
            message[0] = 0xc5156;
            break;
    }
}
