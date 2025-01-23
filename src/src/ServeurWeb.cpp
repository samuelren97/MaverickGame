#include "ServeurWeb.h"
#include "Niveau.h"
#include <Arduino.h>
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <detail/RequestHandlersImpl.h>
#include <ArduinoJson.h>
#include <uri/UriRegex.h>
#include <LittleFS.h>

#define FS LittleFS

String toURI(const String &path) {
  return path;
}

class InformationChangeAdresse {
  public:
  String adresse;
};

ServeurWeb::ServeurWeb(LogiqueJeu* p_logiqueJeu)
: m_logiqueJeu(p_logiqueJeu)
{
  FS.begin(true);

  this->m_webServer = new WebServer();

  this->ajouterFichiersStatiques("/");

  this->m_webServer->on("/niveau", HTTPMethod::HTTP_POST,
                      [this]() {
                        this->envoyerCors();
                        String json = this->m_webServer->arg("plain");
                        DynamicJsonDocument doc(1024);
                        deserializeJson(doc, json);
                        Niveau niveau;
                        niveau.countDownEnSecondes = doc["countdownEnSecondes"];
                        niveau.dureePenaliteEnSecondes = doc["dureePenaliteEnSecondes"];
                        niveau.sequenceATrouver = doc["sequenceATrouver"];
                        this->m_logiqueJeu->setNiveau(niveau);
                        this->m_logiqueJeu->demarrerJeu();

                        this->m_webServer->send(201);
                      });
  
  this->m_webServer->on("/ajouter", HTTPMethod::HTTP_POST,
                      [this]() {
                        this->envoyerCors();
                        String json = this->m_webServer->arg("plain");
                        DynamicJsonDocument docRecu(1024);
                        deserializeJson(docRecu, json);
                        Niveau niveau;
                        niveau.countDownEnSecondes = docRecu["countdownEnSecondes"];
                        niveau.dureePenaliteEnSecondes = docRecu["dureePenaliteEnSecondes"];
                        niveau.sequenceATrouver = docRecu["sequenceATrouver"];
                        String instructions = docRecu["instructions"].as<String>();
                        Serial.println(json);

                        FS.begin();
                        File fichierNiveaux = FS.open("/niveaux.json", FILE_READ);
                        String contenu = fichierNiveaux.readString();
                        fichierNiveaux.close();
                        Serial.println(contenu);

                        DynamicJsonDocument doc(1024);
                        deserializeJson(doc, contenu);

                        JsonObject niveauJson = doc.createNestedObject();
                        niveauJson["sequenceATrouver"] = niveau.sequenceATrouver;
                        niveauJson["dureePenaliteEnSecondes"] = niveau.dureePenaliteEnSecondes;
                        niveauJson["countdownEnSecondes"] = niveau.countDownEnSecondes;
                        niveauJson["instructions"] = instructions;
                        
                        char nouvContenuJson[1024];
                        fichierNiveaux = FS.open("/niveaux.json", FILE_WRITE);
                        serializeJson(doc, nouvContenuJson);
                        fichierNiveaux.print(nouvContenuJson);
                        fichierNiveaux.close();

                        this->m_webServer->send(201);
                      });

  this->m_webServer->on("/", HTTPMethod::HTTP_GET,
                        [this]() { this->afficherRacine(); });
                        
  this->m_webServer->on(UriRegex(".*"), HTTPMethod::HTTP_OPTIONS,
                      [this](){ this->optionsCors(); });
  this->m_webServer->onNotFound(
      [this]() { this->ressourceNonTrouvee(this->m_webServer->uri()); });

  this->m_webServer->begin();
}

void ServeurWeb::tick() { this->m_webServer->handleClient(); }

void ServeurWeb::afficherRacine() {
  Serial.println("Réception requête");
  Serial.println(this->m_webServer->uri());

  this->m_webServer->sendHeader("Location", "index.html", true);
  this->envoyerCors();
  this->m_webServer->send(301, "text/plain", "");
}

void ServeurWeb::optionsCors() const
{
  this->m_webServer->sendHeader("Access-Control-Allow-Origin", "*");
  this->m_webServer->sendHeader("Access-Control-Max-Age", "600");
  this->m_webServer->sendHeader("Access-Control-Allow-Methods", "PUT,GET,OPTIONS");
  this->m_webServer->sendHeader("Access-Control-Allow-Headers", "*");
  this->m_webServer->send(204);
}

void ServeurWeb::envoyerCors() const
{
  this->m_webServer->sendHeader("Access-Control-Allow-Origin", "*");
}

void ServeurWeb::ajouterFichiersStatiques(String const& p_debutNomFichier) {
  File racine = FS.open("/");
  ajouterFichiersStatiques(p_debutNomFichier, "", racine);
}

void ServeurWeb::ajouterFichiersStatiques(String const& p_debutNomFichier,
                                          String const& p_repertoireCourant,
                                          File& p_repertoire) {
  if (!p_repertoire) return;

  Serial.println(String("Traitement du répertoire : ") + p_repertoire.name());

  File fichier = p_repertoire.openNextFile();
  while (fichier) {
    String nomFichier = p_repertoireCourant + "/" + String(fichier.name());
    if (fichier.isDirectory()) {
      ajouterFichiersStatiques(p_debutNomFichier, p_repertoireCourant + "/" + fichier.name(), fichier);
    } else {
      if (nomFichier.startsWith(p_debutNomFichier)) {
        Serial.println(String("Ajout du fichier statique : ") + nomFichier + " pour l'URI " + toURI(nomFichier));
        this->m_webServer->serveStatic(toURI(nomFichier).c_str(), FS,
                                       nomFichier.c_str());
      }
    }
    fichier.close();
    fichier = p_repertoire.openNextFile();
  }

  p_repertoire.close();
}

void ServeurWeb::ressourceNonTrouvee(const String& p_nomRessource) {
  Serial.println("Ressource '" + p_nomRessource + "' non trouvée !");
  this->m_webServer->send(404, "text/plain",
                          "Ressource '" + p_nomRessource + "' non trouvée !");
}