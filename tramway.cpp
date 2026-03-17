#include "tramway.h"
#include <QDebug>

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

void tramway::mettreAjourPosition(const std::vector<tramway*>& tousLesTrams) {

    // ==========================================================
    // CAS 1 : LE TRAMWAY EST STATIONNÉ À UN ARRÊT
    // ==========================================================
    if (d_arretActuel != nullptr) {

        // Le chrono d'attente avance de 1/30ème de seconde
        d_tempsPasseArret += 1.0 / 30.0;

        // Si le temps minimum d'attente est écoulé
        if (d_tempsPasseArret >= d_arretActuel->getTempsArretMinimum()) {

            // --- A. On retrouve le maillon de la liste correspondant à notre arrêt actuel ---
            NoeudArret* noeudIci = d_ligneAffectee->getPremierArret();
            while (noeudIci != nullptr && noeudIci->station != d_arretActuel) {
                noeudIci = noeudIci->suivant;
            }

            // --- B. LOGIQUE DE TERMINUS (Changement de sens) ---
            bool futurSens = d_sensNormal;
            if (futurSens && noeudIci->suivant == nullptr) {
                futurSens = false; // Terminus aller : on fait demi-tour
                qDebug() << "🔄 TERMINUS : Demi-tour à" << QString::fromStdString(d_arretActuel->getNom());
            }
            else if (!futurSens && noeudIci->precedent == nullptr) {
                futurSens = true; // Terminus retour : on repart dans le sens normal
                qDebug() << "🔄 TERMINUS : Demi-tour à" << QString::fromStdString(d_arretActuel->getNom());
            }

            // --- C. On cible le futur arrêt ---
            NoeudArret* futurMaillon = futurSens ? noeudIci->suivant : noeudIci->precedent;
            if (futurMaillon == nullptr) return; // Sécurité extrême (ligne à 1 seul arrêt)

            arret* monDepart = d_arretActuel;
            arret* monArrivee = futurMaillon->station;

            // Astuce : La distance d'un segment est toujours stockée dans le maillon "Aller".
            // Si on va en sens inverse, on prend la distance du maillon précédent.
            double maDistanceAArrivee = futurSens ? noeudIci->distanceSuivant : futurMaillon->distanceSuivant;

            // --- D. VÉRIFICATION DE LA VOIE (Y a-t-il un tramway devant ?) ---
            int vitesseCible = d_vitesseMax;
            for (tramway* autre : tousLesTrams) {
                if (autre == this) continue;

                double distanceEntreNous = 999999.0;

                // Si l'autre est en train de rouler sur notre futur tronçon
                if (autre->getArretActuel() == nullptr && autre->getArretPrecedent() == monDepart && autre->getArretSuivant() == monArrivee) {
                    distanceEntreNous = maDistanceAArrivee - autre->getDistanceRestante();
                }
                // Si l'autre est à l'arrêt, bloqué à notre futur arrêt
                else if (autre->getArretActuel() == monArrivee) {
                    distanceEntreNous = maDistanceAArrivee;
                }

                // Si la voie est encombrée
                if (distanceEntreNous > 0.0 && distanceEntreNous < (double)d_distanceMinimale) {
                    vitesseCible = 0;
                    break;
                }
            }

            // --- E. DÉPART ! ---
            if (vitesseCible > 0) {
                qDebug() << "🟢 DÉPART de" << QString::fromStdString(d_arretActuel->getNom());

                d_sensNormal = futurSens;
                d_maillonSuivant = futurMaillon;
                d_distTotaleSegment = maDistanceAArrivee;
                d_distanceArretSuivant = d_distTotaleSegment;

                d_arretPrecedent = d_arretActuel;
                d_arretActuel = nullptr; // On quitte la station
                d_vitesseActuelle = d_vitesseMax;
                d_tempsPasseArret = 0.0;
            } else if (d_vitesseActuelle > 0) {
                qDebug() << "⏳ ATTENTE : Voie occupée devant" << QString::fromStdString(d_arretActuel->getNom());
                d_vitesseActuelle = 0; // On désactive la vitesse pour ne pas spammer le QDebug
            }
        }
    }

    // ==========================================================
    // CAS 2 : LE TRAMWAY EST EN CIRCULATION SUR LES RAILS
    // ==========================================================
    else if (d_maillonSuivant != nullptr) {

        arret* monDepart = d_arretPrecedent;
        arret* monArrivee = d_maillonSuivant->station;
        double maDistanceAArrivee = d_distanceArretSuivant;

        // --- A. VÉRIFICATION DE SÉCURITÉ EN TEMPS RÉEL ---
        int vitesseCible = d_vitesseMax;
        for (tramway* autre : tousLesTrams) {
            if (autre == this) continue;

            double distanceEntreNous = 999999.0;

            // Vérification sur le même tronçon
            if (autre->getArretActuel() == nullptr && autre->getArretPrecedent() == monDepart && autre->getArretSuivant() == monArrivee) {
                double autreDistance = autre->getDistanceRestante();
                if (autreDistance < maDistanceAArrivee) { // Il est devant nous !
                    distanceEntreNous = maDistanceAArrivee - autreDistance;
                }
            }
            // L'autre tram est arrivé et stationne à notre destination
            else if (autre->getArretActuel() == monArrivee) {
                distanceEntreNous = maDistanceAArrivee;
            }

            // FREINAGE D'URGENCE
            if (distanceEntreNous > 0.0 && distanceEntreNous < (double)d_distanceMinimale) {
                vitesseCible = 0;
                if (d_vitesseActuelle > 0) {
                    qDebug() << "🛑 FREINAGE D'URGENCE à" << distanceEntreNous << "m.";
                }
                break;
            }
        }

        // --- B. DÉPLACEMENT ---
        d_vitesseActuelle = vitesseCible;
        d_distanceArretSuivant -= (double)d_vitesseActuelle / 30.0; // On avance de 1/30ème de la vitesse

        // --- C. ARRIVÉE À LA STATION ---
        if (d_distanceArretSuivant <= 0.0) {
            d_arretActuel = monArrivee;
            d_vitesseActuelle = 0;

            qDebug() << "🚉 ARRIVÉE à" << QString::fromStdString(d_arretActuel->getNom());

            // On ne prépare rien d'autre ici, le "CAS 1" s'en chargera au prochain tour de boucle !
        }
    }
}


bool tramway::getSens() { return d_sensNormal; }

double tramway::calculerProgression() {
    double progression = 0.0;
    NoeudArret* courant = d_ligneAffectee->getPremierArret();

    // On parcourt la liste chaînée depuis le début
    while (courant != nullptr) {
        // Cas 1 : Le tram est actuellement stationné à cet arrêt
        if (d_arretActuel == courant->station) {
            return progression;
        }

        // Cas 2 : Le tram est en train de rouler juste après cet arrêt
        if (d_arretActuel == nullptr && d_arretPrecedent == courant->station) {
            progression += (d_distTotaleSegment - d_distanceArretSuivant);
            return progression;
        }

        // Sinon, on ajoute la distance de ce segment et on passe au maillon suivant
        progression += courant->distanceSuivant;
        courant = courant->suivant;
    }
    return progression;
}

arret* tramway::getArretActuel() { return d_arretActuel; }
arret* tramway::getArretSuivant() { return d_maillonSuivant ? d_maillonSuivant->station : nullptr; }
arret* tramway::getArretPrecedent() { return d_arretPrecedent; }
double tramway::getDistanceRestante() { return d_distanceArretSuivant; }
double tramway::getDistanceTotaleSegment() { return d_distTotaleSegment; }
ligne* tramway::getLigne() { return d_ligneAffectee; }
int tramway::getVitesseActuelle() { return d_vitesseActuelle; }
