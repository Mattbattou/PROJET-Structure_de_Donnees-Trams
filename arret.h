#ifndef ARRET_H
#define ARRET_H

#include <string>

class arret {
private:
    int d_x;                      // Position X pour l'écran
    int d_y;                      // Position Y pour l'écran
    std::string d_nom;           // Nom de l'arrêt
    int tempsArretMinimum;       // Durée minimale d'arrêt

public:
    // Constructeur complet
    arret(std::string nom, int x, int y, int temps);

    // Getters indispensables pour l'affichage et la simulation
    int getX();
    int getY();
    std::string getNom();
    int getTempsArretMinimum();
};

#endif
