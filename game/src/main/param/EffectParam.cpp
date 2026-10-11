#include "main/param/Param.hpp"
#include "main/dss/DssUtils.hpp"

const unsigned int param::EffectParam::size_ = 201;
const unsigned int param::EffectParam::ID_ = 78204;
DataObject param::EffectParam::data_;

THUMB void param::EffectParam::getCameraFile(int camera, char* file)
{
    switch (camera) {
        case 1:
            dss::strcpy(file, "bagix");
            break;
        case 2:
            dss::strcpy(file, "bagix2");
            break;
        case 3:
            dss::strcpy(file, "baron");
            break;
        case 4:
            dss::strcpy(file, "baron2");
            break;
        case 5:
            dss::strcpy(file, "begiragon");
            break;
        case 6:
            dss::strcpy(file, "begiragon2");
            break;
        case 7:
            dss::strcpy(file, "begirama");
            break;
        case 8:
            dss::strcpy(file, "begirama2");
            break;
        case 9:
            dss::strcpy(file, "btltest");
            break;
        case 10:
            dss::strcpy(file, "gigasword");
            break;
        case 11:
            dss::strcpy(file, "gigasword2");
            break;
        case 12:
            dss::strcpy(file, "gira");
            break;
        case 13:
            dss::strcpy(file, "hyadain");
            break;
        case 14:
            dss::strcpy(file, "ionazun");
            break;
        case 15:
            dss::strcpy(file, "ionazun2");
            break;
        case 16:
            dss::strcpy(file, "jigospark");
            break;
        case 17:
            dss::strcpy(file, "jigospark2");
            break;
        case 18:
            dss::strcpy(file, "jisin");
            break;
        case 19:
            dss::strcpy(file, "jiware");
            break;
        case 20:
            dss::strcpy(file, "jiware2");
            break;
        case 21:
            dss::strcpy(file, "m000_at");
            break;
        case 22:
            dss::strcpy(file, "m000_at2");
            break;
        case 23:
            dss::strcpy(file, "m000_ju");
            break;
        case 24:
            dss::strcpy(file, "m001_at");
            break;
        case 25:
            dss::strcpy(file, "m007_at");
            break;
        case 26:
            dss::strcpy(file, "m007_s1");
            break;
        case 27:
            dss::strcpy(file, "m012_ju");
            break;
        case 28:
            dss::strcpy(file, "m012_s1");
            break;
        case 29:
            dss::strcpy(file, "m020_na");
            break;
        case 30:
            dss::strcpy(file, "m027_at");
            break;
        case 31:
            dss::strcpy(file, "m029_at");
            break;
        case 32:
            dss::strcpy(file, "m029_atz");
            break;
        case 33:
            dss::strcpy(file, "m029_fo");
            break;
        case 34:
            dss::strcpy(file, "m029_ju");
            break;
        case 35:
            dss::strcpy(file, "m032_at");
            break;
        case 36:
            dss::strcpy(file, "m035_s2");
            break;
        case 37:
            dss::strcpy(file, "m035_s3");
            break;
        case 38:
            dss::strcpy(file, "m039_dk");
            break;
        case 39:
            dss::strcpy(file, "m039_na");
            break;
        case 40:
            dss::strcpy(file, "m042_s1");
            break;
        case 41:
            dss::strcpy(file, "m044_at");
            break;
        case 42:
            dss::strcpy(file, "m047_at");
            break;
        case 43:
            dss::strcpy(file, "m047_tu");
            break;
        case 44:
            dss::strcpy(file, "m057_s2");
            break;
        case 45:
            dss::strcpy(file, "m058_tu");
            break;
        case 46:
            dss::strcpy(file, "m060_at");
            break;
        case 47:
            dss::strcpy(file, "m063_tu");
            break;
        case 48:
            dss::strcpy(file, "m066_at");
            break;
        case 49:
            dss::strcpy(file, "m066_ju");
            break;
        case 50:
            dss::strcpy(file, "m067_at");
            break;
        case 51:
            dss::strcpy(file, "m067_br");
            break;
        case 52:
            dss::strcpy(file, "m067_ju");
            break;
        case 53:
            dss::strcpy(file, "m072_at");
            break;
        case 54:
            dss::strcpy(file, "m072_atz");
            break;
        case 55:
            dss::strcpy(file, "m072_ju");
            break;
        case 56:
            dss::strcpy(file, "m072_juz");
            break;
        case 57:
            dss::strcpy(file, "m072_tu");
            break;
        case 58:
            dss::strcpy(file, "m077_s2");
            break;
        case 59:
            dss::strcpy(file, "m078_at");
            break;
        case 60:
            dss::strcpy(file, "m085_dk");
            break;
        case 61:
            dss::strcpy(file, "m087_at");
            break;
        case 62:
            dss::strcpy(file, "m096_dk");
            break;
        case 63:
            dss::strcpy(file, "m097_tu");
            break;
        case 64:
            dss::strcpy(file, "m100_at");
            break;
        case 65:
            dss::strcpy(file, "m100_br");
            break;
        case 66:
            dss::strcpy(file, "m100_ju");
            break;
        case 67:
            dss::strcpy(file, "m106_na");
            break;
        case 68:
            dss::strcpy(file, "m107_at");
            break;
        case 69:
            dss::strcpy(file, "m110_at");
            break;
        case 70:
            dss::strcpy(file, "m114_at");
            break;
        case 71:
            dss::strcpy(file, "m132_s2");
            break;
        case 72:
            dss::strcpy(file, "m133_at");
            break;
        case 73:
            dss::strcpy(file, "m133_br");
            break;
        case 74:
            dss::strcpy(file, "m133_ju");
            break;
        case 75:
            dss::strcpy(file, "m152_at");
            break;
        case 76:
            dss::strcpy(file, "m153_s1");
            break;
        case 77:
            dss::strcpy(file, "m158_at");
            break;
        case 78:
            dss::strcpy(file, "m169_s1");
            break;
        case 79:
            dss::strcpy(file, "m169_s1z");
            break;
        case 80:
            dss::strcpy(file, "m174_at");
            break;
        case 81:
            dss::strcpy(file, "m174_atz");
            break;
        case 82:
            dss::strcpy(file, "m174_tu");
            break;
        case 83:
            dss::strcpy(file, "m174_tuz");
            break;
        case 84:
            dss::strcpy(file, "m181_at");
            break;
        case 85:
            dss::strcpy(file, "m181_br");
            break;
        case 86:
            dss::strcpy(file, "m181_ju");
            break;
        case 87:
            dss::strcpy(file, "m181_tu");
            break;
        case 88:
            dss::strcpy(file, "m188_br");
            break;
        case 89:
            dss::strcpy(file, "m188_s1");
            break;
        case 90:
            dss::strcpy(file, "m188_s2");
            break;
        case 91:
            dss::strcpy(file, "m193_at");
            break;
        case 92:
            dss::strcpy(file, "m193_s1");
            break;
        case 93:
            dss::strcpy(file, "m206_at");
            break;
        case 94:
            dss::strcpy(file, "m206_br");
            break;
        case 95:
            dss::strcpy(file, "m206_ju");
            break;
        case 96:
            dss::strcpy(file, "m206_s1");
            break;
        case 97:
            dss::strcpy(file, "m206_s3");
            break;
        case 98:
            dss::strcpy(file, "m207_at");
            break;
        case 99:
            dss::strcpy(file, "m207_atz");
            break;
        case 100:
            dss::strcpy(file, "m207_br");
            break;
        case 101:
            dss::strcpy(file, "m207_s2");
            break;
        case 102:
            dss::strcpy(file, "m207_s3");
            break;
        case 103:
            dss::strcpy(file, "m208_at");
            break;
        case 104:
            dss::strcpy(file, "m209_at");
            break;
        case 105:
            dss::strcpy(file, "m210_br");
            break;
        case 106:
            dss::strcpy(file, "m210_s1");
            break;
        case 107:
            dss::strcpy(file, "m210_s1f");
            break;
        case 108:
            dss::strcpy(file, "m210_s3");
            break;
        case 109:
            dss::strcpy(file, "m231_at");
            break;
        case 110:
            dss::strcpy(file, "m232_at");
            break;
        case 111:
            dss::strcpy(file, "m233_at");
            break;
        case 112:
            dss::strcpy(file, "m235_s1");
            break;
        case 113:
            dss::strcpy(file, "m236_s1");
            break;
        case 114:
            dss::strcpy(file, "m237_at");
            break;
        case 115:
            dss::strcpy(file, "m237_s1");
            break;
        case 116:
            dss::strcpy(file, "m240_s1");
            break;
        case 117:
            dss::strcpy(file, "m242_s1");
            break;
        case 118:
            dss::strcpy(file, "m242_s4");
            break;
        case 119:
            dss::strcpy(file, "m243_s1");
            break;
        case 120:
            dss::strcpy(file, "m246_at");
            break;
        case 121:
            dss::strcpy(file, "m250_tai");
            break;
        case 122:
            dss::strcpy(file, "m250_taiz");
            break;
        case 123:
            dss::strcpy(file, "m252_tai");
            break;
        case 124:
            dss::strcpy(file, "m252_taiz");
            break;
        case 125:
            dss::strcpy(file, "m254_tai");
            break;
        case 126:
            dss::strcpy(file, "m255_tai");
            break;
        case 127:
            dss::strcpy(file, "m256_tai");
            break;
        case 128:
            dss::strcpy(file, "m257_tai");
            break;
        case 129:
            dss::strcpy(file, "m258_tai");
            break;
        case 130:
            dss::strcpy(file, "m259_tai");
            break;
        case 131:
            dss::strcpy(file, "m260_tai");
            break;
        case 132:
            dss::strcpy(file, "m261_tai");
            break;
        case 133:
            dss::strcpy(file, "m300_br");
            break;
        case 134:
            dss::strcpy(file, "m300_ju");
            break;
        case 135:
            dss::strcpy(file, "m300_s1");
            break;
        case 136:
            dss::strcpy(file, "m300_s1z");
            break;
        case 137:
            dss::strcpy(file, "m302_br");
            break;
        case 138:
            dss::strcpy(file, "m302_s3");
            break;
        case 139:
            dss::strcpy(file, "madante");
            break;
        case 140:
            dss::strcpy(file, "madante_evi");
            break;
        case 141:
            dss::strcpy(file, "madante2");
            break;
        case 142:
            dss::strcpy(file, "mahyado");
            break;
        case 143:
            dss::strcpy(file, "mahyado2");
            break;
        case 144:
            dss::strcpy(file, "merazoma");
            break;
        case 145:
            dss::strcpy(file, "minadein");
            break;
        case 146:
            dss::strcpy(file, "minadein2");
            break;
        case 147:
            dss::strcpy(file, "moonsalt");
            break;
        case 148:
            dss::strcpy(file, "moonsalt2");
            break;
        case 149:
            dss::strcpy(file, "p_hikari");
            break;
        case 150:
            dss::strcpy(file, "p_majin");
            break;
        case 151:
            dss::strcpy(file, "parupunte");
            break;
        case 152:
            dss::strcpy(file, "sinkuha");
            break;
        case 153:
            dss::strcpy(file, "sinkuha2");
            break;
        case 154:
            dss::strcpy(file, "start");
            break;
        case 155:
            dss::strcpy(file, "syonin");
            break;
        case 156:
            dss::strcpy(file, "tateyoko1");
            break;
        case 157:
            dss::strcpy(file, "tateyoko2");
            break;
        case 158:
            dss::strcpy(file, "tateyure1");
            break;
        case 159:
            dss::strcpy(file, "tateyure2");
            break;
        case 160:
            dss::strcpy(file, "toucard");
            break;
        case 161:
            dss::strcpy(file, "zengoyure1");
            break;
        case 162:
            dss::strcpy(file, "zengoyure2");
            break;
        default:
            dss::strcpy(file, "inicamera");
            break;
    }
}
