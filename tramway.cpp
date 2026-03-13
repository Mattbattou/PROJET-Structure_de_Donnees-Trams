#include "tramway.h"

tramway::tramway(ligne* ligne, int vMax, int distMin)
    : d_ligneAffectee(ligne), d_vitesseMax(vMax), d_distanceMinimale(distMin) {
    d_vitesseActuelle = 0;
    d_tempsPasseArret = 0;
    d_sensNormal = true;

    if (ligne && ligne -> getPremierArret()) {
        d_arretActuel = ligne -> getPremierArret() -> station;
        d_arretPrecedent = d_arretActuel;
        d_maillonSuivant = ligne -> getPremierArret() -> suivant;

        if (d_maillonSuivant) {
            d_distanceArretSuivant = ligne -> getPremierArret() -> distanceSuivant;
            d_distTotaleSegment = d_distanceArretSuivant;
        }
    }
}

void tramway::mettreAjourPosition() {
    if (d_arretActuel != nullptr) {
        d_tempsPasseArret++;
        if (d_tempsPasseArret >= d_arretActuel -> getTempsArretMinimum() && d_maillonSuivant != nullptr) {
            d_arretPrecedent = d_arretActuel;
            d_arretActuel = nullptr;
            d_vitesseActuelle = d_vitesseMax;
            d_tempsPasseArret = 0;
        }
    }
    else if (d_maillonSuivant != nullptr) {
        d_distanceArretSuivant -= d_vitesseActuelle;
        if (d_distanceArretSuivant <= 0) {
            d_arretActuel = d_maillonSuivant->station;
            d_vitesseActuelle = 0;
            d_distTotaleSegment = d_maillonSuivant->distanceSuivant;
            d_distanceArretSuivant = d_distTotaleSegment;
            d_maillonSuivant = d_maillonSuivant->suivant;
        }
    }
}

arret* tramway::getArretActuel() { return d_arretActuel; }
arret* tramway::getArretSuivant() { return d_maillonSuivant ? d_maillonSuivant->station : nullptr; }
arret* tramway::getArretPrecedent() { return d_arretPrecedent; }
int tramway::getDistanceRestante() { return d_distanceArretSuivant; }
int tramway::getDistanceTotaleSegment() { return d_distTotaleSegment; }
