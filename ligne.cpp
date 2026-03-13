#include "ligne.h"
#include <iostream>

// --- Constructeur ---
ligne::ligne(string nom) : d_nom(nom), d_premierArret(nullptr)
{}

// --- Destructeur ---
ligne::~ligne()
{
    NoeudArret* courant = d_premierArret;
    while (courant != nullptr) {
        NoeudArret* aSupprimer = courant;
        courant = courant -> suivant;
        delete aSupprimer; // On libère proprement le maillon de la liste
    }
}

// --- Insérer un élément ---
void ligne::insererArretFin(arret* nouvelArret, int distancePrecedent)
{
    // Création du nouveau maillon
    NoeudArret* nouveau = new NoeudArret;
    nouveau -> station = nouvelArret;
    nouveau -> distanceSuivant = 0; // C'est le dernier, pas de suivant pour l'instant
    nouveau -> suivant = nullptr;

    // Si la ligne est vide
    if (d_premierArret == nullptr) {
        d_premierArret = nouveau;
    }
    else {
        // Sinon, on parcourt la liste jusqu'à la fin
        NoeudArret* courant = d_premierArret;
        while (courant -> suivant != nullptr) {
            courant = courant->suivant;
        }
        // On relie le dernier maillon actuel au nouveau
        courant -> suivant = nouveau;
        // On met à jour la distance entre l'ancien dernier et le nouveau
        courant -> distanceSuivant = distancePrecedent;
    }
}

// --- Chercher un élément ---
arret* ligne::chercherArret(string nomArret)
{
    NoeudArret* courant = d_premierArret;
    while (courant != nullptr) {
        if (courant -> station -> getNom() == nomArret) {
            return courant -> station;
        }
        courant = courant -> suivant;
    }
    return nullptr; // Non trouvé
}

// --- Supprimer un élément ---
void ligne::supprimerArret(string nomArret)
{
    if (d_premierArret == nullptr) return;

    // Si c'est le premier arrêt à supprimer
    if (d_premierArret -> station -> getNom() == nomArret) {
        NoeudArret* aSupprimer = d_premierArret;
        d_premierArret = d_premierArret -> suivant;
        delete aSupprimer;
        return;
    }

    // Sinon, on cherche dans le reste de la liste
    NoeudArret* courant = d_premierArret;
    while (courant -> suivant != nullptr && courant -> suivant -> station -> getNom() != nomArret) {
        courant = courant -> suivant;
    }

    // Si on l'a trouvé
    if (courant -> suivant != nullptr) {
        NoeudArret* aSupprimer = courant -> suivant;

        // On met à jour la distance (la nouvelle distance est la somme de l'actuelle + celle du supprimé)
        courant -> distanceSuivant += aSupprimer -> distanceSuivant;

        // On relie au suivant du supprimé
        courant -> suivant = aSupprimer -> suivant;
        delete aSupprimer;
    }
}

string ligne::getNom() { return d_nom; }
NoeudArret* ligne::getPremierArret() { return d_premierArret; }
