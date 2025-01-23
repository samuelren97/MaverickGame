#pragma once

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#include "LCD1602Proxy.h"

class LCD1602ProxyI2C : public LCD1602Proxy {
public:
    LCD1602ProxyI2C();
    virtual void definirPositionCurseur(const uint8_t& p_colonne, const uint8_t& p_ligne) const;
    virtual void afficher(const String& p_texte) const;
    virtual void afficherPlain(const String& p_texte) const;
    virtual void effacer() const;
private:
    LiquidCrystal_I2C* m_lcd;
};