#pragma once

#include <Arduino.h>

#include "Affichage4DigitsProxy.h"

class Affichage4Digits {
public:
    Affichage4Digits(Affichage4DigitsProxy* p_proxy);
    void afficher(const uint8_t& p_d1, const uint8_t& p_d2, const uint8_t& p_d3, const uint8_t& p_d4, const bool& p_estNombreEntier = true);
    void afficherEntier(const uint16_t& p_valeur, const uint8_t& p_base = 10);
    void afficherReel(const float& p_valeur);
private:
    Affichage4DigitsProxy* m_proxy;
    uint8_t valeurSegment(const uint8_t& p_valeur);
};