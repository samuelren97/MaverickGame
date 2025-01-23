#include "config.h"
#include "Bouton.h"

#define APPUYEE 0x0
#define RELACHEE 0x1
#define DELAIS_STABILISATION 25

Bouton::Bouton(uint8_t p_pin)
    : m_pin(p_pin),
      m_actionBoutonRelachee(nullptr),
      m_actionBoutonAppuyee(nullptr),
      m_etatPrecedent(RELACHEE),
      m_etatPrecedentStable(RELACHEE),
      m_dateDernierChangementEtat(0)
{
    pinMode(m_pin, INPUT);
}

void Bouton::tick()
{
    unsigned long dateCourante = millis();
    uint8_t etatCourant = digitalRead(m_pin);

    if (etatCourant != m_etatPrecedent)
    {
        m_dateDernierChangementEtat = dateCourante;
        this->m_etatPrecedent = etatCourant;
    }

    if (dateCourante - m_dateDernierChangementEtat > DELAIS_STABILISATION)
    {
        if (etatCourant == RELACHEE && m_etatPrecedentStable == APPUYEE)
        {
            this->executerActionBoutonRelachee();
        }
        else if (etatCourant == APPUYEE && m_etatPrecedentStable == RELACHEE)
        {
            this->executerActionBoutonAppuyee();
        }

        this->m_etatPrecedentStable = etatCourant;
    }
}

void Bouton::actionBoutonRelachee(Action *p_actionBoutonRelachee)
{
    this->m_actionBoutonRelachee = p_actionBoutonRelachee;
}

void Bouton::actionBoutonAppuyee(Action *p_actionBoutonAppuyee)
{
    this->m_actionBoutonAppuyee = p_actionBoutonAppuyee;
}

void Bouton::executerActionBoutonRelachee()
{
    if (this->m_actionBoutonRelachee != nullptr)
    {
        this->m_actionBoutonRelachee->executer();
    }
}

void Bouton::executerActionBoutonAppuyee()
{
    if (this->m_actionBoutonAppuyee != nullptr)
    {
        this->m_actionBoutonAppuyee->executer();
    }
}
