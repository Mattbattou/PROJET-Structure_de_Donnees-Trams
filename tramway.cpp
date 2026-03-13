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
        // Le temps avance de 1/30ème de seconde par tick
        d_tempsPasseArret += 1.0 / 30.0;

        if (d_tempsPasseArret >= d_arretActuel -> getTempsArretMinimum() && d_maillonSuivant != nullptr) {
            d_arretPrecedent = d_arretActuel;
            d_arretActuel = nullptr;
            d_vitesseActuelle = d_vitesseMax;
            d_tempsPasseArret = 0.0; // Reset à 0.0
        }
    }
    else if (d_maillonSuivant != nullptr) {
        // La distance se réduit de 1/30ème de la vitesse par tick
        d_distanceArretSuivant -= (double)d_vitesseActuelle / 30.0;

        if (d_distanceArretSuivant <= 0.0) { // On compare à 0.0
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
double tramway::getDistanceRestante() { return d_distanceArretSuivant; }
double tramway::getDistanceTotaleSegment() { return d_distTotaleSegment; }
ligne* tramway::getLigne() { return d_ligneAffectee; }
