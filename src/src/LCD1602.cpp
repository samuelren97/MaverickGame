#include "LCD1602.h"

LCD1602::LCD1602(LCD1602Proxy* p_proxy)
: m_proxy(p_proxy) 
{
    ;
}

void LCD1602::definirPositionCurseur(const uint8_t& p_colonne, const uint8_t& p_ligne) const {
    this->m_proxy->definirPositionCurseur(p_colonne, p_ligne);
}

void LCD1602::afficher(const String& p_texte) const {
    this->m_proxy->afficher(p_texte);
}

void LCD1602::afficherPlain(const String& p_texte) const {
    this->m_proxy->afficherPlain(p_texte);
}

void LCD1602::effacer() const {
    this->m_proxy->effacer();
}