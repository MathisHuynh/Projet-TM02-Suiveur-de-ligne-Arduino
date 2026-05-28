# Robot Suiveur de Ligne PID

> Un système embarqué autonome combinant un asservissement par correction proportionnelle-dérivée et une interface utilisateur anthropomorphe.

---

## Présentation du Projet

Ce projet consiste en la conception et le développement d'un véhicule robotisé autonome doté d'une architecture algorithmique modulaire en **C/C++ pour Arduino**. 

Le robot intègre un réseau de capteurs optiques infrarouges afin de suivre une trajectoire au sol matérialisée par une ligne noire. L'ajustement de la trajectoire est calculé en temps réel via un régulateur PID, tandis qu'un écran LCD monté sur le châssis externalise l'état interne du système à travers des expressions visuelles dynamiques.

---

## Architecture et Fonctionnalités du Système

* **Asservissement en Temps Réel :** Collecte des données de réflectivité sur cinq points de mesure analogiques et calcul instantané de la correction de trajectoire.
* **Gestion des Virages Critiques :** Algorithme de secours configuré pour intercepter les pertes de trajectoire brutales et appliquer une rotation inverse d'urgence.
* **Interface Homme-Machine (IHM) :** Génération de caractères personnalisés en mémoire RAM pour animer un visage matriciel évolutif sur un écran LCD 20x2 connecté en I2C.

---

## Algorithme de Contrôle et Logique de Navigation

### Régulation par Correction Proportionnelle-Dérivée
Le déplacement repose sur une boucle de rétroaction. L'erreur de positionnement est calculée à partir de la moyenne pondérée de la valeur des capteurs actifs. 
* **Composante Proportionnelle (Kp = 2) :** Assure une correction directe et réactive proportionnelle à l'écart mesuré.
* **Composante Dérivée (Kd = 20) :** Anticipe les variations rapides de la courbe de la ligne. Une valeur élevée a été spécifiquement retenue pour amortir l'inertie du châssis et stabiliser le robot lors de l'approche de courbes à angle droit.

### Comportement Hors-Ligne et Mémoire d'État
Lorsque le robot quitte intégralement la ligne de guidage, le système bascule sur une logique de persistance. Il analyse le dernier état mémorisé des capteurs périphériques pour déterminer la direction de la sortie de piste. Le robot applique alors un couple moteur asymétrique inversé afin de pivoter sur son axe central jusqu'à la ré-interception du signal.

### Animation Visuelle Évolutive
L'affichage sur l'écran LCD est indexé sur les cycles d'horloge du processeur central par l'intermédiaire d'un compteur incrémentiel. Cette structure non bloquante permet de modifier l'orientation du regard et l'expression du robot (neutre, gauche, droite, haut, satisfait) à des intervalles réguliers sans jamais interrompre la scrutation des capteurs ou la modulation PWM des moteurs.

---

## Technologies Utilisées

* **Environnement Logiciel :** C/C++ embarqué (Architecture Arduino), gestion modulaire des sources (fichiers d'en-tête .h et implémentations .cpp).
* **Bibliothèques Matérielles :** Wire (protocole I2C), LiquidCrystal_I2C (contrôle de l'afficheur), TimerOne.
* **Composants Matériels Pris en Charge :** Matrice de 5 capteurs de réflectivité analogiques, pont en H pour la gestion de deux moteurs à courant continu via signaux PWM, écran LCD 20x2.

---

## Équipe et Contributions

* **Développement Logiciel & Architecture PID :** Non spécifié
* **Intégration Matérielle & Conception Électronique :** Non spécifié
* **Débogage & Optimisation des Paramètres :** Non spécifié