#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <Wifi.h>
#include <WiFiManager.h>

#include "List.h"
#include "Bouton.h"
#include "BME.h"
#include "Tache.h"
#include "LogiqueJeu.h"
#include "DEL.h"
#include "LCD1602.h"
#include "ServeurWeb.h"

class Program {
public:
    Program();
    void loop();

private:
    WiFiManagerParameter* m_parametreServeurMQTT;
    WiFiManagerParameter* m_parametreUserMQTT;
    WiFiManagerParameter* m_parametrePasswordMQTT;
    WiFiManager* m_wifiManager;
    WiFiClient m_clientWifi;
    PubSubClient* m_clientMQTT;

    Bouton* m_boutonRouge;
    Bouton* m_boutonBleu;
    Bouton* m_boutonVert;
    Bouton* m_boutonJaune;
    Bouton* m_boutonReset;

    Bouton* m_fil1;

    BME* m_bme;
    DEL* m_del;
    LCD1602* m_lcd;

    LogiqueJeu* m_logiqueJeu;
    ServeurWeb* m_serveurWeb;  

    List<Tache*> m_taches;

    unsigned long m_dateDerniereLectureBME;
    String m_nomUnique;

    void envoyerDonnees();
    bool connectionMQTT();
    void envoyerDecouvertes();
    void chargerParametres();
};