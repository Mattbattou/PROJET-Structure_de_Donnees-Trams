#include "arret.h"

arret::arret(std::string nom, int x, int y, int temps)
    : d_nom(nom), d_x(x), d_y(y), tempsArretMinimum(temps)
{}

int arret::getX() { return d_x; }
int arret::getY() { return d_y; }
std::string arret::getNom() { return d_nom; }
int arret::getTempsArretMinimum() { return tempsArretMinimum; }
