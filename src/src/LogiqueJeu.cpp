#include "LogiqueJeu.h"
#include "Affichage4DigitsProxyTM1637.h"
#include "config.h"

LogiqueJeu::LogiqueJeu(LCD1602* p_lcd, PubSubClient* p_clientMQTT, DEL* p_del)
: m_clientMQTT(p_clientMQTT),
m_del(p_del),
m_4digits(new Affichage4Digits(new Affichage4DigitsProxyTM1637(PIN_CLK, PIN_DIO))),
m_lcd(p_lcd),
m_buzzer(new Buzzer(PIN_BUZZER, 11)),
m_taches(List<Tache*>(2)),
m_countDownEnSecondes(0),
m_countDownInitial(0),
m_dateDernierChangementCountdown(millis()),
m_dateDernierChangementPenalite(millis()),
m_dateDernierEnvoiDeDonnees(millis()),
m_estEnJeu(false),
m_dureePenaliteEnSecondes(10),
m_sequenceATrouver(nullptr),
m_indexSequence(0),
m_etat(0),
m_segmentAttente(1),
m_nomUnique("ESP32Client" + String(ESP.getEfuseMac(), HEX))
{
    this->m_taches.ajouter(this->m_buzzer);
    this->m_sequenceATrouver = new char[1];
    this->m_sequenceATrouver[0] = 0;
}

void LogiqueJeu::demarrerJeu() {
    this->m_estEnJeu = true;
    this->m_lcd->afficher("Desamorcez la   bombe!");
    this->m_del->cesserClignoter();
}

void LogiqueJeu::gererInputs(const char* p_codeInput) {
    Serial.println("[DEBUG] gererInputs.p_codeInput: " + String(p_codeInput));
    Serial.println("[DEBUG] gererInputs.m_sequenceATrouver: " + String(this->m_sequenceATrouver));
    if (this->m_estEnJeu && this->m_etat != ETAT_PENALITE) {
        if (this->m_sequenceATrouver[this->m_indexSequence] == *p_codeInput) {
            ++this->m_indexSequence;
            if (this->m_indexSequence == String(this->m_sequenceATrouver).length()) {
                this->m_estEnJeu = false;
                this->m_lcd->afficher("Bombe desamorce!Bravo!");
                this->m_buzzer->arreterCountdown();
                this->m_buzzer->buzzVictoire();
                this->m_indexSequence = 0;
            }
        } else {
            mettreEnEtatPenalite(this->m_dureePenaliteEnSecondes);
        }
    } else if (this->m_etat != ETAT_PENALITE){
        this->m_del->cesserClignoter();
        this->m_lcd->afficher("Maverick Bomb   SRAB Techs");
    }
}

void LogiqueJeu::setNiveau(const Niveau& p_niveau) {
    this->m_indexSequence = 0;
    if (this->m_sequenceATrouver) {
        delete [] this->m_sequenceATrouver;
    }
    this->m_sequenceATrouver = new char[strlen(p_niveau.sequenceATrouver) + 1];
    strcpy(this->m_sequenceATrouver, p_niveau.sequenceATrouver);
    this->m_countDownEnSecondes = p_niveau.countDownEnSecondes;
    this->m_countDownInitial = p_niveau.countDownEnSecondes;
    this->m_dureePenaliteEnSecondes = p_niveau.dureePenaliteEnSecondes;    
}

void LogiqueJeu::tick() {
    unsigned long dateActuelle = millis();
    for(Tache* tache : this->m_taches) {
        tache->tick();
    }

    gererEnvoiDesDonnees(dateActuelle);

    if (this->m_estEnJeu) {
        gererEtatEnJeu(dateActuelle);

        if (this->m_etat == ETAT_PENALITE) {
            gererEtatPenalite(dateActuelle);
        }
    } else {
        animer4Digits(dateActuelle);
    }
}

// private

void LogiqueJeu::mettreEnEtatPenalite(const uint8_t& p_dureePenaliteEnSecondes) {
    if (this->m_etat != ETAT_PENALITE) {
        this->m_indexSequence = 0;
        this->m_etat = ETAT_PENALITE;
        this->m_countDownEnSecondes -= p_dureePenaliteEnSecondes;
        this->m_dateDernierChangementPenalite = millis();
        this->m_lcd->afficher("Penalite!       -" + String(p_dureePenaliteEnSecondes));
        this->m_buzzer->arreterCountdown();
        this->m_buzzer->buzzPenalite();
    }
}

void LogiqueJeu::gererEtatPenalite(const unsigned long& p_dateActuelle) {
    if (p_dateActuelle - this->m_dateDernierChangementPenalite >= DELAIS_PENALITE) {
        this->m_etat = 0;
        this->m_lcd->afficher("Desamorcez la   bombe!");
    }
}

void LogiqueJeu::gererEtatEnJeu(const unsigned long& p_dateActuelle) {
    if (p_dateActuelle - this->m_dateDernierChangementCountdown >= 1000 && this->m_etat == 0) {
        if (this->m_countDownEnSecondes <= 11) {
            this->m_buzzer->lancerCountdown( this->m_countDownEnSecondes);
        } else {
            this->m_buzzer->buzz(75, 2000);
        }
        --this->m_countDownEnSecondes;
        int minutes = this->m_countDownEnSecondes / 60;
        int seconds = this->m_countDownEnSecondes % 60;
        float temps = minutes + (seconds / 100.0);
        this->m_4digits->afficherReel(temps);
        this->m_dateDernierChangementCountdown = p_dateActuelle;

        if (this->m_countDownEnSecondes > this->m_countDownInitial) {
            this->m_4digits->afficherEntier(0);
            this->m_lcd->afficher("Booooooooooooom!");
            this->m_del->activerClignoter();
            this->m_estEnJeu = false;
            this->m_buzzer->arreterCountdown();
            this->m_buzzer->buzzExplosion();
        }
    }
}

void LogiqueJeu::gererEnvoiDesDonnees(const unsigned long& p_dateActuelle) {
    if (p_dateActuelle - this->m_dateDernierEnvoiDeDonnees >= DELAIS_ENVOI_DONNEES) {
        this->m_dateDernierEnvoiDeDonnees = p_dateActuelle;
        if (this->m_estEnJeu && this->m_clientMQTT->connected()) {
            this->m_clientMQTT->publish((String(this->m_nomUnique) + "/temps").c_str(), String(this->m_countDownEnSecondes).c_str());
        } else {
            this->m_clientMQTT->publish((String(this->m_nomUnique) + "/temps").c_str(), "0");
        }
    }
}

void LogiqueJeu::animer4Digits(const unsigned long& p_dateActuelle) {
    if (p_dateActuelle - this->m_dateDernierChangementCountdown >= DELAIS_ANIMATION_4DIGITS) {
        this->m_dateDernierChangementCountdown = p_dateActuelle;
        this->m_segmentAttente = this->m_segmentAttente << 1;
        
        if (this->m_segmentAttente == 64) {
            this->m_segmentAttente = 1;
        }

        this->m_4digits->afficher(
            this->m_segmentAttente,
            this->m_segmentAttente << 1 >= 64 ? 1 : this->m_segmentAttente << 1,
            this->m_segmentAttente,
            this->m_segmentAttente << 1 >= 64 ? 1 : this->m_segmentAttente << 1,
            false
        );
    }
}