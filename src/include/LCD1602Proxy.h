#pragma once

#include <Arduino.h>

class LCD1602Proxy {
public:
    virtual void definirPositionCurseur(const uint8_t& p_colonne, const uint8_t& p_ligne) const = 0;
    virtual void afficher(const String& p_texte) const = 0;
    virtual void afficherPlain(const String& p_texte) const = 0;
    virtual void effacer() const = 0;
};