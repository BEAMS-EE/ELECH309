# Analyse du projet

Nous voulons développer un robot capable de se déplacer de manière autonome, sur base d'ordres qu'il reçoit par un canal de communication audio.  
Les signaux de commande audio seront produits par un ordinateur ou un smartphone.

Ce robot doit servir dans un projet étudiant. Les choix techniques seront donc également guidés par des considérations pédagogiques.

## Cahier des charges

### Fonctions

* Se déplacer précisément et de façon autonome
* Recevoir et comprendre des signaux de commande audio
* Obéir aux ordres "Avance", "Recule", "tourne à droite", Tourne à gauche" (accompagnés d'un paramètre indiquant la distance ou l'angle à parcourir)

### Contraintes

#### Contraintes environnementales

* Le robot doit être capable de se déplacer sur un terrain solide, plat et horizontal (typiquement une table)
* Le robot doit être capable de fonctionner dans un environnement calme : à l'intérieur, sans bruit de fond (conversation, musique, …)

#### Contraintes utilisateurs

* La tension d'alimentation doit être inférieure ou égale à 24V, pour éviter tout risque d'électrocution
* La vitesse de translation du robot doit être telle que celui-ci puisse être facilement "maitrisé" par les étudiants si nécessaire durant les tests de leurs programmes, indépendamment de la manière dont les moteurs sont commandés
* Le robot doit être facilement tranportable pour laisser aux étudiants la possibilité de l'emporter chez eux

### Découpe en blocs

Notre robot peut être divisé en trois grandes parties :

* Une base roulante qui est la partie mécanique du robot
* Une électronique de commande, basée sur un microcontrôleur
* Un code informatique qui définit le comportement du robot.

## Base roulante

Le robot ne devant pas avoir de performances mécaniques particulières, nous avons choisi la base roulante la plus simple possible : un tricycle équipé de deux roues motrices indépendantes, actionnées chacune par un moteur à courant continu.

Deux capteurs de rotation sont montés sur l'axe des roues pour mesurer leur déplacement angulaire.  Ce sont des capteurs de type encodeur en quadrature.

Le troisième point d'appui est une bille, pour minimiser son effet sur les mouvements du robot.

Cette base roulante est conçue sur base d'un système de pièces pour applications robotiques.  Il possède toutes les pièces mécaniques nécessaires (à part la bille servant de troisième point d'appui).  Le montage et la maintenance ne nécessitent pas d'outillage ou de compétence spécifiques.

Nous utiliserons des moteurs à courant continu équipés d'un réducteur de vitesse et d'encodeurs en quadrature.  
Ces derniers sont alimentés en 5V et fournissent deux signaux logiques compatibles CMOS.  
Nous avons choisi ces moteurs car ils sont conçus pour des applications robotiques et sont compatibles avec les tensions, couples et vitesses que nous voulons.

Nous avons besoin d'un driver de moteur pour faire l'interface entre le dsPIC et les moteurs.  Nous avons sélectionné le DRI0044 qui est un module basé sur un circuit intégré populaire dans le domaine de la robotique ludique.  Il est bon marché, compatible avec un protoboard et convient à nos moteurs.

### Dimensionnement

Le dimensionnement de la structure mécanique n'est soumis qu'à la contrainte limitant la vitesse maximum du robot.  
Les moteurs ont une vitesse maximale de 350 t/min.  
On considère que la vitesse maximale du robot doit être comprise entre 1 et 2 m/s.  Cela correspond donc à un diamètre de roue compris entre 55 et 109 mm.

Nous avons choisi des roues de 82 mm  de diamètre, ce qui donne une vitesse maximale de 1,5 m/s.

La distance entre les roues, l'empattement, est fixé par la géométrie de la base roulante et n'a pas fait l'objet d'un dimensionnement spécifique. Il est de 134 mm.  
Cela donne une vitesse de rotation maximale de 3,57 t/s.  
C'est trop rapide, le robot ne devra jamais atteindre une telle vitesse en foncionnement normal.  
Il est difficile de diminuer cette vitesse maximale, il faudrait :

* diminuer le diamètre des roues, ce qui rendrait le robot lent en ligne droite
* augmenter l'empattement, ce qui augmenterait la taille du robot et le rendrait difficilement transportable

Nous avons donc fait le choix de privilégier la taille du robot en gardant l'empattement initial.

## Electronique de commande

Pour faciliter le démarrage du projet, nous voulons fournir aux étudiants une base roulante fonctionnelle qu'ils devraient "juste" programmer pour remplir la première fonction principale : le déplacement autonome.

Nous avons donc ajouté une carte électronique à la base roulante, que nous appellerons carte principale.
Elle est conçue autour d'un microcontrôleur connecté aux moteurs et aux encodeurs en quadrature.  Pour garder une cohérence avec les laboratoires du cours de "Systèmes à microcontrôleur", nous l'avons choisi dans la famille des dsPIC33FJ de Microchip.
Nous avons besoin de deux Output Compare et de deux Quadrature Encoder Interface pour le contrôle des moteurs.  Cela limite notre choix à une dizaine de candidats. Leurs différences étant sans conséquences pour le projet, nous avons choisi le dsPIC33FJ128MC802 car nous en avions en stock lors du développement du prototype.  De plus, c'est celui qui possède le plus de mémoire, ce qui nous laisse un maximum de liberté pour faire évoluer le projet.
En outre, ce dsPIC possède le Peripheral Pin Select (PPS), qui permet de choisir à quelles pattes du dsPIC sont connectées les I/Os de la plupart des périphériques.  Cela facilite le routage des cartes électroniques et évite les blocages qui peuvent survenir lorsqu'on a besoin de deux périphériques partageant les mêmes pattes.
Le dsPIC doit être alimenté en 3,3V et consomme environ 80mA.  Nous avons donc choisi un régulateur linéaire capable de fournir 250mA.
Les encodeurs et le driver de moteur sont compatibles avec les niveaux logiques CMOS 3,3V et peuvent donc être connectés directement au dsPIC.
Le robot est alimenté par une batterie NiMh de 7,2V au travers d'un circuit de protection.

Fonction communication
La partie communication de l'électronique doit transformer un signal sonore en un signal électrique qui sera numérisé et interprété par le démodulateur.

L'électronique nécessaire à la fonction communication peut donc être divisée en deux parties :

* Une chaine d'acquisition analogique, chargée de mettre en forme le signal capté par un microphone
* Un microcontrôleur chargé de numériser et démoduler le signal mis en forme.

Le dimensionnement de la chaine d'acquisition est à la charge des étudiants.
Pour le microcontrôleur, nous avons deux possibilités :

* Utiliser le dsPIC de la carte principale. Comme il possède un convertisseur analogique/numérique, c'est possible.  L'inconvénient de cette solution est de compliquer la partie informatique du projet.
* Utiliser un dsPIC dédié à la démodulation du signal audio.  Cela simplifie le développement informatique, mais nécessite une communication entre les deux dsPIC (une telle communication n'est toutefois pas difficile à mettre en œuvre).

La solution à un seul dsPIC est évidemment meilleure du point de vue du coût et de l'encombrement de l'électronique.  Toutefois, elle rend le développement sensiblement plus compliqué ; en effet, les deux fonctions étant indépendantes, il est probable que leurs codes informatiques soient développés séparément, par des personnes différentes.  Pour pouvoir travailler en parallèle, deux plateformes de développement seront alors nécessaires.

De plus, la démodulation étant a priori une opération exigeante en temps de calcul, il n'est pas garanti que les deux codes puissent tourner en temps réel sur un seul processeur.

Nous commencerons donc par développer une solution à deux dsPIC. En fonction de ses performances et sur base de l'expérience que nous aurons acquise, nous évaluerons les possibilités d'améliorations à étudier.

Pour faciliter le développement de la partie communication, une plaquette de protoboard amovible a été ajoutée à la base roulante.

La structure générale de l'électronique est donnée par le schéma en annexe de ce document.

## Code informatique

Les choix concernant les codes informatiques du projet sont laissés aux étudiants. Pour les aider, deux documents leurs seront fournis : un décrivant le régulateur de position et son dimensionnement et l'autre le principe du démodulateur qui sera utilisé.
