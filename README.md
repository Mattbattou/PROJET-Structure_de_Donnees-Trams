# Simulateur de Réseau de Tramways

## Description Générale
Ce projet est une application logicielle développée en C++ ayant pour but de modéliser, simuler et visualiser le fonctionnement d'un réseau de transport en commun de manière réaliste. L'objectif principal est de concevoir un système capable de gérer le trafic de multiples rames sur un réseau complexe, tout en restituant son état en temps réel via une interface graphique interactive.

Le développement s'articule autour de trois composants majeurs : une architecture de données robuste pour la modélisation du réseau, un moteur calculant la physique et la logique de la simulation, et un module de rendu visuel.

## Fonctionnalités et Logique de Simulation

### 1. Moteur de Simulation et Comportement
Le cœur du programme repose sur une simulation temporelle discrète (mise à jour par intervalles stricts d'une seconde). Le moteur gère de manière autonome :
* **La cinématique des rames :** La vitesse des tramways est binaire (soit nulle lors d'un arrêt, soit à sa vitesse maximale de croisière).
* **Les règles de sécurité :** Implémentation d'un système d'anti-collision empêchant deux rames de se percuter ou de se dépasser, en imposant une distance de sécurité minimale sur les voies.
* **La logique de trajet :** Gestion des temps d'arrêt obligatoires en station et inversion automatique du sens de circulation aux terminus des lignes.

### 2. Modélisation et Configuration
* **Topologie dynamique :** Le réseau n'est pas codé en dur. Les lignes, les arrêts et les distances qui les séparent sont générés à partir du chargement de fichiers de configuration éditables.
* **Gestion des croisements :** Le système est capable de gérer l'affichage de lignes distinctes partageant un même tronçon ou une même station.

### 3. Interface Graphique (GUI)
* L'interface est entièrement développée avec le framework **Qt**.
* Elle offre une représentation en deux dimensions (2D) du réseau, dessinant les voies sous forme de segments géométriques et animant les rames en temps réel au fil de la simulation.

## Architecture Technique et Contraintes

Ce projet a été réalisé avec un fort niveau d'exigence concernant l'optimisation et la structure du code :

* **Paradigme :** Programmation Orientée Objet (POO) stricte en C++. L'ensemble de la logique (à l'exception de la boucle principale d'exécution) est encapsulé dans des classes.
* **Structures de Données :** Utilisation obligatoire de listes chaînées pour la modélisation séquentielle des arrêts d'une ligne, couplées à des tableaux dynamiques, piles et files pour la gestion globale du trafic.
* **Gestion de la Mémoire :** Les allocations dynamiques de pointeurs (`new`) sont rigoureusement contrôlées et libérées (`delete`) afin de prévenir toute fuite de mémoire (memory leak) lors des cycles de simulation prolongés.
* **Dépendances :** Bibliothèque `Qt` pour l'interface utilisateur, bibliothèques standards `<windows.h>` et `<time.h>` pour la gestion des boucles temporelles.

## Prérequis et Lancement
* **Environnement de développement :** Visual Studio, Code::Blocks ou Qt Creator.
* **Compilation :** Le projet nécessite un compilateur C++ standard (GCC/MinGW ou MSVC) et la liaison avec les bibliothèques Qt (Core, Gui, Widgets).

---
*Projet académique réalisé par un groupe de 4 étudiants dans le cadre de la validation du module universitaire "Structures de Données".*
