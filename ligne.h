#ifndef LIGNE_H
#define LIGNE_H

#include "arret.h"
#include <string>

using namespace std;

// Création de la structure du maillon pour la liste chaînée (Obligatoire)
struct NoeudArret {
    arret* station;           // Pointeur vers l'arrêt physique
    int distanceSuivant;      // Distance vers l'arrêt suivant
    NoeudArret* suivant;      // Pointeur vers le maillon suivant
};

class ligne
{
private:
    string d_nom;
    string d_couleur;
    NoeudArret* d_premierArret;


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
};

#endif
