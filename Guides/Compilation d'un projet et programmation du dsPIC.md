# Compilation du projet et programmation du dsPIC

Quelque soit le langage utilisé, un code source doit être traduit en "code machine", compréhensible par le processeur qui l'exécutera.  Pour cela, il existe 2 possibilités :

* Dans les langages interprétés (comme *Python*), le code source est traduit en code-machine pendant son exécution (l'expression anglaise est *at run-time*).  
L'avantage est que ces codes peuvent être exécutés sur n'importe quelle machine ayant un environnement Python installé.  
Le défaut étant que l'exécution est ralentie par la traduction "en live".
* Dans les langages compilés (comme *C*), le code source est traduit une seule fois. Cette étape s'appelle la compilation.  
Elle fournit des fichiers directement exécutables (les *.exe* de *Windows* par exemple).  
Le principal défaut est que le fichier exécutable n'est compréhensible que par le processeur pour lequel il a été compilé.

La compilation se fait en deux étapes :

* Chaque fichier source (*.c*) du projet est compilé séparément
* Ensuite, le fichier exécutable final est construit en utilisant les fichiers compilés. C'est l'étape de l'édition de liens.

Dans MPLABX, toutes ces étapes sont automatisées. Il existe deux commandes de compilation :

## Build Project
Cette commande ne compile que les fichiers sources qui ont été modifiés depuis la dernière compilation.  
Pour cela, le compilateur compare les dates des fichiers sources avec celles des fichiers compilés correspondants.  
Cette commande permet d’accélérer la compilation des projets où il y a plusieurs fichiers sources dont certains ne sont pas modifiés souvent (comme les fichiers des périphériques).  
C'est la plus utilisée.

## Clean and Build Project  
Elle efface d'abord tous les fichiers compilés, obligeant le compilateur à compiler tous les fichiers sources.  
Cela prend évidemment plus de temps, mais permet de s'assurer que tous les fichiers sont correctement compilés.
En effet, comme *Build project* se base sur les dates des fichiers pour décider lesquels compiler, il se peut qu'il se trompe (par exemple si vous remplacer un fichier source par une version plus ancienne après vous être rendu compte que vos dernières modifications n'étaient en fait pas une si bonne idée).  

Au cours de la compilation, la fenêtre de sortie affiche les messages du compilateur.  Si le programme ne contient pas d'erreurs de syntaxe, elle affichera *BUILD SUCCESSFUL*.

Dans le cas contraire, le document [*Correction des erreurs*](Correction%20des%20erreurs.md) peut vous aider.

## Exécution du programme

Nous pouvons enfin transférer notre programme dans le µC.  
Pour cela, nous avons besoin du *PICkit 4*.  Il se connecte sur un port USB du PC et sur le connecteur de programmation du module (connecteur à 5 pattes situé à l'opposé de l'USB).  
Le *PICkit 4* a un connecteur à 8 pattes pour se connecter au dsPIC.  
Nous n'en utiliserons que 5, celles du côté de la flèche.  La patte 1, désignée par cette flèche, se connecte à la patte correspondante sur le module.
Pour programmer votre dsPIC :

* Connectez le *PICkit 4* à votre PC et au module
* Alimentez le module par son connecteur micro-USB. Vous pouvez utiliser un port USB de votre PC ou un chargeur USB.
* Cliquez sur *Run Main Project*

Lorsque le dsPIC est programmé, votre programme commence à s'exécuter immédiatement.

Une fois le module programmé, vous pouvez déconnecter le *PICkit*.

Le dsPIC stocke son programme dans une mémoire non volatile (du même type que celle des clés USB).  Vous pouvez donc retirer l'alimentation du module et la rebrancher plus tard sans devoir le reprogrammer.
