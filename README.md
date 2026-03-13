# 🚋 Simulation de Gestion d'un Réseau de Tramways
Ce projet a pour objectif de développer un programme simulant un réseau de tramways de la manière la plus réaliste possible, incluant la modélisation du réseau, la simulation des déplacements et une visualisation graphique.


# 🚋 Simulateur de Réseau de Tramways

Projet de développement en C++ simulant un réseau de tramways de façon réaliste, avec affichage graphique via la bibliothèque Qt. Projet réalisé en groupe de 4 étudiants sur 6 semaines.

## 📊 État d'avancement du projet

### 🏗️ Structures de Données & Architecture
* ✅ **Architecture Orientée Objet :** Tout le code (sauf la boucle principale) est encapsulé dans des classes.
* ✅ **Listes chaînées (Obligatoire) :** Implémentées pour relier les arrêts d'une ligne.
* ✅ **Tableaux dynamiques :** Utilisés pour stocker les éléments globaux.
* ✅ **Gestion de la mémoire :** Utilisation optimale des allocations et libérations (`delete`) pour éviter les fuites.
* ❌ **Piles et Files :** Structures suggérées par le sujet, à intégrer si pertinent.
* ❌ **Stockage des Trams par Ligne :** La structure de chaque Ligne DOIT contenir la liste des tramways qui y sont placés. *(Actuellement stocké uniquement dans le réseau global).*

### 📂 Gestion des Fichiers et Console
* ✅ **Chargement multiple :** Les informations sont chargées à partir d'un ou plusieurs fichiers.
* ✅ **Format éditable :** Fichiers éditables facilement à la main.
* ❌ **Menu Console (Mode Texte) :** Le programme doit démarrer par une boucle principale en mode texte.
* ❌ **Minuteur global :** Demander à l'utilisateur la durée de la simulation via la console. La fenêtre Qt s'ouvre ensuite, puis se ferme automatiquement au bout du temps défini.

### 🎨 Interface Graphique (Qt)
* ✅ **Affichage en temps réel :** Visualisation de la simulation avec Qt.
* ✅ **Dessin du réseau :** Les arrêts nécessitent des coordonnées 2D, les voies sont des segments de droite.
* ✅ **Représentation des tramways :** Représentés graphiquement par des petits carrés.
* ✅ **Croisements (Facultatif) :** Gérer l'affichage lorsque deux lignes passent par les mêmes arrêts.

### ⚙️ Moteur de Simulation
* ✅ **Intervalle de temps :** La simulation se fait par intervalle de 1 seconde (arrêt/départ aux arrêts, déplacement).
* ✅ **Vitesse binaire :** La vitesse des tramways est soit nulle, soit égale à sa vitesse maximale.
* ❌ **Initialisation stricte :** Au lancement, tous les tramways doivent obligatoirement être placés aux arrêts.
* ❌ **Données dynamiques du Tramway :** Chaque tramway doit stocker son temps passé à l'arrêt OU la distance le séparant de l'arrêt suivant, ainsi que son sens de déplacement.
* ❌ **Distance de sécurité :** Un tramway doit s'arrêter entre deux arrêts si la distance minimale avec le tramway de devant n'est pas respectée.
* ❌ **Anti-dépassement :** Les tramways ne peuvent en aucun cas se dépasser.
* ❌ **Gestion des Terminus :** La direction de déplacement des tramways doit changer aux terminus.

### 📝 Livrables et Évaluation (À préparer)
* ❌ **Rapport de projet :** Doit expliquer l'interface, l'organisation du programme (avec schéma/organigramme), les choix d'organisation, et définir la contribution de chaque étudiant.
* ❌ **Qualité du code :** Évaluation sur l'implémentation, l'optimisation des boucles et parcours de tableaux.
* ❌ **Soutenance orale :** Présentation PowerPoint de 10 minutes + 20 minutes de questions.
* ❌ **Démonstration :** Une démonstration du programme en direct sera exigée lors de la soutenance.

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
