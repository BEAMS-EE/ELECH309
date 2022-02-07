# ELECH309

Le but du projet intégré d'électricité est de donner l'occasion aux étudiants d'utiliser des concepts vus dans différents cours de la filière électronique-télécommunications, pour mener à bien un projet multidisciplinaire de grande ampleur, ce qui est impossible dans les travaux pratiques classiquement associés à un cours.

## Objectif du projet

L'objectif technique du projet est de réaliser le système de contrôle d'un robot.  Ce robot doit pouvoir se déplacer en ligne droite et effectuer des rotations sur place.  Les déplacements qu'il doit effectuer lui seront communiqués au moyen d'un canal de communication audio.

Le robot devra réagir à deux ordres :

* "Déplace-toi en ligne droite de X cm".
* "Effectue une rotation sur place de Y degrés".

## Organisation du projet

Le projet est divisé en 15 séances de 4h, étalées sur tout le second quadrimestre.  Il a été conçu pour être une équipe de 2 ou 3 étudiants.

Ce projet a également pour but de vous faire réfléchir sur la méthodologie à adopter pour aborder un projet d'ingénierie, vous devrez organiser vous-même votre travail.

Un dépôt Gitub est à votre disposition avec des documents utiles au projet et des exemples de code : <https://github.com/BEAMS-EE/ELECH309>

En particulier, le répertoire "Description du projet" contient les documents qui vous aideront à démarrer :

* L'analyse préliminaire du projet dans laquelle sont décrits et justifiés les choix techniques qui vous sont imposés
* L'étude du déplacement du robot qui décrit les périphériques utilisés pour interfacer le microcontrôleur aux moteurs et encodeurs du robot, ainsi que le dimensionnement de la régulation que nous vous proposons d'utiliser.
* L'étude de la communication audio qui décrit le protocole utilisé, ainsi que le démodulateur que nous vous proposons d'implémenter.

## Evaluation du projet

L’évaluation du projet intégré sera basée sur :

* Une évaluation continue du travail fourni au cours de l'année
* Un rapport écrit, décrivant :
  * La méthodologie utilisée :
    * Analyse du cahier des charges
    * Découpe du projet en modules fonctionnels
    * Développement des modules
    * Stratégie de validation des modules individuellement et du robot dans son ensemble
  * Les solutions techniques adoptées
    * Justification des choix techniques
    * Calculs, simulations…
  * Les résultats des tests et validations
  * Les codes écrits dans le cadre du projet. Ils devront être commentés.
* Une évaluation orale portant sur le contenu du projet :
  * Une défense de 10 minutes, présentant les principaux résultats de votre projet
  * Une démonstration du robot
  * Une séance de questions/réponses
  * Cette évaluation se fera durant la session de Juin
