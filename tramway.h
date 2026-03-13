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
    int d_tempsPasseArret;       // Temps passé à l'arrêt

    NoeudArret* d_maillonSuivant; // Prochain arrêt dans la liste chaînée
    int d_distanceArretSuivant;   // Distance restante
    int d_distTotaleSegment;      // Distance entre deux arrêts
    arret* d_arretPrecedent;

public:
    tramway(ligne* l, int vMax, int distMin);

    void mettreAjourPosition();

    arret* getArretActuel();
    arret* getArretSuivant();
    arret* getArretPrecedent();
    int getDistanceRestante();
    int getDistanceTotaleSegment();
};

#endif
