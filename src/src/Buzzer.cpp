#include "Buzzer.h"

Buzzer::Buzzer(const uint8_t& p_pin, const uint16_t& p_dureeCountdown)
: Buzzer(p_pin, p_dureeCountdown, 1000)
{
    ;
}

Buzzer::Buzzer(const uint8_t& p_pin, const uint16_t& p_dureeCountdown, const unsigned long& p_dureeBuzz)
: m_etat(ETAT_MANUEL),
m_dateDernierChangementDureeCountdown(millis()),
m_dateDernierChangement(millis()),
m_pin(p_pin),
m_dureeBuzz(p_dureeBuzz),
m_dureeDepuisDebutCountdown(0),
m_estEnFonction(false),
m_dureeCountdownEnSecondes(p_dureeCountdown),
m_etapeMusicale(0),
m_frequence(1000)
{
    ;
}

void Buzzer::lancerCountdown() {
    lancerCountdown(this->m_dureeCountdownEnSecondes);
}

void Buzzer::lancerCountdown(const uint16_t& p_dureeCountdown) {
    if (this->m_etat != ETAT_COUNTDOWN) {
        this->m_dureeCountdownEnSecondes = p_dureeCountdown;
        this->m_etat = ETAT_COUNTDOWN;
        this->m_dateDernierChangement = millis();
        this->m_dateDernierChangementDureeCountdown = millis();
        this->m_dureeDepuisDebutCountdown = 0;
    }
}

void Buzzer::arreterCountdown() {
    this->m_etat = ETAT_MANUEL;
    noTone(this->m_pin);
}

void Buzzer::dureeBuzz(unsigned long& p_dureeBuzz) {
    this->m_dureeBuzz = p_dureeBuzz;
}

void Buzzer::buzz(const unsigned long& p_duree, const uint16_t& p_frequence) {
    noTone(this->m_pin);
    tone(this->m_pin, p_frequence, p_duree);
}

void Buzzer::buzzVictoire() {
    this->m_etat = ETAT_BUZZ_VICTOIRE;
    this->m_etapeMusicale = 0;
    this->m_frequence = 1000;
}

void Buzzer::buzzExplosion() {
    this->m_etat = ETAT_BUZZ_EXPLOSION;
    this->m_etapeMusicale = 0;
    this->m_frequence = 4050;
}

void Buzzer::buzzPenalite() {
    this->m_etat = ETAT_BUZZ_PENALITE;
    this->m_etapeMusicale = 0;
    this->m_frequence = FREQ_BASE;
}

void Buzzer::tick() {
    unsigned long dateActuelle = millis();

    if (this->m_etat == ETAT_COUNTDOWN) {

        // AJOUT UNE SECONDE
        if (dateActuelle - this->m_dateDernierChangementDureeCountdown >= 1000) {
            Serial.println("En coundown: " + String(this->m_dureeDepuisDebutCountdown));
            ++this->m_dureeDepuisDebutCountdown;
            this->m_dateDernierChangementDureeCountdown = dateActuelle;
            
            if (this->m_dureeDepuisDebutCountdown == this->m_dureeCountdownEnSecondes / 2) {
                this->m_dureeBuzz /= 2;
            }
        }

        // FIN
        if (this->m_dureeDepuisDebutCountdown >= this->m_dureeCountdownEnSecondes) {
            this->m_etat = ETAT_MANUEL;
            this->m_dateDernierChangement = dateActuelle;
            noTone(this->m_pin);
            this->m_estEnFonction = false;
        }
        
        // Controle du buzz
        if (dateActuelle - this->m_dateDernierChangement >= this->m_dureeBuzz) {
            this->m_dateDernierChangement = dateActuelle;
            
            this->m_estEnFonction = !this->m_estEnFonction;
            if (this->m_estEnFonction) {
                tone(this->m_pin, FREQ_BASE);
            } else {
                noTone(this->m_pin);
            }
        }
    } else if (this->m_etat == ETAT_BUZZ_VICTOIRE) {
        uint16_t dureeBuzz = 350;
        if (this->m_etapeMusicale == 4) {
            dureeBuzz = 3000;
            this->m_etat = ETAT_MANUEL;
        }

        if (dateActuelle - this->m_dateDernierChangement >= dureeBuzz) {
            this->m_dateDernierChangement = dateActuelle;
            ++this->m_etapeMusicale;
            tone(this->m_pin, this->m_frequence, dureeBuzz);
            this->m_frequence += 500;
        }
    } else if (this->m_etat == ETAT_BUZZ_EXPLOSION) {
        uint16_t dureeBuzz = 350;
        if (this->m_etapeMusicale == 4) {
            dureeBuzz = 3000;
            this->m_etat = ETAT_MANUEL;
        }

        if (dateActuelle - this->m_dateDernierChangement >= dureeBuzz) {
            this->m_dateDernierChangement = dateActuelle;
            ++this->m_etapeMusicale;
            tone(this->m_pin, this->m_frequence, dureeBuzz);
            this->m_frequence -= 500;
        }
    } else if (this->m_etat == ETAT_BUZZ_PENALITE) {
        if ((this->m_etapeMusicale & 1) == 0) {
            if (dateActuelle - this->m_dateDernierChangement >= 50) {
                this->m_dateDernierChangement = dateActuelle;
                ++this->m_etapeMusicale;
                tone(this->m_pin, this->m_frequence, 150);
            }
        } else {
            if (dateActuelle - this->m_dateDernierChangement >= 150) {
                this->m_dateDernierChangement = dateActuelle;
                ++this->m_etapeMusicale;
            }
        }

        if (this->m_etapeMusicale == 4) {
            this->m_etat = ETAT_MANUEL;
        }
    }
}