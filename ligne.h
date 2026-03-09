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
    NoeudArret* d_premierArret; // La tête de notre liste chaînée

    // NB: Plus tard, nous ajouterons ici la liste des tramways sur cette ligne

public:
    // Constructeur et Destructeur [cite: 276]
    ligne(string nom);
    ~ligne();

    // Méthodes obligatoires demandées par le projet
    void insererArretFin(arret* nouvelArret, int distancePrecedent);
    void supprimerArret(string nomArret);
    arret* chercherArret(string nomArret);

    // Getters
    string getNom();
    NoeudArret* getPremierArret();
};

#endif
