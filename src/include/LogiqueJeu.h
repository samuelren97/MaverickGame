#pragma once

#define ETAT_PENALITE 1
#define ETAT_VICTOIRE 2

#define DELAIS_PENALITE 3000
#define DELAIS_ENVOI_DONNEES 5000
#define DELAIS_ANIMATION_4DIGITS 50

#include <Arduino.h>
#include <PubSubClient.h>
#include "Tache.h"
#include "Affichage4Digits.h"
#include "LCD1602.h"
#include "Buzzer.h"
#include "List.h"
#include "Niveau.h"
#include "DEL.h"

class LogiqueJeu: public Tache {
public:
    LogiqueJeu(LCD1602* p_lcd, PubSubClient* p_clientMQTT, DEL* p_del);
    void demarrerJeu();
    void gererInputs(const char* p_codeInput);
    void setNiveau(const Niveau& p_niveau);
    virtual void tick();
private:
    PubSubClient* m_clientMQTT;
    DEL* m_del;
    Affichage4Digits* m_4digits;
    LCD1602* m_lcd;
    Buzzer* m_buzzer;
    List<Tache*> m_taches;
    uint16_t m_countDownEnSecondes;
    uint16_t m_countDownInitial;
    unsigned long m_dateDernierChangementCountdown;
    unsigned long m_dateDernierChangementPenalite;
    unsigned long m_dateDernierEnvoiDeDonnees;
    bool m_estEnJeu;
    uint8_t m_dureePenaliteEnSecondes;
    char* m_sequenceATrouver;
    uint8_t m_indexSequence;
    uint8_t m_etat;
    uint8_t m_segmentAttente;
    String m_nomUnique;
    
    void mettreEnEtatPenalite(const uint8_t& p_dureePenaliteEnSecondes);
    void gererEtatPenalite(const unsigned long& p_dateActuelle);
    void gererEtatEnJeu(const unsigned long& p_dateActuelle);
    void gererEnvoiDesDonnees(const unsigned long& p_dateActuelle);
    void animer4Digits(const unsigned long& p_dateActuelle);
};