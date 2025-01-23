#pragma once

#include <Arduino.h>

#include "LCD1602Proxy.h"

class LCD1602 {
public:
    LCD1602(LCD1602Proxy* p_proxy);
    void definirPositionCurseur(const uint8_t& p_colonne, const uint8_t& p_ligne) const;
    void afficher(const String& p_texte) const;
    void afficherPlain(const String& p_texte) const;
    void effacer() const;
private:
    LCD1602Proxy* m_proxy;
};