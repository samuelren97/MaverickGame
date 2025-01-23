
#pragma once

#include <Arduino.h>

#include "Action.h"
#include "LogiqueJeu.h"

class ActionInput: public Action {
public:
    ActionInput(LogiqueJeu* p_logiqueJeu, const char* p_codeInput);

    virtual void executer();

private:
    LogiqueJeu* m_logiqueJeu;
    const char* m_codeInput;
};