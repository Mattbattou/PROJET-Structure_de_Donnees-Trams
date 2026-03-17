#ifndef TRAMWAY_H
#define TRAMWAY_H

#include "ligne.h"
#include "arret.h"

class tramway {
private:
    ligne* d_ligneAffectee;      // Ligne affectée
    int d_vitesseMax;            // Vitesse maximale
    int d_vitesseActuelle;       // Vitesse actuelle
    int d_distanceMinimale;      // Distance de sécurité
    bool d_sensNormal;           // Sens de déplacement

    arret* d_arretActuel;        // Position à l'arrêt
    double d_tempsPasseArret;       // Temps passé à l'arrêt

    NoeudArret* d_maillonSuivant; // Prochain arrêt dans la liste chaînée
    double d_distanceArretSuivant;   // Distance restante
    double d_distTotaleSegment;      // Distance entre deux arrêts
    arret* d_arretPrecedent;

    bool d_sensInitial;

public:
    tramway(ligne* l, int vMax, int distMin, bool sensAller = true);

    void mettreAjourPosition(const std::vector<tramway*>& tousLesTrams);

    arret* getArretActuel();
    arret* getArretSuivant();
    arret* getArretPrecedent();
    double getDistanceRestante();
    double getDistanceTotaleSegment();
    ligne* getLigne();
    double calculerProgression();
    bool getSens();
    int getVitesseActuelle();

    void reinitialiser();
};

#endif
