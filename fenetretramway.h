#ifndef FENETRETRAMWAY_H
#define FENETRETRAMWAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QList>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTimer>

#include "arret.h"
#include "reseau.h"
#include "tramway.h"

// Structure pour regrouper les éléments graphiques d'un arrêt (le rond et son étiquette textuelle)
struct ArretVisuel {
    QGraphicsEllipseItem* cercle;
    QGraphicsTextItem* texte;
};

class fenetreTramway : public QWidget
{
    Q_OBJECT

public:
    explicit fenetreTramway(QWidget *parent = nullptr);
    ~fenetreTramway();

private slots:
    void onNomsArrets();                    // Affiche/Cache le nom des arrêts sur la carte
    void afficherCacherLigne(bool checked); // Affiche/Cache une ligne spécifique (en cochant un case)
    void onSimulation();                    // Gère le basculement du bouton Démarrer/Pause
    void tickSimulation();                  // Moteur de la simulation : fait avancer les trams à chaque "tic"
    void renitialiserSimulation();          //

private:
    // --- FONCTIONS INTERNES ---
    void majVisibiliteArrets();             // Met à jour l'affichage selon les lignes actives
    void chargerReseau();                   // Lit les fichiers .txt et construit le réseau

    // --- ARCHITECTURE GRAPHIQUE ---
    QGraphicsScene *d_scene;                // stocke et gère les coordonnées logiques des objets
    QGraphicsView *d_vue;                   // le widget physique intégré à la fenêtre qui affiche la scène

    // --- MISE EN PAGE DE LA FENÊTRE (Layouts) ---
    QHBoxLayout *layoutPrincipal;           //
    QVBoxLayout *d_layoutBoutons;           //

    // --- ÉLÉMENTS D'INTERFACE ---
    QPushButton *d_boutonVisibiliteNoms;    //
    QPushButton *d_boutonSimulation;        //
    QPushButton *d_boutonReset;             //
    bool d_nomsVisibles;                    // Mémorise l'état actuel de l'affichage du texte

    // --- MOTEUR DE SIMULATION ---
    QTimer *d_timer;                        // Le chronomètre qui cadence l'animation
    reseau *d_monReseau;                    // Le "cerveau" mathématique contenant toutes les données (lignes, trams)

    // --- DICTIONNAIRES (QMap) : Pour lier la logique (données) au visuel (dessins) ---
    QMap<QString, arret*> d_indexArrets;                    // Lie le nom d'un arrêt (ex: "Gare") à son objet logique (arret*)
    QMap<arret*, ArretVisuel> d_mapArretsVisuels;           // Lie un arrêt logique à ses dessins sur la carte (rond + texte)

    QMap<QCheckBox*, QList<QGraphicsLineItem*>> d_lignesGraphiquesMap; // Lie une case à cocher aux rails dessinés à l'écran
    QMap<QCheckBox*, QList<arret*>> d_arretsParLigneMap;               // Lie une case à cocher à la liste de ses arrêts logiques

    QMap<tramway*, QGraphicsRectItem*> d_mapTramsVisuels;   // Lie un tramway logique (calculs) à son petit carré coloré
};
#endif // FENETRETRAMWAY_H