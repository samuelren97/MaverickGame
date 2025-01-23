#pragma once
#include <Arduino.h>
#include <FS.h>
#include "List.h"
#include "Tache.h"
#include "LogiqueJeu.h"

class WebServer;

class ServeurWeb: public Tache {
 public:
  ServeurWeb(LogiqueJeu* p_logiqueJeu);
  virtual void tick();

 private:
  LogiqueJeu* m_logiqueJeu;
  WebServer* m_webServer;

  void optionsCors() const;
  void envoyerCors() const;
  void afficherRacine();
  void ajouterFichiersStatiques(String const& p_debutNomFichier);
  void ajouterFichiersStatiques(String const& p_debutNomFichier,
                                String const& p_repertoireCourant,
                                File& p_fichier);        
  void ressourceNonTrouvee(String const& p_nomFichier);
};