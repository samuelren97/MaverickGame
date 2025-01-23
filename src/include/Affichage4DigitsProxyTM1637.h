#pragma once

#include <Arduino.h>
#include <TM1637Display.h>

#include "Affichage4DigitsProxy.h"

class Affichage4DigitsProxyTM1637 : public Affichage4DigitsProxy {
public:
    Affichage4DigitsProxyTM1637(const uint8_t& p_pinHorloge, const uint8_t& p_pinDonnees);
    virtual void afficher(const uint8_t& p_d1, const uint8_t& p_d2, const uint8_t& p_d3, const uint8_t& p_d4);
private:
    TM1637Display* m_tm1637;
};