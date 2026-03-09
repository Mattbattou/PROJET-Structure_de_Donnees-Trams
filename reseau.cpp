#include "reseau.h"

// --- Constructeur ---
reseau::reseau()
{
    // Les tableaux dynamiques vector s'initialisent vides automatiquement
}

// --- Destructeur ---
reseau::~reseau()
{
    // On libère la mémoire de tous les tramways alloués avec 'new'
    for (tramway* t : d_tramways) {
        delete t;
    }
    d_tramways.clear(); // Vide le tableau dynamique

    // On libère la mémoire de toutes les lignes
    for (ligne* l : d_lignes) {
        delete l;
    }
    d_lignes.clear();
}

// --- Insérer un élément ---
void reseau::ajouterLigne(ligne* nouvelleLigne)
{
    if (nouvelleLigne != nullptr) {
        d_lignes.push_back(nouvelleLigne); // push_back ajoute à la fin du tableau dynamique
    }
}

void reseau::ajouterTramway(tramway* nouveauTram)
{
    if (nouveauTram != nullptr) {
        d_tramways.push_back(nouveauTram);
    }
}

// --- Chercher un élément ---
ligne* reseau::chercherLigne(string nomLigne)
{
    for (ligne* l : d_lignes) {
        if (l->getNom() == nomLigne) {
            return l; // On a trouvé la ligne
        }
    }
    return nullptr; // Ligne non trouvée
}

// --- Getters ---
vector<ligne*> reseau::getLignes() { return d_lignes; }
vector<tramway*> reseau::getTramways() { return d_tramways; }
