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
    vector<ligne*> d_lignes;
    vector<tramway*> d_tramways;

public:
    reseau();
    ~reseau();

    void ajouterLigne(ligne* nouvelleLigne);
    void ajouterTramway(tramway* nouveauTram);

    ligne* chercherLigne(string nomLigne);

    vector<ligne*> getLignes();
    vector<tramway*> getTramways();
};

#endif
