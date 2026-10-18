#include "main/menu/MaterielMenuOverlay.hpp"
#include "main/dss/DssUtils.hpp"

OverlaySlot g_MaterielOverlaySlot;

ARM void loadMaterielMenuOverlay(int menu)
{
    switch (menu) {
        case 0:
            break;
        case 1:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(24));
            break;
        case 2:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(27));
            break;
        case 3:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(28));
            break;
        case 4:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(29));
            break;
        case 5:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(30));
            break;
        case 6:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(31));
            break;
        case 7:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(32));
            break;
        case 8:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(33));
            break;
        case 9:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(36));
            break;
        case 10:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(35));
            break;
        case 11:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(34));
            break;
        case 12:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(25));
            break;
        case 13:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(26));
            break;
        case 14:
            g_MaterielOverlaySlot.unkfunc_0208753c(DQIV_OVERLAY_ID(37));
            break;
    }
}
