#include "main/menu/MaterielMenuOverlay.hpp"
#include "main/dss/DssUtils.hpp"

UnkOverlaySlot data_020efc58;

ARM void loadMaterielMenuOverlay(int menu)
{
    switch (menu) {
        case 0:
            break;
        case 1:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(24));
            break;
        case 2:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(27));
            break;
        case 3:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(28));
            break;
        case 4:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(29));
            break;
        case 5:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(30));
            break;
        case 6:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(31));
            break;
        case 7:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(32));
            break;
        case 8:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(33));
            break;
        case 9:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(36));
            break;
        case 10:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(35));
            break;
        case 11:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(34));
            break;
        case 12:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(25));
            break;
        case 13:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(26));
            break;
        case 14:
            data_020efc58.unkfunc_0208753c(DQIV_OVERLAY_ID(37));
            break;
    }
}
