#pragma once
#include <Arduino.h>
#include "Tache.h"

class DEL : public Tache
{
public:
    DEL(uint8_t p_pin);
    DEL(uint8_t p_pin, float p_dutyCycle);
    void allumer(float p_dutyCycle);
    void allumer();
    void eteindre();
    void clignoter();
    void basculerClignotement();
    void activerClignoter();
    void cesserClignoter();
    bool estClignotante();
    void ajusterIntervals(unsigned long p_dureeAllumee, unsigned long p_dureeEteinte);
    void tick();
    int etat() const;

private:
    uint8_t m_pin;
    uint8_t m_intensite;
    unsigned long m_dureeAllumee;
    unsigned long m_dureeEteinte;
    unsigned long m_dateDernierChangementEtat;
    float m_dutyCycle;
    uint8_t m_estClignotant;
    uint8_t m_etatActuel;
};