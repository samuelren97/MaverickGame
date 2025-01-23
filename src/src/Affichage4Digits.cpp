#include "Affichage4Digits.h"
#include <bitset>

const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001,    // F
  0b10000000     // .
  };

Affichage4Digits::Affichage4Digits(Affichage4DigitsProxy* p_proxy) 
: m_proxy(p_proxy)
{
    ;
}

void Affichage4Digits::afficher(const uint8_t& p_d1, const uint8_t& p_d2, const uint8_t& p_d3, const uint8_t& p_d4, const bool& p_estNombreEntier) {
    uint8_t d1 = p_d1;
    uint8_t d2 = p_d2;
    uint8_t d3 = p_d3;
    uint8_t d4 = p_d4;
    
    
    if (p_estNombreEntier) {
        d1 = valeurSegment(p_d1);
        d2 = valeurSegment(p_d2);
        d3 = valeurSegment(p_d3);
        d4 = valeurSegment(p_d4);
    }
    
    this->m_proxy->afficher(d1, d2, d3, d4);
}

void Affichage4Digits::afficherEntier(const uint16_t& p_valeur, const uint8_t& p_base) {
    uint8_t digits[4] = {0,0,0,0};
    uint16_t val = p_valeur;

    uint8_t i = 0;
    while (val != 0 && i < 4) {
        digits[i] = val % p_base;
        val /= p_base;
        ++i;
    }

    afficher(digits[3], digits[2], digits[1], digits[0]);
}

void Affichage4Digits::afficherReel(const float& p_valeur) {
    uint8_t digits[4] = {0,digitToSegment[16],0,0};
    float val = p_valeur;

    float partieEntiere;
    float partieFranctionnelle = modf(val, &partieEntiere);

    digits[0] = static_cast<uint8_t>(partieEntiere / 10);
    digits[1] = static_cast<uint8_t>(partieEntiere) % 10;
    digits[2] = static_cast<uint8_t>(partieFranctionnelle * 10) % 10;
    digits[3] = static_cast<uint8_t>(partieFranctionnelle * 100) % 10;

    this->m_proxy->afficher(
        valeurSegment(digits[0]),
        valeurSegment(digits[1]) | digitToSegment[16],
        valeurSegment(digits[2]),
        valeurSegment(digits[3])
    );
}

uint8_t Affichage4Digits::valeurSegment(const uint8_t& p_valeur) {
    return digitToSegment[p_valeur % 16];
}