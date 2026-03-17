#include "fenetretramway.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QLabel>

fenetreTramway::fenetreTramway(QWidget *parent)
    : QWidget(parent), nomsVisibles(true)
{
    scene = new QGraphicsScene(this);
    vue = new QGraphicsView(scene);
    scene->setSceneRect(0, 0, 800, 600);
    vue->setRenderHint(QPainter::Antialiasing);

    layoutBoutons = new QVBoxLayout();
    monReseau = new reseau();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &fenetreTramway::tickSimulation);
    // On a retiré le timer->start(33) ici pour qu'il ne se lance qu'au clic du bouton !

    boutonSimulation = new QPushButton("▶ Démarrer Simulation", this);
    boutonSimulation->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 10px;");
    connect(boutonSimulation, &QPushButton::clicked, this, &fenetreTramway::toggleSimulation);
    layoutBoutons->addWidget(boutonSimulation);

    boutonReset = new QPushButton("🔄 Réinitialiser", this);
    boutonReset->setStyleSheet("background-color: #FF9800; color: white; font-weight: bold; padding: 10px;");
    boutonReset->setEnabled(false); // Désactivé au lancement
    connect(boutonReset, &QPushButton::clicked, this, &fenetreTramway::resetSimulation);
    layoutBoutons->addWidget(boutonReset);

    boutonNoms = new QPushButton("Masquer les noms", this);
    connect(boutonNoms, &QPushButton::clicked, this, &fenetreTramway::toggleNomsArrets);
    layoutBoutons->addWidget(boutonNoms);

    // --- NOUVEAU : LÉGENDE DES ÉTATS ---
    layoutBoutons->addSpacing(20); // Petit espace visuel

    QLabel *titreLegende = new QLabel("<b>Légende des états :</b>", this);
    layoutBoutons->addWidget(titreLegende);

    QLabel *legendeRoule = new QLabel("<span style='color:green; font-size:16px;'>■</span> En circulation", this);
    layoutBoutons->addWidget(legendeRoule);

    QLabel *legendeArret = new QLabel("<span style='color:orange; font-size:16px;'>■</span> À l'arrêt (Station)", this);
    layoutBoutons->addWidget(legendeArret);

    QLabel *legendeUrgence = new QLabel("<span style='color:red; font-size:16px;'>■</span> Arrêt d'urgence", this);
    layoutBoutons->addWidget(legendeUrgence);

    // On prépare le layout global
    layoutPrincipal = new QHBoxLayout(this);
    layoutPrincipal->addLayout(layoutBoutons);
    layoutPrincipal->addWidget(vue);

    setWindowTitle("Simulation Réseau Tramway");
    resize(950, 650);
    layoutBoutons->addStretch(); // Pousse tout vers le haut

    chargerReseau();
}

fenetreTramway::~fenetreTramway() {
    delete monReseau;
}

void fenetreTramway::chargerReseau() {
    int rayon = 7; // Rayon des cercles des arrêts

    // ==========================================
    // --- 1. CHARGEMENT DES ARRÊTS ---
    // ==========================================
    QFile fichierArrets("arrets.txt");
    if (fichierArrets.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichierArrets);
        while (!flux.atEnd()) {
            QStringList infos = flux.readLine().split(";");

            if (infos.size() == 4) {
                arret* a = new arret(infos[0].toStdString(), infos[1].toInt(), infos[2].toInt(), infos[3].toInt());
                indexArrets.insert(infos[0], a);

                // Dessin du rond de l'arrêt
                QGraphicsEllipseItem* cercle = scene->addEllipse(a->getX() - rayon, a->getY() - rayon, rayon * 2, rayon * 2, QPen(Qt::black), QBrush(Qt::white));
                cercle->setZValue(1);

                // Texte du nom de l'arrêt
                QGraphicsTextItem* texte = scene->addText(infos[0]);
                texte->setPos(a->getX() + 8, a->getY() - 20);
                texte->setZValue(2);

                mapArretsVisuels.insert(a, {cercle, texte});
            }
        }
        fichierArrets.close();
    }

    // ==========================================
    // --- 2. CHARGEMENT DES LIGNES ---
    // ==========================================
    QFile fichierLignes("lignes.txt");
    if (fichierLignes.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichierLignes);
        while (!flux.atEnd()) {
            QStringList infos = flux.readLine().split(";");
            if (infos.size() >= 3) {
                // NOUVEAU : On passe la couleur au constructeur de la ligne
                ligne* l = new ligne(infos[0].toStdString(), infos[1].toStdString());
                monReseau->ajouterLigne(l);

                QCheckBox *cb = new QCheckBox(infos[0], this);
                cb->setChecked(true);
                // On insère la case à cocher dans le menu de gauche
                layoutBoutons->insertWidget(layoutBoutons->count() - 1, cb);
                connect(cb, &QCheckBox::toggled, this, &fenetreTramway::toggleLigneVisibility);

                QList<QGraphicsLineItem*> segments;
                QList<arret*> arretsLigne;
                QPen pen(QColor(infos[1]), 4); // Couleur du trait des rails

                for (int i = 2; i < infos.size(); ++i) {
                    if (indexArrets.contains(infos[i])) {
                        arret* aCible = indexArrets.value(infos[i]);
                        arretsLigne.append(aCible);
                        l->insererArretFin(aCible, 100);

                        if (i > 2) {
                            arret* aPrec = indexArrets.value(infos[i-1]);
                            QGraphicsLineItem* s = scene->addLine(aPrec->getX(), aPrec->getY(), aCible->getX(), aCible->getY(), pen);
                            s->setZValue(-1); // Les rails sont au fond
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

    // ==========================================
    // --- 3. CHARGEMENT DES TRAMWAYS ---
    // ==========================================
    QFile fichierTrams("tramways.txt");
    if (fichierTrams.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Fichier tramways.txt ouvert avec succès.";
        QTextStream flux(&fichierTrams);
        while (!flux.atEnd()) {
            QStringList infos = flux.readLine().split(";");
            if (infos.size() >= 3) {
                ligne* l = monReseau->chercherLigne(infos[0].toStdString());

                if (l) {
                    // NOUVEAU : On alterne le sens de départ (Aller ou Retour)
                    bool sensAller = (l->getTramways().size() % 2 == 0);

                    // On crée le tramway avec ce sens
                    tramway* t = new tramway(l, infos[1].toInt(), infos[2].toInt(), sensAller);

                    // NOUVEAU : On l'ajoute au réseau global ET à sa ligne
                    monReseau->ajouterTramway(t);
                    l->ajouterTramway(t);

                    arret* depart = t->getArretActuel();
                    if (depart) {
                        qDebug() << "Création graphique du tram sur la ligne" << infos[0] << (sensAller ? "(Aller)" : "(Retour)");

                        // NOUVEAU : Le tramway prend la couleur de sa ligne !
                        QColor couleurLigne = QColor(QString::fromStdString(l->getCouleur()));

                        QGraphicsRectItem* r = scene->addRect(depart->getX() - 6, depart->getY() - 6, 12, 12, QPen(Qt::black), QBrush(couleurLigne));
                        r->setZValue(3); // Tout devant
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

    // 1. Création d'une liste contenant UNIQUEMENT les tramways des lignes cochées (optimisation)
    std::vector<tramway*> tramsActifs;
    for (tramway* t : monReseau->getTramways()) {
        if (t->getLigne()->estVisible()) {
            tramsActifs.push_back(t);
        }
    }

    // 2. Mise à jour de tous les tramways
    for (tramway* tramway : monReseau->getTramways()) {

        bool ligneVisible = tramway->getLigne()->estVisible();

        if (mapTramsVisuels.contains(tramway)) {
            QGraphicsRectItem* carre = mapTramsVisuels.value(tramway);

            // On cache le carré si la ligne a été décochée
            carre->setVisible(ligneVisible);

            // Si la ligne est masquée, on passe au tram suivant sans faire de calculs
            if (!ligneVisible) continue;

            // On met à jour la position en lui donnant SEULEMENT les trams visibles (anti-collision)
            tramway->mettreAjourPosition(tramsActifs);

            // --- NOUVEAU : GESTION DES COULEURS DE CONTOUR ---
            QPen contour;
            contour.setWidth(2); // Épaisseur du liseré

            if (tramway->getArretActuel() != nullptr) {
                contour.setColor(QColor("orange")); // À l'arrêt (Station)
            } else if (tramway->getVitesseActuelle() == 0) {
                contour.setColor(Qt::red);          // Arrêt d'urgence (bloqué par un autre tram)
            } else {
                contour.setColor(Qt::green);        // En circulation normale
            }
            carre->setPen(contour);

            // --- CALCUL DES COORDONNÉES X ET Y ---
            int x, y;

            if (tramway->getArretActuel()) {
                x = tramway->getArretActuel()->getX();
                y = tramway->getArretActuel()->getY();
            } else {
                arret *arretPrecedant = tramway->getArretPrecedent();
                arret *arretSuivant = tramway->getArretSuivant();

                if (arretPrecedant && arretSuivant && tramway->getDistanceTotaleSegment() > 0) {
                    double pourcentageDistanceParcourue = (double)(tramway->getDistanceTotaleSegment() - tramway->getDistanceRestante()) / tramway->getDistanceTotaleSegment();

                    x = arretPrecedant->getX() + (arretSuivant->getX() - arretPrecedant->getX()) * pourcentageDistanceParcourue;
                    y = arretPrecedant->getY() + (arretSuivant->getY() - arretPrecedant->getY()) * pourcentageDistanceParcourue;
                } else {
                    continue; // Sécurité si données manquantes
                }
            }

            // Mise à jour de la position du carré sur la carte
            carre->setRect(x - 6, y - 6, 12, 12);
        }
    }
    scene->update(); // On redessine tout à la fin
}

void fenetreTramway::toggleSimulation() {
    if (timer->isActive()) {
        // 1. ON MET EN PAUSE
        timer->stop();
        boutonSimulation->setText("▶ Reprendre Simulation");
        boutonSimulation->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 10px;");

        // On active le bouton de réinitialisation uniquement en pause !
        boutonReset->setEnabled(true);

        // ATTENTION : On NE réactive PAS les cases à cocher ici !

    } else {
        // 2. ON DÉMARRE LA SIMULATION
        timer->start(33);
        boutonSimulation->setText("⏸ Mettre en Pause");
        boutonSimulation->setStyleSheet("background-color: #f44336; color: white; font-weight: bold; padding: 10px;");

        // On désactive le bouton de réinitialisation
        boutonReset->setEnabled(false);

        // On grise/désactive toutes les cases à cocher pendant que ça roule
        for (QCheckBox* cb : lignesGraphiquesMap.keys()) {
            cb->setEnabled(false);
        }
    }
}

void fenetreTramway::toggleNomsArrets() {
    nomsVisibles = !nomsVisibles;
    boutonNoms -> setText(nomsVisibles ? "Masquer les noms" : "Afficher les noms");
    majVisibiliteArrets();
}

void fenetreTramway::toggleLigneVisibility(bool checked) {
    QCheckBox *cb = qobject_cast<QCheckBox*>(sender());
    if (cb && lignesGraphiquesMap.contains(cb)) {

        for(auto s : lignesGraphiquesMap.value(cb)) s->setVisible(checked);

        ligne* l = monReseau->chercherLigne(cb->text().toStdString());
        if (l != nullptr) {
            l->setVisible(checked);

            for (tramway* t : l->getTramways()) {
                if (mapTramsVisuels.contains(t)) {
                    mapTramsVisuels.value(t)->setVisible(checked);
                }
            }
        }
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

void fenetreTramway::resetSimulation() {
    // 1. On réinitialise tous les tramways
    for (tramway* t : monReseau->getTramways()) {
        t->reinitialiser();
    }

    // 2. On déverrouille les cases à cocher pour qu'elles soient à nouveau modifiables
    for (QCheckBox* cb : lignesGraphiquesMap.keys()) {
        cb->setEnabled(true);
    }

    // 3. On remet l'interface à zéro
    boutonReset->setEnabled(false);
    boutonSimulation->setText("▶ Démarrer Simulation");

    // 4. On force un affichage immédiat des nouvelles positions
    tickSimulation();
}
