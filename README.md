# 🚋 Simulation de Gestion d'un Réseau de Tramways
Ce projet a pour objectif de développer un programme simulant un réseau de tramways de la manière la plus réaliste possible, incluant la modélisation du réseau, la simulation des déplacements et une visualisation graphique.


## 📊 État d'avancement du projet

### 🏗️ Structures de Données & Architecture
* ✅ **Création des classes :** Architecture orientée objet en place (`reseau`, `ligne`, `arret`, `tramway`).
* ✅ **Listes chaînées :** Implémentation réussie (et obligatoire !) pour relier les arrêts d'une ligne via une structure `NoeudArret`.
* ✅ **Tableaux dynamiques :** Utilisation de `std::vector` pour stocker les lignes et les tramways.
* ✅ **Méthodes fondamentales :** Constructeurs, destructeurs, insertion, suppression et recherche implémentés.
* ✅ **Gestion de la mémoire :** Destructeurs implémentés avec libération propre des pointeurs (`delete`) pour éviter les fuites.
* ❌ **Piles et Files :** Structures mentionnées dans les consignes, à intégrer potentiellement pour des algorithmes spécifiques.

### 📂 Gestion des Fichiers
* ✅ **Chargement des données :** Lecture fonctionnelle depuis plusieurs fichiers textes.
* ✅ **Format modifiable :** Fichiers éditables à la main avec séparateur (`;`).
* ✅ **Informations stockées :** Récupération réussie des données des lignes , des arrêts et des tramways.

### 🎨 Interface Graphique (Qt)
* ✅ **Mise en place de Qt :** Utilisation de `QGraphicsView` et `QGraphicsScene` pour la visualisation en temps réel.
* ✅ **Affichage du réseau :** Dessin des arrêts (avec leurs coordonnées 2D) et des voies (segments de droite).
* ✅ **Affichage des tramways :** Représentation visuelle des trams sous forme de petits carrés.
* ✅ **Croisements (Facultatif) :** Masquage dynamique des lignes via `QCheckBox` avec gestion intelligente des arrêts partagés.
* 🚧 **Détails graphiques :** Coloration automatique des carrés de tramway en fonction de la couleur de leur ligne (logique définie, intégration en cours).

### ⚙️ Moteur de Simulation
* ✅ **Boucle en temps réel :** Utilisation d'un `QTimer` pour l'exécution et l'affichage fluide.
* ✅ **Logique de déplacement :** Calcul de la position 2D interpolée des tramways sur le réseau.
* ✅ **Gestion des arrêts :** Respect de la durée minimale d'attente aux stations.
* 🚧 **Fluidité temporelle :** Ajustement des variables (`int` vers `double`) pour supporter un affichage à 30 IPS tout en simulant des intervalles d'1 seconde.
* ❌ **Anti-dépassement :** Garantir que les tramways ne peuvent pas se doubler.
* ❌ **Distance de sécurité :** Implémenter le freinage/arrêt d'un tramway entre deux arrêts si la distance minimale avec celui de devant n'est pas respectée.
* ❌ **Terminus :** Inverser le sens de déplacement des tramways lorsqu'ils atteignent la fin de la ligne

## 📋 Description du Projet
L'application permet de charger, simuler et visualiser un réseau de transport en commun complexe. Le projet se divise en trois axes principaux :


Modélisation : Gestion des lignes, des arrêts et des tramways via des structures de données spécifiques.


Simulation : Gestion du trafic en temps réel (déplacements, arrêts, vitesse).


Visualisation : Affichage graphique 2D de l'état du réseau.

## ✨ Fonctionnalités Principales

Configuration flexible : Chargement des données du réseau (lignes, arrêts, distances) à partir de fichiers externes modifiables.

Simulation réaliste :

Mise à jour par intervalles de 1 seconde.

Gestion des vitesses (max ou nulle) et temps d'arrêt obligatoires.

Respect des règles de sécurité : distance minimale entre rames et interdiction de dépassement.

Gestion des terminus (changement de sens).

Interface Graphique (GUI) :

Visualisation en temps réel avec la bibliothèque Qt.

Représentation des voies (segments) et des tramways (carrés mobiles).

## 🛠️ Contraintes Techniques

Langage & Outils : C++ (implémentation de classes), Visual Studio ou Code::Blocks.

Structures de données : Utilisation obligatoire de listes chaînées, ainsi que tableaux dynamiques, piles et files.

Gestion mémoire : Allocation et libération dynamique des pointeurs optimisée.

Bibliothèques :

- Qt pour l'affichage graphique.

- windows.h et time.h pour la gestion du temps.

## 👥 Auteurs
Projet réalisé par une équipe de 4 étudiants dans le cadre du module "Structures de Données".
