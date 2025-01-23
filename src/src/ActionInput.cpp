#include "ActionInput.h"

ActionInput::ActionInput(LogiqueJeu* p_logiqueJeu, const char* p_codeInput)
: m_logiqueJeu(p_logiqueJeu),
m_codeInput(p_codeInput)
{
    ;
}

void ActionInput::executer() {
    this->m_logiqueJeu->gererInputs(this->m_codeInput);
}