#pragma once

#include <Arduino.h>

class Affichage4DigitsProxy {
public:
    virtual void afficher(const uint8_t& p_d1, const uint8_t& p_d2, const uint8_t& p_3, const uint8_t& p_4) = 0;
};