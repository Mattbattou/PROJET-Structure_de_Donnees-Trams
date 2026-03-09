#ifndef RESEAU_H
#define RESEAU_H

#include "ligne.h"
#include "tramway.h"
#include <vector>
#include <string>

using namespace std;

class reseau
{
private:
    // Utilisation de tableaux dynamiques (std::vector) comme demandé
    vector<ligne*> d_lignes;
    vector<tramway*> d_tramways;

public:
    // Constructeur et Destructeur
    reseau();
    ~reseau();

    // Méthodes d'ajout (Insérer un élément)
    void ajouterLigne(ligne* nouvelleLigne);
    void ajouterTramway(tramway* nouveauTram);

    // Méthodes de recherche (Chercher un élément)
    ligne* chercherLigne(string nomLigne);

    // Getters pour récupérer les listes lors de l'affichage
    vector<ligne*> getLignes();
    vector<tramway*> getTramways();
};

#endif
