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

// Structure pour lier le dessin du point et son texte
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
    void toggleNomsArrets();
    void toggleLigneVisibility(bool checked);
    void toggleSimulation(); // Pour le bouton Démarrer/Pause
    void tickSimulation();   // La fonction appelée toutes les secondes par le QTimer

private:
    void majVisibiliteArrets();
    void chargerReseau();

    QGraphicsScene *scene;
    QGraphicsView *vue;

    QHBoxLayout *layoutPrincipal;
    QVBoxLayout *layoutBoutons;

    QPushButton *boutonNoms;
    QPushButton *boutonSimulation;
    bool nomsVisibles;

    // --- Variables pour la simulation ---
    QTimer *horloge;
    reseau *monReseau;

    // --- Dictionnaires pour gérer l'affichage dynamique et la lecture ---
    QMap<QString, arret*> indexArrets;
    QMap<arret*, ArretVisuel> mapArretsVisuels;
    QMap<QCheckBox*, QList<QGraphicsLineItem*>> lignesGraphiquesMap;
    QMap<QCheckBox*, QList<arret*>> arretsParLigneMap;

    // Pour lier un objet tramway logique à son petit carré graphique (pour plus tard)
    QMap<tramway*, QGraphicsRectItem*> mapTramsVisuels;
};
#endif // FENETRETRAMWAY_H
