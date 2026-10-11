#include "main/status/UseActionMessage.hpp"


THUMB void status::UseActionMessage::clear()
{
    for (int i = 0; i < 2; i++) {
        this->beforeMessage_[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        this->execMessage_[i] = 0;
    }

    for (int i = 0; i < 2; i++) {
        this->spclMessage_[i] = 0;
    }

    for (int i = 0; i < 2; i++) {
        this->resultMessage_[i] = 0;
    }

    for (int i = 0; i < 2; i++) {
        this->addMessage_[i] = 0;
    }
}