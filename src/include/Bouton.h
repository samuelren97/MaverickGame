#pragma once

#include <Arduino.h>
#include "Action.h"
#include "Tache.h"

class Bouton : public Tache
{
public:
    Bouton(uint8_t p_pin);
    void actionBoutonRelachee(Action *p_actionBoutonRelachee);
    void actionBoutonAppuyee(Action *p_actionBoutonAppuyee);
    virtual void tick();

private:
    uint8_t m_pin;
    Action *m_actionBoutonRelachee = nullptr;
    Action *m_actionBoutonAppuyee = nullptr;
    uint8_t m_etatPrecedent;
    uint8_t m_etatPrecedentStable;
    unsigned long m_dateDernierChangementEtat = 0;

    void executerActionBoutonRelachee();
    void executerActionBoutonAppuyee();
};