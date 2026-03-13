#include "fenetretramway.h"
#include <QBrush>
#include <QPen>
#include <QDebug>

fenetreTramway::fenetreTramway(QWidget *parent)
    : QWidget(parent), nomsVisibles(true)
{
    scene = new QGraphicsScene(this);
    vue = new QGraphicsView(scene);
    scene -> setSceneRect(0, 0, 800, 600);
    vue -> setRenderHint(QPainter::Antialiasing); // QPainter::Antialiasing c'est pour pas que ce soit flou (de ce que j'ai compris MDRRRR)

    layoutBoutons = new QVBoxLayout();
    monReseau = new reseau();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &fenetreTramway::tickSimulation);

    boutonSimulation = new QPushButton("▶ Démarrer Simulation", this);
    boutonSimulation -> setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 10px;");
    connect(boutonSimulation, &QPushButton::clicked, this, &fenetreTramway::toggleSimulation);
    layoutBoutons -> addWidget(boutonSimulation);

    boutonNoms = new QPushButton("Masquer les noms", this);
    connect(boutonNoms, &QPushButton::clicked, this, &fenetreTramway::toggleNomsArrets);
    layoutBoutons -> addWidget(boutonNoms);

    layoutPrincipal = new QHBoxLayout(this);
    layoutPrincipal -> addLayout(layoutBoutons);
    layoutPrincipal -> addWidget(vue);

    setWindowTitle("Simulation Réseau Tramway");
    resize(950, 650);
    layoutBoutons->addStretch();

    chargerReseau();
}

fenetreTramway::~fenetreTramway() {
    delete monReseau;
}

void fenetreTramway::chargerReseau() {
    int rayon = 7; // Rayon des cercles des arrêts

    // --- CHARGEMENT DES ARRÊTS ---
    QFile fichierArrets("arrets.txt");
    if (fichierArrets.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichierArrets);
        while (!flux.atEnd()) { // Tant que c'est pas la fin du fichier (obvious)
            QStringList infos = flux.readLine().split(";"); // Dans le fichie on va utiliser les ";" pour séparer les valeurs

            if (infos.size() == 4) { // On compte le nombre de valeurs pour vérifier que le fichier est ordonné correctement
                //On creéer chaque arret jusqu'à la fin
                arret* a = new arret(infos[0].toStdString(), infos[1].toInt(), infos[2].toInt(), infos[3].toInt()); //(nom, x, y, temps d'attente à cet arret (en s)
                indexArrets.insert(infos[0], a);

                // Les ronds des arrets

                QGraphicsEllipseItem* cercle = scene -> addEllipse(a -> getX() - rayon, a -> getY() - rayon, rayon * 2, rayon * 2, QPen(Qt::black), QBrush(Qt::white));
                cercle -> setZValue(1); // Pour qu'il soit au premier plan (plus c'est haut plus c'est devant en prio)

                QGraphicsTextItem* texte = scene -> addText(infos[0]); // Nom de l'arret
                texte -> setPos(a -> getX() + 8, a -> getY() - 20);
                texte -> setZValue(2);
                mapArretsVisuels.insert(a, {cercle, texte}); // On l'ajoute pour que le bouton afficher/cacher marche
            }
        }
        fichierArrets.close();
    }

    // --- CHARGEMENT DES LIGNES ---
    QFile fichierLignes("lignes.txt");
    if (fichierLignes.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichierLignes);
        while (!flux.atEnd()) {
            QStringList infos = flux.readLine().split(";");
            if (infos.size() >= 3) {
                ligne* l = new ligne(infos[0].toStdString(), infos[1].toStdString());
                monReseau -> ajouterLigne(l);

                QCheckBox *cb = new QCheckBox(infos[0], this);
                cb->setChecked(true);
                layoutBoutons -> insertWidget(layoutBoutons -> count() - 1, cb);
                connect(cb, &QCheckBox::toggled, this, &fenetreTramway::toggleLigneVisibility);

                QList<QGraphicsLineItem*> segments;
                QList<arret*> arretsLigne;
                QPen pen(QColor(infos[1]), 4);

                for (int i = 2; i < infos.size(); ++i) {
                    if (indexArrets.contains(infos[i])) {
                        arret* aCible = indexArrets.value(infos[i]);
                        arretsLigne.append(aCible);
                        l -> insererArretFin(aCible, 100);

                        if (i > 2) {
                            arret* aPrec = indexArrets.value(infos[i-1]);
                            QGraphicsLineItem* s = scene -> addLine(aPrec -> getX(), aPrec -> getY(), aCible -> getX(), aCible -> getY(), pen);
                            s -> setZValue(-1);
                            segments.append(s);
                        }
                    }
                }
                lignesGraphiquesMap.insert(cb, segments);
                arretsParLigneMap.insert(cb, arretsLigne);
            }
        }
        fichierLignes.close();
    }

    // --- CHARGEMENT DES TRAMWAYS ---
    QFile fichierTrams("tramways.txt");
    if (fichierTrams.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Fichier tramways.txt ouvert avec succès.";
        QTextStream flux(&fichierTrams);
        while (!flux.atEnd()) {
            QStringList infos = flux.readLine().split(";");
            if (infos.size() >= 3) {
                ligne* l = monReseau->chercherLigne(infos[0].toStdString());
                if (l) {
                    tramway* t = new tramway(l, infos[1].toInt(), infos[2].toInt());
                    monReseau -> ajouterTramway(t);

                    arret* depart = t->getArretActuel();
                    if (depart) {
                        qDebug() << "Création graphique du tram sur la ligne" << infos[0];
                        // On récupère la couleur de la ligne de ce tramway en QString pour Qt
                        QString couleurLigne = QString::fromStdString(t->getLigne()->getCouleur());

                        // Remplace la création de ton QGraphicsRectItem par ça :
                        QGraphicsRectItem* r = scene->addRect(depart->getX() - 6, depart->getY() - 6, 12, 12, QPen(Qt::black), QBrush(QColor(couleurLigne)));                        r -> setZValue(3);
                        mapTramsVisuels.insert(t, r);
                    } else {
                        qDebug() << "Erreur : Le tram n'a pas d'arrêt de départ sur la ligne" << infos[0];
                    }
                } else {
                    qDebug() << "Erreur : La ligne" << infos[0] << "n'existe pas pour ce tram.";
                }
            }
        }
        fichierTrams.close();
    } else {
        qDebug() << "ERREUR : Impossible d'ouvrir tramways.txt. Vérifiez le nom et l'emplacement !";
    }
}

void fenetreTramway::tickSimulation() {
    for (tramway* tramway : monReseau -> getTramways()) {
        tramway -> mettreAjourPosition();
        if (mapTramsVisuels.contains(tramway)) {
            QGraphicsRectItem* carre = mapTramsVisuels.value(tramway);

            int x, y;

            if (tramway -> getArretActuel()) {
                x = tramway -> getArretActuel() -> getX();
                y = tramway -> getArretActuel() -> getY();
            } else {
                arret *arretPrecedant = tramway -> getArretPrecedent(), *arretSuivant = tramway -> getArretSuivant();

                if (arretPrecedant && arretSuivant && tramway -> getDistanceTotaleSegment() > 0) {
                    // Ici , pourcentageDistanceParcourue c'est le % que le tram a parcouru entre deux arrets, on l'utilise pour les petits carrés dans la simulation, on les utilise pour avoir x et y
                    double pourcentageDistanceParcourue = (double)(tramway -> getDistanceTotaleSegment() - tramway -> getDistanceRestante()) / tramway -> getDistanceTotaleSegment();

                    x = arretPrecedant -> getX() + (arretSuivant -> getX() - arretPrecedant -> getX()) * pourcentageDistanceParcourue;
                    y = arretPrecedant -> getY() + (arretSuivant -> getY() - arretPrecedant -> getY()) * pourcentageDistanceParcourue;
                } else continue;
            }
            carre -> setRect(x - 6, y - 6, 12, 12);
        }
    }
    scene->update();
}

void fenetreTramway::toggleSimulation() {
    if (timer -> isActive()) {
        timer -> stop();
        boutonSimulation -> setText("▶ Démarrer Simulation");
        boutonSimulation -> setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 10px;");
    } else {
        timer -> start(33); // 33 ms = ~30 IPS
        boutonSimulation -> setText("⏸ Mettre en Pause");
        boutonSimulation -> setStyleSheet("background-color: #f44336; color: white; font-weight: bold; padding: 10px;");
    }
}
void fenetreTramway::toggleNomsArrets() {
    nomsVisibles = !nomsVisibles;
    boutonNoms -> setText(nomsVisibles ? "Masquer les noms" : "Afficher les noms");
    majVisibiliteArrets();
}

void fenetreTramway::toggleLigneVisibility(bool checked) {
    QCheckBox *cb = qobject_cast<QCheckBox*>(sender()); // Parmi toutes les cases à cocher de l'interface, laquelle vient d'être cliquée pour déclencher cette action ?
    if (cb && lignesGraphiquesMap.contains(cb)) {
        for(auto s : lignesGraphiquesMap.value(cb)) s->setVisible(checked);
        majVisibiliteArrets();
    }
}

void fenetreTramway::majVisibiliteArrets() {
    for (auto it = mapArretsVisuels.begin(); it != mapArretsVisuels.end(); ++it) {
        bool actif = false;
        for (auto itL = arretsParLigneMap.begin(); itL != arretsParLigneMap.end(); ++itL) {
            if (itL.key() -> isChecked() && itL.value().contains(it.key())) {
                actif = true;
                break;
            }
        }
        it.value().cercle -> setVisible(actif);
        it.value().texte -> setVisible(actif && nomsVisibles);
    }
}
