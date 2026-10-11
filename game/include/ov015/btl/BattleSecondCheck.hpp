#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "ov003/btl/BattleSelectTargetParam.hpp"

namespace btl {
    struct BattleSecondCheck {
        static bool personalCheckRandom(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMahokanta(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckRarihoma(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMedapani(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckBothOne(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMahotora(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckRukani(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckSukara(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckBaikiruto(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckHoimi(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMyHoimi(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckThereHoimi(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckZaoriku(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckTaiatari(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckRarihomaForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMedapaniForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMahotoraForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMahosute(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckRukaniForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckSukaraForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckBaikirutoForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMosyasu(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckNormalCharacter(BattleSelectTargetParam *battleSelectTargetParam, status::StatusChange::Status statusType);
        static bool personalCheckMyMahokanta(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckChargeAttack(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckPartThree(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckAnkoku(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckFizzleZone(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckEscape(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckNakamayobi(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckGattai(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMahokantaForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckFreeOne(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckWeapon(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckRandomGroup(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMahokantaGroup(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckRariho(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckManusa(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMahoton(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckDokunoiki(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckYaketukuiki(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMedapaniDance(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckRukanan(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckSukuruto(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckPiorimu(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckRarihoForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMahotonForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckRukananForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckNormalMember(BattleSelectTargetParam *battleSelectTargetParam,status::StatusChange::Status statusType);
        static bool personalCheckMahokantaAll(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMegante(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckMeganteForGod(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckBehomara(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckKiariku(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckZameha(BattleSelectTargetParam *battleSelectTargetParam);
        static bool personalCheckHubaha(BattleSelectTargetParam* battleSelectTargetParam);
        static bool personalCheckMegazaru(BattleSelectTargetParam* battleSelectTargetParam);
        static bool personalCheckOtakebi(BattleSelectTargetParam* battleSelectTargetParam);
        static bool personalCheckHadou(BattleSelectTargetParam* battleSelectTargetParam);
        static bool personalCheckFree();
    };
}

namespace btl { struct BattleMonsterDraw2; }
