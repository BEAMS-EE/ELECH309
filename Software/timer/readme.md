# Les timers

## Introduction

Dans le 1er exemple sur les PIO, nous avons fait clignoter une LED en utilisant *__delay_ms()* pour fixer la période.

Cette méthode est simple à implémenter.  Elle a cependant 2 défauts :

* La période de clignotement n'est pas exactement égale au délai, car il faut tenir compte du temps d'exécution total de la boucle principale.  Dans notre cas, c'est négligeable, mais pour des applications demandant plus de précision et/ou une période plus courte, cela peut être gênant.
* Pendant l'exécution de *__delay_ms()*, le µC ne fait rien d'autre.  Cela empêche le µC de se consacrer à d'autres tâches.

En théorie, on peut ajouter du code dans la boucle principale pour que le µC fasse autre chose et diminuer le délai pour tenir compte du temps d'exécution de la boucle principale

En pratique, cela peut vite devenir compliqué : s'il y a des conditions dans le code (if, while, for…), le temps d'exécution de la boucle n'est pas constant, sauf si on ajoute des délais aux bons endroits.  
De plus, il faut adapter tous ces délais à chaque modification.

## Cahier des charges des exemples

Pour illustrer ces problèmes et les solutions possibles, nous allons essayer de satisfaire le cahier des charges suivant :

* Le µC doit générer un signal sur **RB15** à la fréquence de 5kHz, avec un rapport cyclique  de 50%.
* D'autre part, le µC doit également exécuter une autre tâche dont l'exécution prend quelques ms.    Cela peut par exemple être le calcul d'un filtre numérique, le contrôle d'un moteur...

Dans nos codes, cette tâche sera simplement simulée par une fonction contenant des délais : *aLongTask()*.  
Elle s'exécute généralement en 100 µs, mais toutes les 20 exécutions, elle dure 1 ms.

## *timer1.X*

Dans cette implémentation, on utilise *__delay_ms()* pour fixer la période (comme dans *pio1.X*).  
Le code génère aussi un signal sur **RB13** dont l'état haut correspond à l'exécution de *aLongTask()*.

On remarque que la période de **RB15** est perturbée par cette fonction.

## Utilisation d'un *timer*

Heureusement, les µC possèdent un périphérique pour gérer plus efficacement le temps : le *timer*.

Notre dsPIC possède 5 *timers*, nous utiliserons le *timer1*.

Un *timer* compte les périodes d'horloge du µC, jusqu'à atteindre un seuil défini (par le code).  
Il recommence alors à compter en partant de 0 (on dit que le timer déborde).  Au même moment, il met à 1 un bit appelé flag d'interruption.

Tous les périphériques possèdent un ou plusieurs flags d'interruption. Ceux-ci sont "levés" (mis à 1) lorsqu'un évènement particulier se produit (dans le cas des timers, lors d'un débordement).

Ils permettent de synchroniser l'exécution d'un code avec des évènements hardware.  On s'en sert de deux manières : le *polling* et l'*interruption*.

## Utilisation d'un timer en polling (*timer2.X*)

La méthode la plus simple pour utiliser un timer est le polling.

Elle consiste à vérifier l'état du flag d'interruption qui nous intéressent dans la boucle principale.  
Cette vérification est très rapide et ne bloque donc pas l'exécution du code.  Elle est faite régulièrement dans la boucle principale.

Le principe de fonctionnement est le suivant :

* Le timer déborde et lève son flag
* La boucle principale, qui vérifie l'état du flag, détecte sa levée
* Elle effectue l'action prévue (dans notre cas, changer l'état de la LED)
* Elle baisse le flag (le remet à 0) pour indiquer qu'elle a réagi à l'évènement.

Les 2 dernières étapes peuvent être inversées.  

Pour illustrer ce principe, on utilise souvent une analogie avec les boîtes aux lettres américaines (qui aurait inspiré l'idée du polling aux informaticiens).  
Cette analogie est expliquée en annexe.

Les flags d'interruption sont regroupés dans les registres **IFSx** (Interrupt Flag Status).  Le flag du *timer1* se trouve dans le registre **IFS0**.

Si on traduit ce principe en code, nous obtenons la structure suivante :

```C
int main() {
    // Ajouter ici la configuration du timer1

    while(1) {
        if (IFS0bits.T1IF == 1) {
            IFS0bits.T1IF = 0;
            // Ajouter le code à exécuter lorsque le timer1 déborde
        }
        // Ajouter ici les autres actions (aLongTask)
    }
    return 0
}
```

Cette solution est implémentée dans *timer2.X*.

Néanmoins, on peut observer que certaines périodes sont toujours perturbées par l'exécution de *aLongTask()*.

Cela vient de l'existence d'un délai entre le débordement du *timer1* et le l'instant où la boucle principale "voit" que le flag d'interruption est levé.

Ce délai peut varier de 0 au temps d'exécution de la boucle principale.

On appelle ce délai la *latence*.

Par contre, par rapport à *timer1.X*, la période est correcte en moyenne.  
En effet, quand une période est trop longue à cause de *aLongTask()*, la suivante sera plus courte pour compenser.

Cela crée une imprécision sur l'instant où se produisent les flancs du signal (on appelle cela du *jitter*).

## Utilisation d'un *timer* en interruption

Pour éviter le *jitter*, il suffit que la latence soit constante.  C'est ce que permet le mécanisme des *interruptions*.

Pour utiliser une *interruption*, le code configure le µC pour exécuter automatiquement une fonction lorsque le flag d'interruption lié à un événement spécifique est activé.

Le µC interrompt alors l'exécution du programme principal et exécute cette fonction dédiée à cette source d’interruption.  
Cette fonction est nommée *routine d’interruption* ou *ISR* (*Interrupt Service Routine*).

L'ISR est une fonction avec une syntaxe spécifique.

Elle ne doit jamais être appelée par le code.

Le flag d’interruption doit être remis à zéro dans l'ISR (comme dans le cas du polling).

Pour appliquer cette technique à notre cas, la procédure est :

* La configuration du timer1 est la même que pour le polling
* A la fin de cette configuration, il faut activer l’interruption du timer1.  Cela se fait en mettant à ’1’ le bit Interrupt Enable qui lui correspond.  Dans le cas du timer1, c'est IEC0bits.T1IE.
* Il faut supprimer tout ce qui concerne le timer1 et RB15 dans la boucle principale.  Ce n'est plus elle qui s'en occupe, mais l'ISR.
* L’ISR correspondant au traitement de l’événement doit être écrite.  Chaque ISR a un nom prédéfini indiquant au compilateur à quelle flag d'interruption elle doit être liée (voir code ci-dessous).  Cette ISR doit contenir le code qui gère RB15.

Cela nous donne la structure du code suivante :

```C
int main() {
    // Ajouter ici la configuration du timer1 et l'activation de T1IE

    while(1) {
        // il n'y a rien à faire concernant le timer 1 et RB15 dans la 
        // boucle principale.
        // Ajouter ici les autres actions (aLongTask)
    }
    return 0
}


void __attribute__((interrupt, no_auto_psv))_T1Interrupt(void) {
    IFS0bits.T1IF = 0;
    // Ajouter le code à exécuter lorsque le timer1 déborde
}
```

Le principe de fonctionnement de ce code est :

* La boucle principale est exécutée normalement
* En parallèle, le timer1 compte
* Lorsque le timer1 déborde, il lève son flag d'interruption.
* Comme l'interruption est activée (T1IE=1), le µC interrompt l'exécution de la boucle principale et commence à exécuter l'ISR du timer1
* L'ISR doit baisser le flag d'interruption pour indiquer que l'évènement a été traité.
* Lorsque l'ISR se termine, le µC reprend l'exécution de la boucle principale.

La latence de l'interruption (le délai entre le débordement et le début de l'exécution de l'ISR) est constante car elle est due au circuit logique qui gère les interruptions dont le comportement est complètement déterministe.

## Analogie postale

![Boite aux lettres "américaine"](../img/boiteAuxLettres.jpg)

Les boites aux lettres américaines ont souvent un petit "drapeau" qui peut être levé ou baissé.  
Il est utile pour les gens dont la maison est éloignée de la route et qui ont donc une "grande" distance à parcourir pour aller récupérer leur courrier.

Lorsque le facteur dépose du courrier dans la boite, il lève le drapeau.  
Il n'est alors plus besoin d'aller jusqu'à la boite pour savoir s'il y a du courrier, un coup d'œil par la fenêtre suffit.

Après avoir pris son courrier, on baisse simplement le drapeau.

Dans notre cas, le facteur, c'est le *timer1* (ou tout autre périphérique), le courrier, c'est l'information qu'il y a eu un débordement et le propriétaire de la boite, c'est le µC.

Remarques :

* Le facteur ne baisse jamais le drapeau.  Si le drapeau est levé quand il passe, le facteur n'y touche pas.  De même, un périphérique ne remet jamais un flag d'interruption à 0.
* Cela implique que si la fréquence du polling n'est pas suffisante (la boucle principale est trop longue), on peut rater des évènements.  
  Dans notre cas, si *aLongTask* durait 100ms par exemple, le polling ne fonctionnerait pas du tout.
