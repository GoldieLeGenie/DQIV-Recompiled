#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/UnkTextConvert.hpp"
#include "main/dss/UnkMessageFind.hpp"
#include "main/dss/UnkBgText.hpp"

struct TextHookBase;

struct TextAPI {
    static void setLanguage(int language);
    static void setMACRO0(int slot, int type, int value, int value2, int value3);
    static void setMACRO0(int slot, int type, int value, int value2);
    static void setMACRO0(int slot, int type, int value);
    static void setMACRO1(int slot, int type, int value);
    static void setMACRO2(int slot, int type, int value);
    static void setMACRO3(int slot, int type, int value);
    static void setMACRO4(int slot, int type, int value);
    static void setMACRO5(int slot, int type, int value);
    static void resetMacro();
    static int isExistMessage(int messageID);
    static void getMessage(char* message, int messageSize, char* name, int nameSize, int messageID);
    static int getMessageSound();
    static void getMonsterNamePlateTextImitation(char* text, int monsterIndex, int flag);
    static void getPlayerNamePlateTextImitation(char* text, int playerIndex, int flag);
    static void extractText(char* text, int size, int type, int value);
    static void setHeroName(char* name);
    static void setUserString(int index, char* string);
    static int isGermanMonsterException(int monsterIndex);
    static void unkfunc_02054690();
    static void Init();
    static void unkfunc_020547f0();
    static void unkfunc_020547f4();
    static void unkfunc_0205487c();
    static void unkfunc_02054884();
    static void unkfunc_02054888();

    static unsigned char m_msg_last_sound;
    static int m_lang;
    static int m_extra;
    static char m_work2[0x80];
    static char m_work1[0x200];
};

enum MACRO_STAT {
    MST_NULL            = 0x0,
    MST_MALE            = 0x1,
    MST_FEMALE          = 0x2,
    MST_NEUTER          = 0x4,
    MST_SOLO            = 0x8,
    MST_PROPER          = 0x10,
    MST_VOWEL           = 0x20,
    MST_VOWEL_FR        = 0x40,
    MST_SINGLE          = 0x80,
    MST_SINGLE_FR       = 0x100,
    MST_LASTLETTER_S    = 0x200,
    MST_LASTLETTER_S_DE = 0x400,
    MST_SISTER          = 0x800,
    MST_PLRNOUN         = 0x1000,
    MST_ACTTGT          = 0x2000,
    MST_LEADER          = 0x4000,
    MST_PLUS            = 0x8000,
    MST_TALKER          = 0x10000,
    MST_ALLMEMBER       = 0x20000
};

struct MsgVar {
    int m_def;                  /* 0x00 */
    int m_array_index_no;       /* 0x04 */
    int m_type;                 /* 0x08 */
    int m_no;                   /* 0x0C */
    int m_opt;                  /* 0x10 */
    int m_fake;                 /* 0x14 */
    MACRO_STAT m_macro_stat;    /* 0x18 */

    void set(int def, int array_index_no, int type, int no, int fake, int opt);
    void set(int def, int array_index_no, int type, int no, int opt);
    int unkfunc_02053b2c(int def, int array_index_no);
    int extract_var(char* dst, int size, int ex);
    int unkfunc_02053d04();
};

struct TextEnv {
    MsgVar m_msg_var[64];           /* 0x000 */
    int m_msg_var_length;           /* 0x700 */
    TextHookBase* m_text_hook;      /* 0x704 */

    TextEnv();
    void resetMacro();
    void add_msg_var(int def, int array_index_no, int type, int no);
    void add_msg_var(int def, int array_index_no, int type, int no, int opt);
    void add_msg_var(int def, int array_index_no, int type, int no, int fake, int opt);
    void process_msg(char* dst, int size, const char* src);
    void unkfunc_02053e04(char* dst, int size, unsigned char* src);
    MsgVar* search_msg_var(int def, int array_index_no);
    char* check_text_hook(int def, int array_index_no);
    MACRO_STAT unkfunc_02053f74(int def, int array_index_no);
    char* unkfunc_02053f90(int def, int array_index_no);
    char* unkfunc_02054010(char* dst, int size, unsigned char* src, int cap);
    MACRO_STAT macro_getMacroStat(int def, int array_index_no);
    MACRO_STAT macro_checkActorTarget();
    MACRO_STAT macro_checkVowel(char* text);
    MACRO_STAT macro_checkLastS(char* text);
};

struct TextExtractor {
    char m_hero_name[32];           /* 0x000 */
    char m_user_str[8][64];         /* 0x020 */
    MACRO_STAT m_macro_stat;        /* 0x220 */

    TextExtractor();
    int extractText(char* dst, int size, int msg_id);
    int extractText(char* dst, int size, int type, int no);
    int extract_text(char* dst, int size, int type, int no, int ex, int plural);
    int extract_text_msg(char* dst, int size, unsigned int msg_id, int ex, int plural);
    void setHeroName(char* name);
    int extract_text_user_str(char* dst, int size, int no);
    void setUserString(int num, char* name);
    int extract_text_number(char* dst, int size, int no);
    const char* unkfunc_02055280(int type);
    void unkfunc_020553c4(char* dst, const unsigned char* src, int number, int art, int casus);
    const char* unkfunc_02055438(int number, int art, int casus, int c);
};

struct GrammarSuffix {
    int ch;
    int art;
    const char* str[4];
};

struct MsgData {
    char* m_addr;                   /* 0x00 */
    int m_size;                     /* 0x04 */
    int m_msg_base_id;              /* 0x08 */
    int unk_0c;                     /* 0x0C */
    int unk_10;                     /* 0x10 */
    int unk_14;                     /* 0x14 */
    int unk_18;                     /* 0x18 */

    MsgData();
    void msg_setup(int msg_base_id, int arg1, int size, int count);
    void unkfunc_020554ec();
    int unkfunc_02055510(unsigned int msg_id);
    int unkfunc_02055524(unsigned int msg_id, int lang);
};

struct MsgMeta {
    unsigned char m_meta[10][0x80];  /* 0x000 */
    unsigned char m_head[0x80];      /* 0x500 */
    unsigned char m_body[0x200];     /* 0x580 */
    unsigned char m_sound[0x80];     /* 0x780 */

    int unkfunc_02055604(const unsigned char* src, int msg_id, int lang);
    unsigned char* unkfunc_02055740(int index);
};

struct MsgFile {
    unsigned char* m_addr;          /* 0x000 */
    int m_size;                     /* 0x004 */
    MsgMeta m_meta;                 /* 0x008 */
    int m_msg_id;                   /* 0x808 */
    int unk_80c;                    /* 0x80C */

    void unkfunc_020557cc();
    void unkfunc_020557f8();
    int unkfunc_02055810(unsigned int msg_id, int lang);
    int unkfunc_02055848(unsigned int msg_id, int lang);
    int unkfunc_02055894(unsigned int msg_id, int lang);
    unsigned char* unkfunc_020558cc(int index);
};

struct BadWordList {
    DataObject m_data;              /* 0x00 */

    void unkfunc_02056b00();
    void unkfunc_02056b48();
    int unkfunc_02056b50(char* name);
    int unkfunc_02056c1c(char* word);
};

struct MessageMacro {
    int stateStackPos_;             /* 0x00 */
    int stateStack_[16];            /* 0x04 */
    int stateNow_;                  /* 0x44 */
    char* dst_;                     /* 0x48 */
    int size_;                      /* 0x4C */
    const char* src_;               /* 0x50 */

    void initialize(char* dst, int size, const char* src);
    void processMessage(char* dst, int size, const char* src);
    void judgeState(MACRO_STAT mask, int def, int array_index_no);
    void processIF();
    void processELSE();
    void processENDIF();
    void stateStackPush(int state) {
        stateStackPos_++;
        stateStack_[stateStackPos_] = state;
    }
};

struct MacroName {
    int id;
    const char* name;
};

MsgData* unkfunc_0205488c(int msg_id);
int unkfunc_02054970(int msg_id, int lang);
int unkfunc_02054984(int msg_id, int lang);
char* msg_get_head();
char* msg_get_body();
unsigned char msg_get_sound();
int CheckBadWord(char* name);
int unkfunc_020549f0(char* name);

extern MsgData g_msg_data_default;
extern MsgData g_msg_data[11];
extern TextEnv g_text_env;
extern TextExtractor g_text_extractor;
extern BadWordList g_bad_word_list;
extern MsgFile g_msg_file;

extern int data_021098c4;

