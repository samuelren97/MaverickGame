#include <Arduino.h>

#include "DEL.h"

#define INTENSITE_MIN_PCT 0.00f
#define INTENSITE_MAX_PCT 100.00f
#define PAS_INTENSITE 25.0f
#define ALLUME 0x1
#define ETEINT 0x0
#define DESACTIVEE 0x0
#define ACTIVEE 0x1

DEL::DEL(uint8_t p_pin) : DEL(p_pin, INTENSITE_MAX_PCT)
{
    ;
}

DEL::DEL(uint8_t p_pin, float p_dutyCycle)
{
    this->m_pin = p_pin;
    this->m_intensite = map(p_dutyCycle, INTENSITE_MIN_PCT, INTENSITE_MAX_PCT, 0, 255);
    this->m_dureeAllumee = 300;
    this->m_dureeEteinte = 300;
    this->m_dateDernierChangementEtat = 0;
    this->m_estClignotant = DESACTIVEE;
    this->m_etatActuel = ETEINT;
    pinMode(this->m_pin, OUTPUT);
}

void DEL::allumer(float p_dutyCycle)
{
    this->m_intensite = map(p_dutyCycle, INTENSITE_MIN_PCT, INTENSITE_MAX_PCT, 0, 255);
    allumer();
}

void DEL::allumer()
{
    analogWrite(this->m_pin, this->m_intensite);
    this->m_etatActuel = ALLUME;
}

void DEL::eteindre()
{
    analogWrite(this->m_pin, 0);
    this->m_etatActuel = ETEINT;
}

void DEL::clignoter()
{
    unsigned long dateCourante = millis();
    unsigned long dureeDepuisChangement = dateCourante - this->m_dateDernierChangementEtat;

    if (this->m_etatActuel == ALLUME && dureeDepuisChangement > this->m_dureeAllumee)
    {
        this->eteindre();
        this->m_dateDernierChangementEtat = dateCourante;
    }
    else if (this->m_etatActuel == ETEINT && dureeDepuisChangement > this->m_dureeEteinte)
    {
        this->allumer();
        this->m_dateDernierChangementEtat = dateCourante;
    }
}

void DEL::basculerClignotement() {
    if (this->m_estClignotant == ACTIVEE) {
        cesserClignoter();
    } else {
        activerClignoter();
    }
}

void DEL::activerClignoter()
{
    this->m_estClignotant = ACTIVEE;
}

void DEL::cesserClignoter()
{
    this->m_estClignotant = DESACTIVEE;
    this->eteindre();
}

bool DEL::estClignotante() {
    return this->m_estClignotant == ACTIVEE;
}

void DEL::ajusterIntervals(unsigned long p_dureeAllumee, unsigned long p_dureeEteinte)
{
    this->m_dureeAllumee = p_dureeAllumee;
    this->m_dureeEteinte = p_dureeEteinte;
}

void DEL::tick()
{
    if (this->m_estClignotant == ACTIVEE)
    {
        this->clignoter();
    }
}

int DEL::etat() const {
    return this->m_etatActuel;
}