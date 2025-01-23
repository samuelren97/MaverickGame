#include <LittleFS.h>
#include <ArduinoJson.h>

#include "Program.h"
#include "Config.h"
#include "LCD1602ProxyI2C.h"
#include "ActionInput.h"
#include "ActionReset.h"
#include "Niveau.h"

Program::Program()
    : m_parametreServeurMQTT(nullptr),
      m_parametreUserMQTT(nullptr),
      m_parametrePasswordMQTT(nullptr),
      m_wifiManager(new WiFiManager()),
      m_clientWifi(),
      m_clientMQTT(new PubSubClient(this->m_clientWifi)),
      m_boutonRouge(new Bouton(PIN_BOUTON_ROUGE)),
      m_boutonBleu(new Bouton(PIN_BOUTON_BLEU)),
      m_boutonVert(new Bouton(PIN_BOUTON_VERT)),
      m_boutonJaune(new Bouton(PIN_BOUTON_JAUNE)),
      m_boutonReset(new Bouton(PIN_BOUTON_RESET)),
      m_fil1(new Bouton(PIN_FIL_1)),
      m_bme(new BME(ADRESSE_BME)),
      m_del(new DEL(LED_ROUGE)),
      m_lcd(new LCD1602(new LCD1602ProxyI2C())),
      m_logiqueJeu(new LogiqueJeu(this->m_lcd, this->m_clientMQTT, this->m_del)),
      m_serveurWeb(nullptr),
      m_taches(List<Tache *>(7)),
      m_dateDerniereLectureBME(millis()),
      m_nomUnique("ESP32Client" + String(ESP.getEfuseMac(), HEX))
{
    pinMode(LED_VERTE, OUTPUT);
    digitalWrite(LED_VERTE, HIGH);
    this->m_lcd->afficher("Mode            Configuration");
    chargerParametres();

    this->m_del->eteindre();
    this->m_wifiManager->setDebugOutput(false);
    this->m_wifiManager->setAPCallback([](WiFiManager *wifiManager)
    { 
        Serial.println("Connexion au wifi impossible, mode point d'acces");
    });
    this->m_wifiManager->setSaveParamsCallback([this]()
    {
        DynamicJsonDocument doc(1024);
        doc["adresse_mqtt"] = this->m_parametreServeurMQTT->getValue();
        doc["user_mqtt"] = this->m_parametreUserMQTT->getValue();
        doc["password_mqtt"] = this->m_parametrePasswordMQTT->getValue();

        File fichierMQTT = LittleFS.open(FICHIER_CONFIG, FILE_WRITE);
        char json[1024];
        serializeJson(doc, json);
        fichierMQTT.print(json);
        fichierMQTT.close();
    });
    this->m_wifiManager->addParameter(this->m_parametreServeurMQTT);
    this->m_wifiManager->addParameter(this->m_parametreUserMQTT);
    this->m_wifiManager->addParameter(this->m_parametrePasswordMQTT);
    this->m_wifiManager->setParamsPage(true);
    IPAddress adressePortail(192, 168, 2, 1);
    IPAddress gatewayPortail(192, 168, 2, 1);
    IPAddress masquePortail(255, 255, 255, 0);
    this->m_wifiManager->setAPStaticIPConfig(adressePortail, gatewayPortail, masquePortail);
    this->m_wifiManager->setParamsPage(false);
    this->m_wifiManager->autoConnect(SSID_PORTAIL, PASSWORD_PORTAIL);
    this->m_clientMQTT->setBufferSize(1024);
    this->m_clientMQTT->setServer(
        this->m_parametreServeurMQTT->getValue(), MQTT_PORT);

    this->m_boutonRouge->actionBoutonAppuyee(new ActionInput(this->m_logiqueJeu, "R"));
    this->m_boutonBleu->actionBoutonAppuyee(new ActionInput(this->m_logiqueJeu, "B"));
    this->m_boutonVert->actionBoutonAppuyee(new ActionInput(this->m_logiqueJeu, "V"));
    this->m_boutonJaune->actionBoutonAppuyee(new ActionInput(this->m_logiqueJeu, "J"));
    this->m_boutonRouge->actionBoutonRelachee(new ActionInput(this->m_logiqueJeu, "r"));
    this->m_boutonBleu->actionBoutonRelachee(new ActionInput(this->m_logiqueJeu, "b"));
    this->m_boutonVert->actionBoutonRelachee(new ActionInput(this->m_logiqueJeu, "v"));
    this->m_boutonJaune->actionBoutonRelachee(new ActionInput(this->m_logiqueJeu, "j"));
    this->m_boutonReset->actionBoutonAppuyee(new ActionReset(this->m_wifiManager));
    
    this->m_fil1->actionBoutonAppuyee(new ActionInput(this->m_logiqueJeu, "X"));
    this->m_fil1->actionBoutonRelachee(new ActionInput(this->m_logiqueJeu, "x"));
    
    this->m_taches.ajouter(this->m_boutonRouge);
    this->m_taches.ajouter(this->m_boutonBleu);
    this->m_taches.ajouter(this->m_boutonVert);
    this->m_taches.ajouter(this->m_boutonJaune);
    this->m_taches.ajouter(this->m_boutonReset);
    this->m_taches.ajouter(this->m_fil1);
    this->m_taches.ajouter(this->m_del);
    this->m_taches.ajouter(this->m_logiqueJeu);
    this->m_serveurWeb = new ServeurWeb(this->m_logiqueJeu);
    this->m_taches.ajouter(this->m_serveurWeb);

    if (WiFi.isConnected()) {
        this->m_lcd->afficher("Connecte        " + String(WiFi.localIP().toString()));
    }
}

void Program::loop()
{
    for (Tache *tache : this->m_taches)
    {
        tache->tick();
    }

    if (WiFi.isConnected())
    {
        if (connectionMQTT())
        {
            this->m_clientMQTT->loop();
            envoyerDonnees();
        }
    }
}

// Private

void Program::envoyerDonnees()
{
    unsigned long dateCourante = millis();
    if (dateCourante - this->m_dateDerniereLectureBME > DELAIS_ENVOI_DONNEES)
    {
        this->m_clientMQTT->publish((String(this->m_nomUnique) + "/availability").c_str(), "online");
        this->m_clientMQTT->publish((String(this->m_nomUnique) + "/temperature").c_str(), String(this->m_bme->temperature()).c_str());
        this->m_clientMQTT->publish((String(this->m_nomUnique) + "/pression").c_str(), String(this->m_bme->pression()).c_str());
        this->m_clientMQTT->publish((String(this->m_nomUnique) + "/humidite").c_str(), String(this->m_bme->humidite()).c_str());
        this->m_clientMQTT->publish((String(this->m_nomUnique) + "/led").c_str(), this->m_del->estClignotante() ? "ON" : "OFF");
        this->m_dateDerniereLectureBME = dateCourante;
    }
}

bool Program::connectionMQTT()
{
    if (!this->m_clientMQTT->connected())
    {
        Serial.println("Connexion MQTT: " + String(this->m_parametreServeurMQTT->getValue()));
        if (this->m_clientMQTT->connect(this->m_nomUnique.c_str(),
                                        this->m_parametreUserMQTT->getValue(),
                                        this->m_parametrePasswordMQTT->getValue(),
                                        (String(this->m_nomUnique) + "/availability").c_str(),
                                        0,
                                        true, "offline"))
        {
            Serial.println("Connection success");
            this->m_clientMQTT->subscribe((String(this->m_nomUnique) + "/#").c_str());
            this->m_clientMQTT->setCallback(
                [this](char *topic, byte *payload, unsigned int length)
                {
                    String payloadString = "";

                    for (int i = 0; i < length; i++)
                    {
                        payloadString += (char)payload[i];
                    }

                    if (String(topic).equals(String(this->m_nomUnique) + "/led/set"))
                    {
                        Serial.println(String(topic) + ", " + payloadString);
                        this->m_del->basculerClignotement();
                    }
                });
            envoyerDecouvertes();
            this->m_clientMQTT->publish((String(this->m_nomUnique) + "/availability").c_str(), "online");
        }
        else
        {
            Serial.println("Connection FAIL");
            delay(2000);
        }
    }

    return this->m_clientMQTT->connected();
}

void Program::envoyerDecouvertes()
{
    LittleFS.begin();
    File fichierTemperature = LittleFS.open("/DiscoveryBombe.json", FILE_READ);
    String donneesJson = fichierTemperature.readString();
    donneesJson.replace("#{idUnique}#", this->m_nomUnique);
    fichierTemperature.close();

    DynamicJsonDocument doc(2548);
    DeserializationError err = deserializeJson(doc, donneesJson);

    JsonArray arrJson = doc.as<JsonArray>();
    int compteur = 0;
    for (JsonObject obj : arrJson)
    {
        char json[1024];
        serializeJson(obj, json);
        this->m_clientMQTT->publish((obj["discovery_topic"].as<String>()).c_str(), json);
    }
}

void Program::chargerParametres()
{
    LittleFS.begin();
    File fichierConfig = LittleFS.open(FICHIER_CONFIG, FILE_READ);
    String donneesJson = fichierConfig.readString();
    fichierConfig.close();

    DynamicJsonDocument doc(1024);
    DeserializationError err = deserializeJson(doc, donneesJson);

    this->m_parametreServeurMQTT = new WiFiManagerParameter("1", "Adresse du serveur MQTT", doc["adresse_mqtt"], 20);
    this->m_parametreUserMQTT = new WiFiManagerParameter("2", "Utilisateur Home Assistant", doc["user_mqtt"], 120);
    this->m_parametrePasswordMQTT = new WiFiManagerParameter("3", "Password Home Assistant", doc["password_mqtt"], 120);
}