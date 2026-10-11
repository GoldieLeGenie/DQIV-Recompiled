#include "Settings.hpp"
#include "SaveStore.hpp"
#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace pc {
namespace {
std::filesystem::path settingsFile() {return saveDirectory()/"settings.ini";}
}
namespace {bool settingsLoaded=false;} // only interactive play reads and writes the file
GameSettings& gameSettings() {static GameSettings settings;return settings;}
void loadSettings() {
    settingsLoaded=true;
    std::ifstream file(settingsFile());
    auto& s=gameSettings();
    for(std::string line;std::getline(file,line);) {
        const auto equals=line.find('=');
        if(equals==std::string::npos)continue;
        const auto key=line.substr(0,equals);
        int value=0;
        try {value=std::stoi(line.substr(equals+1));} catch(...) {continue;}
        if(key=="language")s.language=std::clamp(value,0,5);
        else if(key=="showFps")s.showFps=value!=0;
        else if(key=="fullscreen")s.fullscreen=value!=0;
        else if(key=="windowSize")s.windowSize=std::clamp(value,0,windowSizeCount-1);
        else if(key=="wide")s.wide=value!=0;
        else if(key=="stretch")s.stretch=value!=0;
        else if(key=="vsync")s.vsync=value!=0;
        else if(key=="quality")s.quality=std::clamp(value,0,4);
        else if(key=="smoothTextures")s.smoothTextures=value!=0;
        else if(key=="volume")s.volume=std::clamp(value,0,100);
        else if(key=="textSpeed")s.textSpeed=std::clamp(value,0,textSpeedCount-1);
        else if(key=="frameRate")s.frameRate=std::clamp(value,0,frameRateCount-1);
        else if(key=="battleBoost")s.battleBoost=std::clamp(value,0,3);
        else if(key=="cameraZoom")s.cameraZoom=std::clamp(value,cameraZoomMin,cameraZoomMax);
        else if(key=="mobileSound")s.mobileSound=value!=0;
        else if(key.rfind("key",0)==0&&key.size()>3) {
            const int i=std::atoi(key.c_str()+3);
            if(i>=0&&i<controlCount)s.keys[i]=static_cast<unsigned>(std::stoul(line.substr(equals+1)));
        } else if(key.rfind("pad",0)==0&&key.size()>3) {
            const int i=std::atoi(key.c_str()+3);
            if(i>=0&&i<controlCount)s.pads[i]=value;
        }
    }
}
void saveSettings() {
    if(!settingsLoaded)return;
    const auto path=settingsFile();
    std::error_code error;std::filesystem::create_directories(path.parent_path(),error);
    std::ofstream file(path,std::ios::trunc);
    const auto& s=gameSettings();
    file<<"language="<<s.language<<"\nshowFps="<<s.showFps<<"\nfullscreen="<<s.fullscreen<<"\nwindowSize="<<s.windowSize
        <<"\nwide="<<s.wide<<"\nstretch="<<s.stretch<<"\nvsync="<<s.vsync<<"\nquality="<<s.quality
        <<"\nsmoothTextures="<<s.smoothTextures<<"\nvolume="<<s.volume<<"\ntextSpeed="<<s.textSpeed<<"\nframeRate="<<s.frameRate<<"\nbattleBoost="<<s.battleBoost<<"\ncameraZoom="<<s.cameraZoom<<"\nmobileSound="<<s.mobileSound<<"\n";
    for(int i=0;i<controlCount;++i)file<<"key"<<i<<"="<<s.keys[i]<<"\npad"<<i<<"="<<s.pads[i]<<"\n";
}
namespace {
// Japanese texts, found by their English text.
const char* japanese(const char* en) {
    static const std::unordered_map<std::string_view,const char*> texts={
        {"16:9 fills the screen; DS screens keeps the original layout.","16:9は がめんいっぱい、DSのがめんは もとのならび。"},
        {"3D quality","3Dのひんしつ"},
        {"3D supersampling; Auto adapts to the graphics card.","3Dのスーパーサンプリング。オートは グラフィックカードにあわせる。"},
        {"A (confirm)","A (けってい)"},
        {"Auto","オート"},
        {"Avoids tearing.","がめんの ずれを ふせぐ。"},
        {"B (cancel)","B (キャンセル)"},
        {"Back","もどる"},
        {"Battle messages","せんとうメッセージ"},
        {"Battle speed","せんとうの はやさ"},
        {"Camera zoom","カメラズーム"},
        {"DS screens","DSのがめん"},
        {"Default controls","しょきせっていに もどす"},
        {"Display","がめん"},
        {"Down","した"},
        {"Fast","はやい"},
        {"Fills the window instead of keeping proportions.","たてよこの ひりつを かえて ウィンドウに あわせる。"},
        {"Filtered 3D textures (No: sharp pixels).","3Dテクスチャを なめらかにする (いいえ: ドットのまま)。"},
        {"Frame rate","フレームレート"},
        {"Frames per second, top left.","1びょうあたりの フレームすう (ひだりうえ)。"},
        {"Full screen","フルスクリーン"},
        {"Game","ゲーム"},
        {"Gamepad","コントローラー"},
        {"How fast dialogue text appears.","メッセージの でる はやさ。"},
        {"Instant","いっしゅん"},
        {"Keyboard","キーボード"},
        {"L/R: page","L/R: ページ"},
        {"Language","ことば"},
        {"Language of the game's texts.","ゲームの ことば。"},
        {"Left","ひだり"},
        {"Music","おんがく"},
        {"No","いいえ"},
        {"Normal","ふつう"},
        {"Overall volume.","ぜんたいの おんりょう。"},
        {"Picture","がめんの かたち"},
        {"Press...","ボタンを おしてください..."},
        {"Right","みぎ"},
        {"Settings","せってい"},
        {"Show FPS","FPSを ひょうじ"},
        {"Size in windowed mode.","ウィンドウモードの おおきさ。"},
        {"Slow","おそい"},
        {"Smooth textures","なめらかな テクスチャ"},
        {"Sound effects","こうかおん"},
        {"Stretch picture","がめんを ひきのばす"},
        {"Text speed","メッセージの はやさ"},
        {"Unlimited","むせいげん"},
        {"Up","うえ"},
        {"Vertical sync","すいちょくどうき"},
        {"Very fast","とても はやい"},
        {"Volume","おんりょう"},
        {"Wait for a key","ボタンを まつ"},
        {"Window size","ウィンドウの おおきさ"},
        {"Japanese","日本語"},
        {"HD sound","HDサウンド"},
        {"Mobile","スマホばん"},
        {"Music and sounds: smartphone recordings or the DS synthesizer.","おんがくと こうかおん: スマホばんの ろくおん か DSの おと。"},
        {"Party chat","なかまと はなす"},
        {"X (menu)","X (メニュー)"},
        {"Yes","はい"},
    };
    const auto found=texts.find(en);
    return found!=texts.end()?found->second:en;
}
// Texts per game language: Japanese, English, French, German, Italian, Spanish.
const char* pick(int language,const char* en,const char* fr,const char* de,const char* it,const char* es) {
    switch(language) {case 0:return japanese(en);case 2:return fr;case 3:return de;case 4:return it;case 5:return es;default:return en;}
}
const char* yesNo(bool value,int language) {
    return value?pick(language,"Yes","Oui","Ja","Sì","Sí"):pick(language,"No","Non","Nein","No","No");
}
bool applyRequested=false;
std::atomic<bool> partyChatRequest=false;
}
void setPartyChatRequest(bool pressed) {partyChatRequest=pressed;}
bool takePartyChatRequest() {return partyChatRequest.exchange(false);}
const char* settingsTitle(int language) {return pick(language,"Settings","Paramètres","Einstellungen","Impostazioni","Ajustes");}
const char* pageTitle(int page,int language) {
    switch(page) {
    case pageKeyboard:return pick(language,"Keyboard","Clavier","Tastatur","Tastiera","Teclado");
    case pageGamepad:return pick(language,"Gamepad","Manette","Controller","Controller","Mando");
    case pageDisplay:return pick(language,"Display","Affichage","Anzeige","Schermo","Pantalla");
    }
    return pick(language,"Game","Jeu","Spiel","Gioco","Juego");
}
const char* controlName(int action,int language) {
    switch(action) {
    case 0:return pick(language,"A (confirm)","A (valider)","A (bestätigen)","A (conferma)","A (aceptar)");
    case 1:return pick(language,"B (cancel)","B (annuler)","B (abbrechen)","B (annulla)","B (cancelar)");
    case 2:return pick(language,"X (menu)","X (menu)","X (Menü)","X (menu)","X (menú)");
    case 3:return "Y";
    case 4:return "L";
    case 5:return "R";
    case 6:return "Start";
    case 7:return "Select";
    case 8:return pick(language,"Up","Haut","Oben","Su","Arriba");
    case 9:return pick(language,"Down","Bas","Unten","Giù","Abajo");
    case 10:return pick(language,"Left","Gauche","Links","Sinistra","Izquierda");
    case 11:return pick(language,"Right","Droite","Rechts","Destra","Derecha");
    case controlPartyChat:return pick(language,"Party chat","Parler au groupe","Gruppengespräch","Parla al gruppo","Hablar al grupo");
    }
    return "";
}
const char* rebindPrompt(int language) {return pick(language,"Press...","Appuyez...","Drü""cken...","Premi...","Pulsa...");}
const char* pageHint(int language) {return pick(language,"L/R: page","L/R : page","L/R: Seite","L/R: pagina","L/R: página");}
const char* resetLabel(int language) {return pick(language,"Default controls","Touches par dé""faut","Standardbelegung","Comandi predefiniti","Controles por defecto");}
namespace {int waitingPage=-1,waitingAction=-1;}
void startRebind(int page,int action) {waitingPage=page;waitingAction=action;}
bool rebindWaiting() {return waitingPage>=0;}
void cancelRebind() {waitingPage=waitingAction=-1;}
int rebindPage() {return waitingPage;}
int rebindAction() {return waitingAction;}
void resetControls(int page) {
    const GameSettings defaults;
    auto& s=gameSettings();
    for(int i=0;i<controlCount;++i) {
        if(page==pageKeyboard)s.keys[i]=defaults.keys[i];
        if(page==pageGamepad)s.pads[i]=defaults.pads[i];
    }
}
const char* settingsBack(int language) {return pick(language,"Back","Retour","Zurück","Indietro","Volver");}
const char* settingLabel(int item,int language) {
    switch(item) {
    case settingLanguage:return pick(language,"Language","Langue","Sprache","Lingua","Idioma");
    case settingFps:return pick(language,"Show FPS","Afficher les FPS","FPS anzeigen","Mostra FPS","Mostrar FPS");
    case settingFullscreen:return pick(language,"Full screen","Plein écran","Vollbild","Schermo intero","Pantalla completa");
    case settingWindowSize:return pick(language,"Window size","Taille de la fenêtre","Fenstergröße","Dimensione finestra","Tamaño de ventana");
    case settingPicture:return pick(language,"Picture","Format de l'image","Bildformat","Formato immagine","Formato de imagen");
    case settingStretch:return pick(language,"Stretch picture","Étirer l'image","Bild strecken","Allarga immagine","Estirar imagen");
    case settingVsync:return pick(language,"Vertical sync","Synchro verticale","V-Sync","Sincronia verticale","Sincronía vertical");
    case settingQuality:return pick(language,"3D quality","Qualité 3D","3D-Qualität","Qualità 3D","Calidad 3D");
    case settingTextures:return pick(language,"Smooth textures","Textures lissées","Glatte Texturen","Texture filtrate","Texturas suaves");
    case settingVolume:return pick(language,"Volume","Volume général","Gesamtlautstärke","Volume generale","Volumen general");
    case settingTextSpeed:return pick(language,"Text speed","Vitesse du texte","Textgeschwindigkeit","Velocità del testo","Velocidad del texto");
    case settingFrameRate:return pick(language,"Frame rate","Images par seconde","Bildrate","Fotogrammi al secondo","Imágenes por segundo");
    case settingBattleBoost:return pick(language,"Battle speed","Vitesse des combats","Kampftempo","Velocità battaglie","Velocidad de combate");
    case settingCameraZoom:return pick(language,"Camera zoom","Zoom caméra","Kamerazoom","Zoom telecamera","Zoom de cámara");
    case settingMobileSound:return pick(language,"HD sound","Son HD","HD-Sound","Audio HD","Sonido HD");
    }
    return "";
}
std::string settingValue(int item,int language) {
    const auto& s=gameSettings();
    switch(item) {
    case settingLanguage: {
        static const char* names[]={"","English","Français","Deutsch","Italiano","Español"};
        // The menu font of the other languages has no kanji.
        if(s.language==0)return pick(language,"Japanese","Japonais","Japanisch","Giapponese","Japonés");
        return names[std::clamp(s.language,1,5)];
    }
    case settingFps:return yesNo(s.showFps,language);
    case settingFullscreen:return yesNo(s.fullscreen,language);
    case settingWindowSize:return std::to_string(windowSizes[s.windowSize][0])+"x"+std::to_string(windowSizes[s.windowSize][1]);
    case settingPicture:return s.wide?"16:9":pick(language,"DS screens","Écrans DS","DS-Bildschirme","Schermi DS","Pantallas DS");
    case settingStretch:return yesNo(s.stretch,language);
    case settingVsync:return yesNo(s.vsync,language);
    case settingQuality:
        if(s.quality==0)return pick(language,"Auto","Auto","Auto","Auto","Auto");
        return std::to_string(s.quality)+"x";
    case settingTextures:return yesNo(s.smoothTextures,language);
    case settingVolume:return std::to_string(s.volume)+"%";
    case settingTextSpeed:
        switch(s.textSpeed) {
        case textSlow:return pick(language,"Slow","Lente","Langsam","Lenta","Lenta");
        case textFast:return pick(language,"Fast","Rapide","Schnell","Veloce","Rápida");
        case textInstant:return pick(language,"Instant","Instantanée","Sofort","Istantanea","Instantánea");
        }
        return pick(language,"Normal","Normale","Normal","Normale","Normal");
    case settingFrameRate:
        if(!frameRates[s.frameRate])return pick(language,"Unlimited","Illimité","Unbegrenzt","Illimitati","Ilimitadas");
        return std::to_string(frameRates[s.frameRate]);
    case settingBattleBoost:return "x"+std::to_string(s.battleBoost+1);
    case settingCameraZoom:return std::to_string(s.cameraZoom)+"%";
    case settingMobileSound:return s.mobileSound?pick(language,"Mobile","Mobile","Mobil","Mobile","Móvil"):"DS";
    }
    return {};
}
const char* settingHint(int item,int language) {
    switch(item) {
    case settingLanguage:return pick(language,"Language of the game's texts.","Langue des textes du jeu.","Sprache der Spieltexte.","Lingua dei testi del gioco.","Idioma de los textos del juego.");
    case settingFps:return pick(language,"Frames per second, top left.","Images par seconde, en haut à gauche.","Bilder pro Sekunde, oben links.","Fotogrammi al secondo, in alto a sinistra.","Imágenes por segundo, arriba a la izquierda.");
    case settingWindowSize:return pick(language,"Size in windowed mode.","Taille en mode fenêtré.","Größe im Fenstermodus.","Dimensione in modalità finestra.","Tamaño en modo ventana.");
    case settingPicture:return pick(language,"16:9 fills the screen; DS screens keeps the original layout.","16:9 remplit l'écran ; Écrans DS garde la disposition d'origine.","16:9 füllt den Bildschirm; DS-Bildschirme: Original.","16:9 riempie lo schermo; Schermi DS: disposizione originale.","16:9 llena la pantalla; Pantallas DS: disposición original.");
    case settingStretch:return pick(language,"Fills the window instead of keeping proportions.","Remplit la fenêtre au lieu de garder les proportions.","Füllt das Fenster statt die Proportionen zu halten.","Riempie la finestra invece di mantenere le proporzioni.","Llena la ventana en lugar de mantener las proporciones.");
    case settingVsync:return pick(language,"Avoids tearing.","Évite les déchirures de l'image.","Verhindert Bildrisse.","Evita lo strappo dell'immagine.","Evita el desgarro de la imagen.");
    case settingQuality:return pick(language,"3D supersampling; Auto adapts to the graphics card.","Suréchantillonnage 3D ; Auto s'adapte à la carte graphique.","3D-Supersampling; Auto passt sich der Grafikkarte an.","Supersampling 3D; Auto si adatta alla scheda video.","Supermuestreo 3D; Auto se adapta a la tarjeta gráfica.");
    case settingTextures:return pick(language,"Filtered 3D textures (No: sharp pixels).","Textures 3D filtrées (Non : pixels nets).","Gefilterte 3D-Texturen (Nein: scharfe Pixel).","Texture 3D filtrate (No: pixel nitidi).","Texturas 3D filtradas (No: píxeles nítidos).");
    case settingVolume:return pick(language,"Overall volume.","Volume général.","Gesamtlautstärke.","Volume generale.","Volumen general.");
    case settingMobileSound:return pick(language,"Music and sounds: smartphone recordings or the DS synthesizer.","Musiques et bruitages : enregistrements de la version mobile ou synthé de la DS.","Musik und Klänge: Aufnahmen der Smartphone-Version oder DS-Synthesizer.","Musica e suoni: registrazioni della versione mobile o sintetizzatore DS.","Música y sonidos: grabaciones de la versión móvil o sintetizador de DS.");
    case settingTextSpeed:return pick(language,"How fast dialogue text appears.","Vitesse d'affichage des dialogues.","Wie schnell Dialogtext erscheint.","Velocità di comparsa dei dialoghi.","Velocidad de aparición de los diálogos.");
    }
    return "";
}
bool changeSetting(int item,int step) {
    auto& s=gameSettings();
    auto cycle=[&](int v,int low,int high){return v+step<low?high:v+step>high?low:v+step;};
    switch(item) {
    case settingLanguage:s.language=cycle(s.language,0,5);return true;
    case settingFps:s.showFps=!s.showFps;return true;
    case settingMobileSound:s.mobileSound=!s.mobileSound;return true;
    case settingFullscreen:s.fullscreen=!s.fullscreen;return true;
    case settingWindowSize:s.windowSize=cycle(s.windowSize,0,windowSizeCount-1);return true;
    case settingPicture:s.wide=!s.wide;return true;
    case settingStretch:s.stretch=!s.stretch;return true;
    case settingVsync:s.vsync=!s.vsync;return true;
    case settingQuality:s.quality=cycle(s.quality,0,4);return true;
    case settingTextures:s.smoothTextures=!s.smoothTextures;return true;
    case settingVolume: {
        const int before=s.volume;s.volume=std::clamp(s.volume+step*10,0,100);return s.volume!=before;
    }
    case settingTextSpeed: {
        const int before=s.textSpeed;s.textSpeed=std::clamp(s.textSpeed+step,0,textSpeedCount-1);return s.textSpeed!=before;
    }
    case settingFrameRate:s.frameRate=cycle(s.frameRate,0,frameRateCount-1);return true;
    case settingCameraZoom: {
        const int before=s.cameraZoom;s.cameraZoom=std::clamp(s.cameraZoom+step*10,cameraZoomMin,cameraZoomMax);return s.cameraZoom!=before;
    }
    case settingBattleBoost: {
        const int before=s.battleBoost;s.battleBoost=std::clamp(s.battleBoost+step,0,3);return s.battleBoost!=before;
    }
    }
    return false;
}
const char* battleSpeedLabel(int language) {
    return pick(language,"Battle messages","Messages de combat","Kampfnachrichten","Messaggi di battaglia","Mensajes de combate");
}
const char* battleSpeedValue(int speed,int language) {
    switch(speed) {
    case 0:return pick(language,"Very fast","Très rapide","Sehr schnell","Molto veloce","Muy rápida");
    case 1:return pick(language,"Fast","Rapide","Schnell","Veloce","Rápida");
    case 3:return pick(language,"Slow","Lente","Langsam","Lenta","Lenta");
    case 4:return pick(language,"Wait for a key","Attendre une touche","Auf Taste warten","Attendi un tasto","Esperar una tecla");
    }
    return pick(language,"Normal","Normale","Normal","Normale","Normal");
}
const char* musicLabel(int language) {return pick(language,"Music","Musique","Musik","Musica","Música");}
const char* soundsLabel(int language) {return pick(language,"Sound effects","Effets sonores","Soundeffekte","Effetti sonori","Efectos de sonido");}
void zoomCamera(int step) {
    auto& s=gameSettings();
    s.cameraZoom=std::clamp(s.cameraZoom+step,cameraZoomMin,cameraZoomMax);
}
void requestSettingsApply() {applyRequested=true;}
bool takeSettingsApply() {const bool requested=applyRequested;applyRequested=false;return requested;}
}
