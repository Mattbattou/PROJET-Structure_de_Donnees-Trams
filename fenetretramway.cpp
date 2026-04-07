#include "fenetretramway.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QLabel>
#include <QFrame>

fenetreTramway::fenetreTramway(QWidget *parent)
    : QWidget(parent), d_nomsVisibles(true)
{
    // --- 1. Initialisation du moteur graphique ---
    d_scene = new QGraphicsScene(this);
    d_vue = new QGraphicsView(d_scene);
    d_scene -> setSceneRect(0, 0, 800, 600);
    d_vue -> setRenderHint(QPainter::Antialiasing);

    d_layoutBoutons = new QVBoxLayout();
    d_monReseau = new reseau();

    d_timer = new QTimer(this);
    connect(d_timer, &QTimer::timeout, this, &fenetreTramway::tickSimulation);

    // --- 2. Création des contrôles utilisateur ---
    d_boutonSimulation = new QPushButton("▶ Démarrer Simulation", this);
    d_boutonSimulation -> setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px 15px; border-radius: 6px; } QPushButton:hover { background-color: #45a049; }");
    d_boutonSimulation -> setFixedWidth(220);
    connect(d_boutonSimulation, &QPushButton::clicked, this, &fenetreTramway::onSimulation);
    d_layoutBoutons -> addWidget(d_boutonSimulation);

    d_boutonReset = new QPushButton("🔄 Réinitialiser", this);
    d_boutonReset -> setStyleSheet("QPushButton { background-color: #FF9800; color: white; font-weight: bold; padding: 8px 15px; border-radius: 6px; } QPushButton:hover { background-color: #e68a00; }");
    d_boutonReset -> setEnabled(false);
    connect(d_boutonReset, &QPushButton::clicked, this, &fenetreTramway::renitialiserSimulation);
    d_layoutBoutons -> addWidget(d_boutonReset);

    d_boutonVisibiliteNoms = new QPushButton("Masquer les noms", this);
    connect(d_boutonVisibiliteNoms, &QPushButton::clicked, this, &fenetreTramway::onNomsArrets);
    d_layoutBoutons -> addWidget(d_boutonVisibiliteNoms);

    d_layoutBoutons -> addSpacing(10);

    // --- 3. Création de la légende visuelle ---
    auto separateurHaut {new QFrame{}};
    separateurHaut -> setFrameStyle(QFrame::HLine | QFrame::Sunken);
    d_layoutBoutons -> addWidget(separateurHaut);

    d_layoutBoutons -> addWidget(new QLabel("<b>Légende des états :</b>", this));
    d_layoutBoutons -> addWidget(new QLabel("<span style='color:green; font-size:16px;'>■</span> En circulation", this));
    d_layoutBoutons -> addWidget(new QLabel("<span style='color:orange; font-size:16px;'>■</span> À l'arrêt (Station)", this));
    d_layoutBoutons -> addWidget(new QLabel("<span style='color:red; font-size:16px;'>■</span> Arrêt d'urgence", this));

    auto separateurBas {new QFrame{}};
    separateurBas -> setFrameStyle(QFrame::HLine | QFrame::Sunken);
    d_layoutBoutons -> addWidget(separateurBas);

    // --- 4. Assemblage final et redimensionnement dynamique ---
    layoutPrincipal = new QHBoxLayout(this);
    layoutPrincipal -> addLayout(d_layoutBoutons);
    layoutPrincipal -> addWidget(d_vue);

    setWindowTitle("Simulation Réseau Tramway");
    d_layoutBoutons -> addStretch();

    chargerReseau();

    // Ajustement automatique de la fenêtre pour englober tout le réseau sans zoomer
    QRectF zoneReseau = d_scene -> itemsBoundingRect();
    zoneReseau.adjust(-50, -50, 50, 50);
    d_scene -> setSceneRect(zoneReseau);

    int largeurFenetreFinale = zoneReseau.width() + 250;
    int hauteurFenetreFinale = zoneReseau.height();
    this -> resize(largeurFenetreFinale + 50, hauteurFenetreFinale + 50);
}

fenetreTramway::~fenetreTramway() {
    delete d_monReseau;
}

void fenetreTramway::chargerReseau() {
    int rayonArret = 7;

    // ==========================================
    // --- 1. CHARGEMENT DES ARRÊTS ---
    // ==========================================
    QFile fichierArrets("arrets.txt");
    if (fichierArrets.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichierArrets);
        while (!flux.atEnd()) {
            QStringList infosArret = flux.readLine().split(";");

            if (infosArret.size() == 4) {
                arret* arretCourant = new arret(infosArret[0].toStdString(), infosArret[1].toInt(), infosArret[2].toInt(), infosArret[3].toInt());
                d_indexArrets.insert(infosArret[0], arretCourant);

                QGraphicsEllipseItem* cercleGraphique = d_scene -> addEllipse(arretCourant -> getX() - rayonArret, arretCourant -> getY() - rayonArret, rayonArret * 2, rayonArret * 2, QPen(Qt::black), QBrush(Qt::white));
                cercleGraphique -> setZValue(1);

                QGraphicsTextItem* texteGraphique = d_scene -> addText(infosArret[0]);
                texteGraphique -> setPos(arretCourant -> getX() + 8, arretCourant -> getY() - 20);
                texteGraphique -> setZValue(2);

                d_mapArretsVisuels.insert(arretCourant, {cercleGraphique, texteGraphique});
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
            QStringList infosLigne = flux.readLine().split(";");
            if (infosLigne.size() >= 3) {

                ligne* ligneCourante = new ligne(infosLigne[0].toStdString(), infosLigne[1].toStdString());
                d_monReseau -> ajouterLigne(ligneCourante);

                // Ajout de la case à cocher dynamique dans le menu latéral
                QCheckBox* checkBoxLigne = new QCheckBox(infosLigne[0], this);
                checkBoxLigne -> setChecked(true);
                d_layoutBoutons -> insertWidget(d_layoutBoutons -> count() - 1, checkBoxLigne);
                connect(checkBoxLigne, &QCheckBox::toggled, this, &fenetreTramway::afficherCacherLigne);

                QList<QGraphicsLineItem*> segmentsGraphiques;
                QList<arret*> arretsDeLaLigne;
                QPen styleLigne(QColor(infosLigne[1]), 4);

                // Création des tronçons physiques entre les arrêts
                for (int i = 2; i < infosLigne.size(); ++i) {
                    if (d_indexArrets.contains(infosLigne[i])) {
                        arret* arretCible = d_indexArrets.value(infosLigne[i]);
                        arretsDeLaLigne.append(arretCible);
                        ligneCourante -> insererArretFin(arretCible, 100);

                        if (i > 2) {
                            arret* arretPrecedent = d_indexArrets.value(infosLigne[i-1]);
                            QGraphicsLineItem* segment = d_scene -> addLine(arretPrecedent -> getX(), arretPrecedent -> getY(), arretCible -> getX(), arretCible -> getY(), styleLigne);
                            segment -> setZValue(-1); // Place les rails en arrière-plan
                            segmentsGraphiques.append(segment);
                        }
                    }
                }
                d_lignesGraphiquesMap.insert(checkBoxLigne, segmentsGraphiques);
                d_arretsParLigneMap.insert(checkBoxLigne, arretsDeLaLigne);
            }
        }
        fichierLignes.close();
    }

    // ==========================================
    // --- 3. CHARGEMENT DES TRAMWAYS ---
    // ==========================================
    QFile fichierTrams("tramways.txt");
    if (fichierTrams.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichierTrams);
        while (!flux.atEnd()) {
            QStringList infosTram = flux.readLine().split(";");
            if (infosTram.size() >= 3) {
                ligne* ligneAffectee = d_monReseau -> chercherLigne(infosTram[0].toStdString());

                if (ligneAffectee) {
                    // Alterne la direction (Aller/Retour) en fonction du nombre de trams existants
                    bool departSensAller = (ligneAffectee -> getTramways().size() % 2 == 0);
                    tramway* nouveauTramway = new tramway(ligneAffectee, infosTram[1].toInt(), infosTram[2].toInt(), departSensAller);

                    d_monReseau -> ajouterTramway(nouveauTramway);
                    ligneAffectee -> ajouterTramway(nouveauTramway);

                    arret* arretDepart = nouveauTramway -> getArretActuel();
                    if (arretDepart) {
                        QColor couleurTram = QColor(QString::fromStdString(ligneAffectee -> getCouleur()));
                        QGraphicsRectItem* carreGraphique = d_scene -> addRect(arretDepart -> getX() - 6, arretDepart -> getY() - 6, 12, 12, QPen(Qt::black), QBrush(couleurTram));
                        carreGraphique -> setZValue(3); // Affiche le tram au-dessus du reste
                        d_mapTramsVisuels.insert(nouveauTramway, carreGraphique);
                    }
                }
            }
        }
        fichierTrams.close();
    }
}

void fenetreTramway::tickSimulation() {
    // 1. Filtrage des tramways pour ignorer ceux dont la ligne est masquée
    std::vector<tramway*> tramsActifs;
    for (tramway* tramwayCourant : d_monReseau -> getTramways()) {
        if (tramwayCourant -> getLigne() -> estVisible()) {
            tramsActifs.push_back(tramwayCourant);
        }
    }

    // 2. Mise à jour de la logique et du rendu pour chaque tramway
    for (tramway* tramwayCourant : d_monReseau -> getTramways()) {

        bool ligneEstVisible = tramwayCourant -> getLigne() -> estVisible();

        if (d_mapTramsVisuels.contains(tramwayCourant)) {
            QGraphicsRectItem* carreGraphique = d_mapTramsVisuels.value(tramwayCourant);
            carreGraphique -> setVisible(ligneEstVisible);

            if (!ligneEstVisible) continue; // Ignore les calculs si invisible

            tramwayCourant -> mettreAjourPosition(tramsActifs);

            // --- Gestion du code couleur selon l'état actuel ---
            QPen contourEtat;
            contourEtat.setWidth(2);

            if (tramwayCourant -> getArretActuel() != nullptr) {
                contourEtat.setColor(QColor("orange")); // À l'arrêt
            } else if (tramwayCourant -> getVitesseActuelle() == 0) {
                contourEtat.setColor(Qt::red);          // Arrêt d'urgence
            } else {
                contourEtat.setColor(Qt::green);        // Circulation normale
            }
            carreGraphique -> setPen(contourEtat);

            // --- Interpolation de la position (calcul X/Y) ---
            int coordX, coordY;

            if (tramwayCourant -> getArretActuel()) {
                coordX = tramwayCourant -> getArretActuel() -> getX();
                coordY = tramwayCourant -> getArretActuel() -> getY();
            } else {
                arret* arretPrecedent = tramwayCourant -> getArretPrecedent();
                arret* arretSuivant = tramwayCourant -> getArretSuivant();

                if (arretPrecedent && arretSuivant && tramwayCourant -> getDistanceTotaleSegment() > 0) {
                    double pourcentageParcouru = (double)(tramwayCourant -> getDistanceTotaleSegment() - tramwayCourant -> getDistanceRestante()) / tramwayCourant -> getDistanceTotaleSegment();

                    coordX = arretPrecedent -> getX() + (arretSuivant -> getX() - arretPrecedent -> getX()) * pourcentageParcouru;
                    coordY = arretPrecedent -> getY() + (arretSuivant -> getY() - arretPrecedent -> getY()) * pourcentageParcouru;
                } else {
                    continue;
                }
            }

            carreGraphique -> setRect(coordX - 6, coordY - 6, 12, 12);
        }
    }
    d_scene -> update();
}

void fenetreTramway::onSimulation() {
    if (d_timer -> isActive()) {
        // --- MODE PAUSE ---
        d_timer -> stop();
        d_boutonSimulation -> setText("▶ Reprendre Simulation");
        d_boutonSimulation -> setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px 15px; border-radius: 6px; } QPushButton:hover { background-color: #45a049; }");
        d_boutonReset -> setEnabled(true);

    } else {
        // --- MODE DÉMARRAGE ---
        d_timer -> start(33);
        d_boutonSimulation -> setText("⏸ Mettre en Pause");
        d_boutonSimulation -> setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; padding: 8px 15px; border-radius: 6px; } QPushButton:hover { background-color: #da190b; }");
        d_boutonReset -> setEnabled(false);

        // Verrouille les cases à cocher des lignes pendant la simulation
        for (QCheckBox* checkBoxLigne : d_lignesGraphiquesMap.keys()) {
            checkBoxLigne -> setEnabled(false);
        }
    }
}

void fenetreTramway::onNomsArrets() {
    d_nomsVisibles = !d_nomsVisibles;
    d_boutonVisibiliteNoms -> setText(d_nomsVisibles ? "Masquer les noms" : "Afficher les noms");
    majVisibiliteArrets();
}

void fenetreTramway::afficherCacherLigne(bool checked) {
    QCheckBox* checkBoxDeclencheur = qobject_cast<QCheckBox*>(sender());

    if (checkBoxDeclencheur && d_lignesGraphiquesMap.contains(checkBoxDeclencheur)) {
        // Cache/Affiche les segments (rails)
        for(auto segmentGraphique : d_lignesGraphiquesMap.value(checkBoxDeclencheur)) {
            segmentGraphique -> setVisible(checked);
        }

        ligne* ligneConcernee = d_monReseau -> chercherLigne(checkBoxDeclencheur -> text().toStdString());
        if (ligneConcernee != nullptr) {
            ligneConcernee -> setVisible(checked);

            // Cache/Affiche les tramways de cette ligne instantanément
            for (tramway* tramwayCourant : ligneConcernee -> getTramways()) {
                if (d_mapTramsVisuels.contains(tramwayCourant)) {
                    d_mapTramsVisuels.value(tramwayCourant) -> setVisible(checked);
                }
            }
        }
        majVisibiliteArrets();
    }
}

void fenetreTramway::majVisibiliteArrets() {
    for (auto iterateurArret = d_mapArretsVisuels.begin(); iterateurArret != d_mapArretsVisuels.end(); ++iterateurArret) {
        bool arretDoitEtreVisible = false;

        // Vérifie si cet arrêt appartient à au moins une ligne actuellement visible
        for (auto iterateurLigne = d_arretsParLigneMap.begin(); iterateurLigne != d_arretsParLigneMap.end(); ++iterateurLigne) {
            if (iterateurLigne.key() -> isChecked() && iterateurLigne.value().contains(iterateurArret.key())) {
                arretDoitEtreVisible = true;
                break;
            }
        }
        iterateurArret.value().cercle -> setVisible(arretDoitEtreVisible);
        iterateurArret.value().texte -> setVisible(arretDoitEtreVisible && d_nomsVisibles);
    }
}

void fenetreTramway::renitialiserSimulation() {
    for (tramway* tramwayCourant : d_monReseau -> getTramways()) {
        tramwayCourant -> reinitialiser();
    }

    // Déverrouille les cases à cocher en mode pause
    for (QCheckBox* checkBoxLigne : d_lignesGraphiquesMap.keys()) {
        checkBoxLigne -> setEnabled(true);
    }

    d_boutonReset -> setEnabled(false);
    d_boutonSimulation -> setText("▶ Démarrer Simulation");

    tickSimulation(); // Force un rafraîchissement visuel des tramways à l'arrêt
}