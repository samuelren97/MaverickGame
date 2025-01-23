#include "Affichage4DigitsProxyTM1637.h"

Affichage4DigitsProxyTM1637::Affichage4DigitsProxyTM1637(const uint8_t& p_pinHorloge, const uint8_t& p_pinDonnees)
: m_tm1637(new TM1637Display(p_pinHorloge, p_pinDonnees))
{
    this->m_tm1637->setBrightness(7);
}

void Affichage4DigitsProxyTM1637::afficher(const uint8_t& p_d1, const uint8_t& p_d2, const uint8_t& p_d3, const uint8_t& p_d4) {
    uint8_t segments[4] = {
        p_d1, p_d2, p_d3, p_d4
    };
    this->m_tm1637->setSegments(segments, 4);
}