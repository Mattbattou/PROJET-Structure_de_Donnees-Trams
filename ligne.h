#ifndef LIGNE_H
#define LIGNE_H

#include "arret.h"
#include <string>
#include <vector>

using namespace std;

class tramway;

// Création de la structure du maillon pour la liste chaînée (Obligatoire)
struct NoeudArret {
    arret* station;           // Pointeur vers l'arrêt physique
    int distanceSuivant;      // Distance vers l'arrêt suivant
    NoeudArret* suivant;      // Pointeur vers le maillon suivant
    NoeudArret* precedent;    // Pointeur vers le maillon precedant
};

class ligne
{
private:
    string d_nom;
    string d_couleur;
    NoeudArret* d_premierArret;
    vector<tramway*> d_tramwaysLigne;
    bool d_estVisible;


public:
    ligne(string nom, string couleur);
    ~ligne();

    // Méthodes obligatoires demandées par le projet
    void insererArretFin(arret* nouvelArret, int distancePrecedent);
    void supprimerArret(string nomArret);
    arret* chercherArret(string nomArret);

    string getNom();
    string getCouleur();
    NoeudArret* getPremierArret();

    void ajouterTramway(tramway* t);
    vector<tramway*> getTramways();

    void setVisible(bool visible);
    bool estVisible();
};

#endif
