#pragma ipa file
#include "main/menu/MessageWindow.hpp"
#ifdef DQIV_NATIVE
#include "Settings.hpp"
#endif
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/UnkMenuTextDisplay.hpp"
#include "main/menu/UnkMenuWindowFrame.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/sound/Sound.hpp"
#include <nitro/std.h>
#include "main/menu/UnkMenuDisplays.hpp"

THUMB MessageWindow::MessageWindow()
{
    windowType_ = 0;
    cursor_ = 0;
    soundSpeed_ = 25;
}

THUMB void MessageWindow::setup(int id)
{
    id_ = id;
    unkfunc_0204f1ac();
    unkfunc_0204f260(2);
    state_ = 0;
    shake_ = 0;
    shakeCount_ = 0;
    shakeX_ = 0;
    shakeY_ = 0;
    keyWait_ = 0;
    keySound_ = 0;
}

THUMB void MessageWindow::update(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    if (shake_) {
        shakeX_ = data_020c1e54[shakeCount_ / 2];
        shakeY_ = data_020c1e74[shakeCount_ / 2];
        shakeCount_++;
        if (shakeCount_ == 16) {
            shake_ = 0;
        }
    } else {
        shakeX_ = 0;
        shakeY_ = 0;
    }
}

THUMB void MessageWindow::execute(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    unkfunc_0204d7c4();
}

THUMB void MessageWindow::draw(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    if (!enable_) {
        unkfunc_0208174c(0);
    }
    if (!unkfunc_0204f214(main, sub) || state_ == 0) {
        return;
    }
    frame_->unkfunc_0204f270(x_ + shakeX_, y_ + shakeY_);
    frame_->unkfunc_0204f264(1);
    if (namePlate_) {
        namePlate_->unkfunc_0204f270(shakeX_ + (x_ + config_->margin_), shakeY_ + (y_ + config_->margin_ - config_->nameY_) - 4);
        namePlate_->unkfunc_0204e584(-1);
        namePlate_->unkfunc_0204f264(1);
    }
    for (int i = 0; i < lineCount_; i++) {
        UnkMenuTextDisplay* line = lines_[i];
        if (line) {
            if (config_->frameType_ == 1) {
                line->unkfunc_0204e668(1);
            } else {
                line->unkfunc_0204e668(0);
            }
            line->unkfunc_0204f270(config_->margin_ + (x_ + shakeX_), config_->margin_ + (y_ + shakeY_) + i * lineHeight_ - scroll_);
            line->unkfunc_0204f264(1);
        }
    }
    int type;
    switch (unkfunc_020817bc()) {
    case 1:
        type = 0x14;
        break;
    case 2:
        type = 0x11;
        break;
    case 3:
        type = 0x11;
        break;
    default:
        type = 0;
        break;
    }
    if (type) {
        unkfunc_0208174c(1);
        unkfunc_0208176c(type, x_ + 2 + shakeX_, y_ - 0xbe + shakeY_, width_ - 4, height_ - 4);
    }
}

THUMB void MessageWindow::unkfunc_0204d3a4(const char* name)
{
    char* buf = name_;
    dss::strlen(name);
    MI_CpuFill(0, buf, sizeof(name_));
    STD_CopyString(buf, name);
}

THUMB void MessageWindow::unkfunc_0204d3c8(const char* text)
{
    text_[0] = 0;
    unkfunc_0204d3d8(text);
}

THUMB void MessageWindow::unkfunc_0204d3d8(const char* text)
{
    char* buf = text_;
    dss::strlen(text);
    int length = dss::strlen(buf);
    int maxWidth = config_->width_ - config_->margin_ * 2;
    int x = 0;
    unkfunc_02080038(config_->font_);
    Utf8Iterator src;
    src.unkfunc_020875ec(text);
    Utf8Iterator dst;
    dst.unkfunc_02087610(buf + length, sizeof(text_));
    unsigned short c = src.unkfunc_0208771c();
    int wordPos = -1;
    int wordLength = -1;
    int line = 0;
    while (c != 0) {
        c = src.unkfunc_0208771c();
        src.unkfunc_020877b8();
        if (c == 0x1b) {
            dst.unkfunc_02087734(c);
            c = src.unkfunc_0208771c();
            src.unkfunc_020877b8();
            dst.unkfunc_02087734(c);
            c = src.unkfunc_0208771c();
            src.unkfunc_020877b8();
        }
        if (c == 0xd) {
            dst.unkfunc_02087734(c);
            c = src.unkfunc_0208771c();
            src.unkfunc_020877b8();
        }
        if (c == 0x3221) {
            src.unkfunc_020877b8();
            int count = lineCount_ - line;
            if (line == 0 && x == 0) {
                count = 0;
            }
            for (; count != 0; count--) {
                dst.unkfunc_02087734('\n');
                line++;
            }
            x = 0;
        } else if (c != '\n') {
            int width = unkfunc_0207fc88(NULL, 0, 0, 0, c);
            dst.unkfunc_02087734(c);
            if (c == ' ' || c == '-' || c == 0x3220) {
                wordPos = src.pos_;
                wordLength = dst.unkfunc_020876a8();
            }
            x += width;
            if (x > maxWidth) {
                if (wordPos != -1) {
                    src.pos_ = wordPos;
                    wordPos = -1;
                    dst.unkfunc_020876dc(wordLength);
                    wordLength = -1;
                }
                dst.unkfunc_02087734('\n');
                x = 0;
                line++;
            }
        } else {
            dst.unkfunc_02087734(c);
            x = 0;
            line++;
        }
        if (line == lineCount_ && src.unkfunc_0208771c() != 0) {
            dst.unkfunc_02087734(0x328a);
            line = 0;
        }
    }
    dst.unkfunc_02087734(0);
}
int data_020c1e74[8] = { -2, 0, 0, 0, -2, 0, 0, 0 };
int data_020c1e54[8] = { 0, 0, -2, 0, 0, 0, -2, 0 };
static MessageWindowConfig s_config[8] = {
    { 0, 0, 0x138, 128, 72, 8, 16, 16, 3, 8, 1, 0, 12, 12, 4, 2, 1, 1, 0, 0, 0 },
    { 1, 0, 0x138, 256, 72, 8, 16, 16, 3, 8, 1, 0, 12, 12, 4, 2, 1, 1, 0, 0, 1 },
    { 2, 0, 0x138, 256, 72, 8, 16, 16, 3, 8, 1, 0, 12, 12, 4, 2, 1, 1, 0, 0, 1 },
    { 3, 0, 0x138, 256, 72, 8, 0, 0, 4, 4, 1, 0, 10, 10, 4, 2, 1, 100, 30, 40, 0 },
    { 4, 0, 0x140, 256, 70, 8, 0, 0, 2, 8, 1, 1, 10, 10, 4, 2, 1, 100, 0, 0, 0 },
    { 5, 0, 0x138, 256, 72, 8, 0, 0, 4, 4, 1, 0, 12, 12, 2, 2, 1, 100, 30, 40, 0 },
    { 6, 0, 0x140, 256, 70, 8, 0, 0, 2, 8, 1, 1, 12, 12, 2, 2, 1, 100, 0, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 10, 0, 0, 0, 0, 1, 0, 0, 0 },
};

THUMB void MessageWindow::unkfunc_0204d578(int type)
{
    if (type == 3) {
        type = 5;
    }
    if (type == 4) {
        type = 6;
    }
    MessageWindowConfig* config = s_config;
    while (config->type_ != -1) {
        if (config->type_ == type) {
            break;
        }
        config++;
    }
    windowType_ = type;
    config_ = config;
    lineCount_ = config->lineCount_;
}

THUMB void MessageWindow::unkfunc_0204d5b4()
{
    lineCount_ = config_->lineCount_;
    const char* name = name_;
    if (lineCount_ == 4) {
        name = NULL;
    }
    lineHeight_ = config_->fontHeight_ + config_->lineSpace_;
    if (config_->nameX_ == 0) {
        name = NULL;
    }
    int offset;
    if (name) {
        namePlate_ = &data_020f530c.texts_[3];
        namePlate_->unkfunc_0204e430(config_->font_, name, 0, 0, 1);
        offset = config_->nameX_ + namePlate_->unkfunc_0204e5e0();
    } else {
        offset = 0;
        namePlate_ = NULL;
    }
    unkfunc_0204dcc0();
    frame_ = &data_020f530c.windowFrame1_;
    frame_->unkfunc_0204f800(offset);
    frame_->unkfunc_0204f270(x_, y_);
    frame_->unkfunc_0204f828(config_->frameType_);
    frame_->unkfunc_0204f80c(0);
    width_ = config_->width_;
    int height = config_->margin_ * 2 - config_->lineSpace_ + lineHeight_ * lineCount_;
    height += config_->unk_24;
    height_ = height;
    frame_->unkfunc_0204f278(width_, height_);
    waitLine_ = NULL;
    scroll_ = 0;
    scrollSpeed_ = config_->scrollSpeed_;
#ifdef DQIV_NATIVE
    if (pc::gameSettings().textSpeed == pc::textFast) {
        scrollSpeed_ *= 2;
    } else if (pc::gameSettings().textSpeed == pc::textInstant) {
        scrollSpeed_ = lineHeight_;
    }
#endif
    unkfunc_0204f270(config_->x_, config_->y_);
    intervalCursor_ = config_->intervalCursor_ ? 1 : 0;
    lastCursor_ = config_->lastCursor_ ? 1 : 0;
    iterator_.unkfunc_020875ec(text_);
    state_ = 1;
    shake_ = 0;
    shakeCount_ = 0;
    shakeX_ = 0;
    shakeY_ = 0;
    keyWait_ = 0;
    keySound_ = 0;
    unk_9cc = 0;
    if (config_->sound_ == 0) {
        sound_ = -1;
    } else {
        sound_ = 0;
    }
}

THUMB void MessageWindow::unkfunc_0204d70c()
{
    lineCount_ = config_->lineCount_;
    const char* name = name_;
    if (lineCount_ == 4) {
        name = NULL;
    }
    lineHeight_ = config_->fontHeight_ + config_->lineSpace_;
    if (config_->nameX_ == 0) {
        name = NULL;
    }
    int offset;
    if (name) {
        namePlate_ = &data_020f530c.texts_[3];
        namePlate_->unkfunc_0204e430(config_->font_, name, 0, 0, 1);
        offset = config_->nameX_ + namePlate_->unkfunc_0204e5e0();
    } else {
        offset = 0;
        namePlate_ = NULL;
    }
    frame_ = &data_020f530c.windowFrame1_;
    frame_->unkfunc_0204f800(offset);
    frame_->unkfunc_0204f270(x_, y_);
    frame_->unkfunc_0204f828(config_->frameType_);
    frame_->unkfunc_0204f80c(0);
    width_ = config_->width_;
    int height = config_->margin_ * 2 - config_->lineSpace_ + lineHeight_ * lineCount_;
    height += config_->unk_24;
    height_ = height;
    frame_->unkfunc_0204f278(width_, height_);
    unkfunc_0204f270(config_->x_, config_->y_);
}

THUMB void MessageWindow::unkfunc_0204d7c4()
{
    switch (state_) {
    case 0:
        unkfunc_0204d818();
        break;
    case 1:
        unkfunc_0204d81c();
        break;
    case 2:
        unkfunc_0204da24();
        break;
    case 3:
        unkfunc_0204dab4();
        break;
    case 4:
        unkfunc_0204db14();
        break;
    case 5:
        unkfunc_0204dc10();
        break;
    case 6:
        unkfunc_0204dc34();
        break;
    case 7:
        break;
    }
}

THUMB void MessageWindow::unkfunc_0204d818()
{
}

THUMB void MessageWindow::unkfunc_0204d81c()
{
    char buf[0x800];
    int c = getIterator()->unkfunc_0208771c();
    if (unkfunc_0204dda4(c)) {
        state_ = 4;
        unk_9c0 = -1;
        return;
    }
    if (c == 0x1b) {
        getIterator()->unkfunc_020877b8();
        c = getIterator()->unkfunc_0208771c();
        getIterator()->unkfunc_020877b8();
        if (sound_ != -1) {
            if (c == '0') {
                sound_ = 0;
            }
            if (c == '1') {
                sound_ = 0x12d;
            }
            if (c == '2') {
                sound_ = 0x12e;
            }
            if (c == '3') {
                sound_ = 0x12f;
            }
        }
        if (c == 'A') {
            unkfunc_0204df48();
        }
        return;
    }
    if (c == 0x328a) {
        state_ = 4;
        unk_9c0 = -1;
        getIterator()->unkfunc_020877b8();
        if (getIterator()->unkfunc_0208771c()) {
            getIterator()->unkfunc_02087800();
        }
        return;
    }
    if (c == 0x328d) {
        getIterator()->unkfunc_020877b8();
        state_ = 5;
        unkfunc_0204e064(1);
        return;
    }
    UnkMenuTextDisplay* line = unkfunc_0204dcf0();
    if (line == NULL) {
        state_ = 3;
        return;
    }
    if (waitLine_ == NULL) {
        waitLine_ = line;
    }
    char* start = iterator_.buf_ + iterator_.pos_;
    char* end;
    while (true) {
        c = getIterator()->unkfunc_0208771c();
        end = iterator_.buf_ + iterator_.pos_;
        if (unkfunc_0204dda4(c) || unkfunc_0204ddc4(c)) {
            break;
        }
        if (unkfunc_0204ddb0(c)) {
            unkfunc_0204ddf4(getIterator());
            break;
        }
        getIterator()->unkfunc_020877b8();
    }
    int len = end - start;
    MI_CpuCopyU8(start, buf, len);
    buf[len] = 0;
    line->unkfunc_0204e430(config_->font_, buf, 0, 0, 0);
    soundTimer_ = 100;
    state_ = 2;
}

THUMB void MessageWindow::unkfunc_0204da24()
{
    UnkMenuTextDisplay* line = unkfunc_0204dd3c();
    if (line->unkfunc_0204e5d0() != line->unkfunc_0204e5d8()) {
#ifdef DQIV_NATIVE
        // The player's text speed, for text shown letter by letter.
        int printSpeed = config_->printSpeed_;
        if (printSpeed < 100) {
            static int s_slowFrame;
            switch (pc::gameSettings().textSpeed) {
            case pc::textSlow:
                if (++s_slowFrame & 1) {
                    return;
                }
                break;
            case pc::textFast:
                printSpeed *= 3;
                break;
            case pc::textInstant:
                printSpeed = line->unkfunc_0204e5d8();
                break;
            }
        }
        line->unkfunc_0204e584(printSpeed + line->unkfunc_0204e5d0());
#else
        line->unkfunc_0204e584(config_->printSpeed_ + line->unkfunc_0204e5d0());
#endif
        wait_ = config_->wait_;
        unk_9c8 = config_->unk_4c;
        if (sound_ != 0 && sound_ != -1) {
            if (soundTimer_ > 100) {
                Sound::sePlay(sound_);
                soundTimer_ -= 100;
            }
            soundTimer_ += soundSpeed_;
        }
        return;
    }
    if (wait_ == 0) {
        state_ = 1;
        return;
    }
    wait_--;
}

THUMB void MessageWindow::unkfunc_0204dab4()
{
    scroll_ += scrollSpeed_;
    if (scroll_ > lineHeight_) {
        scroll_ = lineHeight_;
    }
    unkfunc_0204dd38()->unkfunc_0204e5e8(scroll_);
    if (scroll_ == lineHeight_) {
        if (waitLine_ == unkfunc_0204dd38()) {
            waitLine_ = NULL;
        }
        unkfunc_0204dd60();
        scroll_ = 0;
        state_ = 1;
    }
}

THUMB void MessageWindow::unkfunc_0204db14()
{
    if (waitLine_ != unkfunc_0204dd38()) {
        unkfunc_0204dab4();
        state_ = 4;
        return;
    }
    if (unk_9c8) {
        unk_9c8--;
        return;
    }
    if (unkfunc_0204dda4(getIterator()->unkfunc_0208771c())) {
        keyWait_ = 0;
        keySound_ = 0;
        state_ = 6;
        return;
    }
    if (unk_9c0 == 0) {
        getIterator()->unkfunc_020877b8();
        unkfunc_0204ddf4(getIterator());
        frame_->unkfunc_0204f80c(0);
        waitLine_ = NULL;
        keyWait_ = 0;
        keySound_ = 0;
        state_ = 1;
        return;
    }
    if (intervalCursor_) {
        frame_->unkfunc_0204f80c(1);
    }
    if (unk_9c0 != -1) {
        unk_9c0--;
    }
    if (keyWait_ == 1) {
        keyWait_ = 0;
        unk_9c0 = 0;
        if (keySound_ == 1) {
            Sound::sePlay(300);
        }
        keySound_ = 0;
    }
}

THUMB void MessageWindow::unkfunc_0204dc10()
{
    if (unk_9c8) {
        unk_9c8--;
        return;
    }
    if (unk_9cc != 1) {
        state_ = 1;
    }
}

THUMB void MessageWindow::unkfunc_0204dc34()
{
    if (cursor_ == 0 && lastCursor_) {
        frame_->unkfunc_0204f80c(1);
    }
    if (cursor_ == 1) {
        frame_->unkfunc_0204f80c(1);
    }
    if (cursor_ == -1) {
        frame_->unkfunc_0204f80c(0);
    }
    if (keyWait_ == 1) {
        keyWait_ = 0;
        state_ = 7;
        sound_ = -1;
        if (lastCursor_ == 1 && keySound_ == 1) {
            Sound::sePlay(300);
        }
        keySound_ = 0;
    }
}

THUMB void MessageWindow::unkfunc_0204dcc0()
{
    for (int i = 0; i < lineCount_; i++) {
        lines_[i] = NULL;
        freeLines_[i] = &data_020f530c.texts_[i];
    }
}

THUMB UnkMenuTextDisplay* MessageWindow::unkfunc_0204dcf0()
{
    for (int i = 0; i < lineCount_; i++) {
        if (freeLines_[i]) {
            UnkMenuTextDisplay* line = freeLines_[i];
            freeLines_[i] = NULL;
            for (int j = 0; j < lineCount_; j++) {
                if (lines_[j] == NULL) {
                    lines_[j] = line;
                    return line;
                }
            }
        }
    }
    return NULL;
}

THUMB UnkMenuTextDisplay* MessageWindow::unkfunc_0204dd38()
{
    return lines_[0];
}

THUMB UnkMenuTextDisplay* MessageWindow::unkfunc_0204dd3c()
{
    UnkMenuTextDisplay* last = NULL;
    for (int i = 0; i < lineCount_; i++) {
        if (lines_[i]) {
            last = lines_[i];
        }
    }
    return last;
}

THUMB void MessageWindow::unkfunc_0204dd60()
{
    UnkMenuTextDisplay* first = lines_[0];
    for (int i = 1; i < lineCount_; i++) {
        lines_[i - 1] = lines_[i];
        lines_[i] = NULL;
    }
    for (int i = 0; i < lineCount_; i++) {
        if (freeLines_[i] == NULL) {
            freeLines_[i] = first;
            return;
        }
    }
}

THUMB int MessageWindow::unkfunc_0204dda4(int c)
{
    if (c == 0) {
        return 1;
    }
    return 0;
}

THUMB int MessageWindow::unkfunc_0204ddb0(int c)
{
    if (c == '\r') {
        return 1;
    }
    if (c == '\n') {
        return 1;
    }
    return 0;
}

THUMB int MessageWindow::unkfunc_0204ddc4(int c)
{
    switch (c) {
    case 0x328a:
    case 0x328b:
    case 0x328c:
    case 0x328d:
    case 0x328e:
    case 0x328f:
    case 0x3290:
        return 1;
    }
    return 0;
}

THUMB void MessageWindow::unkfunc_0204ddf4(UnkTextIterator* it)
{
    while (unkfunc_0204ddb0(it->unkfunc_0208771c())) {
        it->unkfunc_020877b8();
    }
}

THUMB void MessageWindow::unkfunc_0204de2c(int type, const char* name, const char* text)
{
    unkfunc_0204d578(type);
    unkfunc_0204d3a4(name);
    unkfunc_0204d3c8(text);
    unkfunc_0204d5b4();
}

THUMB void MessageWindow::unkfunc_0204de50(const char* text)
{
    unkfunc_0204d3d8(text);
    iterator_.unkfunc_020875ec(text_);
}

THUMB void MessageWindow::unkfunc_0204de6c()
{
    unkfunc_0204de8c(0x328a);
}

THUMB void MessageWindow::unkfunc_0204de7c()
{
    unkfunc_0204de8c(0x328d);
}

THUMB void MessageWindow::unkfunc_0204de8c(int code)
{
    const char* text;
    switch (code) {
    case 0x328a:
        text = "\xe3\x8a\x8a";
        break;
    case 0x328b:
        text = "\xe3\x8a\x8b";
        break;
    case 0x328c:
        text = "\xe3\x8a\x8c";
        break;
    case 0x328d:
        text = "\xe3\x8a\x8d";
        break;
    case 0x328e:
        text = "\xe3\x8a\x8e";
        break;
    case 0x328f:
        text = "\xe3\x8a\x8f";
        break;
    case 0x3290:
        text = "\xe3\x8a\x90";
        break;
    default:
        return;
    }
    unkfunc_0204d3d8(text);
}

THUMB void MessageWindow::unkfunc_0204def4()
{
    nameIndex_ = -1;
    nameCount_ = 0;
    for (int i = 0; i < 32; i++) {
        names_[i][0] = 0;
    }
}

THUMB void MessageWindow::unkfunc_0204df1c(const char* name)
{
    if (nameCount_ != 32) {
        dss::strcpy_s(names_[nameCount_], 0x80, name);
        nameCount_++;
    }
}

THUMB void MessageWindow::unkfunc_0204df48()
{
    const char* prev = unkfunc_0204df94();
    nameIndex_++;
    const char* next = unkfunc_0204df94();
    if (prev == NULL) {
        prev = "";
    }
    if (next == NULL) {
        next = "";
    }
    if (dss::strcmp(prev, next) != 0) {
        unkfunc_0204d3a4(next);
        unkfunc_0204d70c();
    }
}

THUMB const char* MessageWindow::unkfunc_0204df94()
{
    if (nameIndex_ == -1) {
        return NULL;
    }
    if (nameIndex_ >= nameCount_) {
        return NULL;
    }
    return names_[nameIndex_];
}

THUMB void MessageWindow::unkfunc_0204dfc0()
{
    state_ = 1;
    waitLine_ = NULL;
    text_[0] = 0;
}

THUMB int MessageWindow::unkfunc_0204dfd8()
{
    if (state_ != 4) {
        return 0;
    }
    if (waitLine_ == unkfunc_0204dd38()) {
        return 1;
    }
    return 0;
}

THUMB int MessageWindow::unkfunc_0204e004()
{
    if (state_ == 6) {
        return 1;
    }
    return 0;
}

THUMB int MessageWindow::unkfunc_0204e018()
{
    if (state_ == 7) {
        return 1;
    }
    return 0;
}

THUMB bool MessageWindow::unkfunc_0204e02c()
{
    if (state_ == 5) {
        return 1;
    }
    return 0;
}

THUMB void MessageWindow::unkfunc_0204e040()
{
    keyWait_ = 1;
    keySound_ = 1;
}

THUMB void MessageWindow::unkfunc_0204e050()
{
    keyWait_ = 1;
    keySound_ = 0;
}

THUMB void MessageWindow::unkfunc_0204e064(int flag)
{
    unk_9cc = flag;
}
