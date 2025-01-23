#include "LCD1602ProxyI2C.h"
#include "config.h"

#define ADRESSE_LCD 0x27

LCD1602ProxyI2C::LCD1602ProxyI2C() 
: m_lcd(new LiquidCrystal_I2C(ADRESSE_LCD, 16, 2))
{
    this->m_lcd->begin(ADRESSE_LCD, 16, 2);
    this->m_lcd->init();
    this->m_lcd->backlight();
}

void LCD1602ProxyI2C::definirPositionCurseur(const uint8_t& p_colonne, const uint8_t& p_ligne) const {
    this->m_lcd->setCursor(p_colonne, p_ligne);
}

void LCD1602ProxyI2C::afficher(const String& p_texte) const {
    definirPositionCurseur(0,0);
    effacer();
    if(p_texte.length() > 16) {
        this->m_lcd->print(p_texte.substring(0, 16));

        definirPositionCurseur(0,1);
        this->m_lcd->print(p_texte.substring(16));
    }
    else {
        this->m_lcd->print(p_texte);
    }
}

void LCD1602ProxyI2C::afficherPlain(const String& p_texte) const {
    this->m_lcd->print(p_texte);
}

void LCD1602ProxyI2C::effacer() const {
    this->m_lcd->clear();
}