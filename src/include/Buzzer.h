#pragma once

#include <Arduino.h>
#include "Tache.h"

#define ETAT_MANUEL 0x0
#define ETAT_COUNTDOWN 0x1
#define ETAT_BUZZ_VICTOIRE 0x2
#define ETAT_BUZZ_EXPLOSION 0x3
#define ETAT_BUZZ_PENALITE 0x4
#define FREQ_BASE 1500

class Buzzer: public Tache {
public:
    Buzzer(const uint8_t& p_pin, const uint16_t& p_dureeCountdown);
    Buzzer(const uint8_t& p_pin, const uint16_t& p_dureeCountdown, const unsigned long& p_dureeBuzz);
    void lancerCountdown();
    void lancerCountdown(const uint16_t& p_dureeCountdown);
    void arreterCountdown();
    void dureeBuzz(unsigned long& p_dureeBuzz);
    void buzz(const unsigned long& p_duree, const uint16_t& p_frequence);
    void buzzVictoire();
    void buzzExplosion();
    void buzzPenalite();
    virtual void tick();
private:
    uint8_t m_etat;
    unsigned long m_dateDernierChangementDureeCountdown;
    unsigned long m_dateDernierChangement;
    uint8_t m_pin;
    unsigned long m_dureeBuzz;
    uint16_t m_dureeDepuisDebutCountdown;
    bool m_estEnFonction;
    uint16_t m_dureeCountdownEnSecondes;
    uint8_t m_etapeMusicale;
    uint16_t m_frequence;
};