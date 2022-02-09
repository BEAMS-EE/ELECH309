# pio2.X

Dans cet exemple, on veut que la LED s'allume lorsque'un bouton est enfoncé et s'éteigne lorsqu'il est relâché.

## Ajout d'un bouton

Nous allons donc ajouter un bouton au circuit de *pio1.X* pour pouvoir interagir avec le dsPIC :

[Schéma de pio2.X](img/pio2Schematic.png)

Lorsque le bouton est enfoncé, il connecte la patte au 3,3V.  
Dans le cas contraire, l'état de la patte est défini par R2.  
En effet, si la patte est une entrée, elle ne fournit aucun courant et la tension aux bornes de R2 est donc nulle.

## Code

Par rapport à *pio1.X*, nous avons fait quelques changements :

* La définition des *configuration bits* a été déplacée dans un nouveau fichier : *configuration.c*.  
On peut voir que ce fichier a été ajouté dans le dossier *Source Files* du projet, pour qu'il soit pris en compte lors de la compilation.
* On n'utilise plus *__delay_ms*, on a donc supprimé l'inclusion de *libpic30.h*

Le code du *main* est semblable à celui de *pio1.X* :

```C
#include <xc.h>         // Contains SFR variables definition

int main(void) {
    TRISBbits.TRISB15 = 0;  // Configure RB15 as a digital output pin
    TRISBbits.TRISB14 = 1;  // Configure RB14 as a digital input pin

    // Main (infinite) loop
    while(1) {
        LATBbits.LATB15 = PORTBbits.RB14;
    }
    
    return 0;
}
```

On commence par initialiser les *TRIS* pour définir le sens de PIO utilisées.  

Cela n'est normalement pas nécessaire pour *TRISB14*, car il vaut 0 par défaut au démarrage du dsPIC.  
Il existe au moins 2 raisons de le faire quand même :

* cela explicite le fait qu'on utilise RB14 en entrée. Cela contribue donc à rendre le code plus lisible.
* Il se pourrait qu'une fonction d'initialisation d'un autre périphérique modifie tout le registre *TRISB* pour configurer d'autres pattes et change à votre insu la valeur de *TRISB14*.  
Cette hypothèse suppose que la fonction en question est (très) mal écrite, elle semble donc peu probable, mais c'est le genre de bug difficile a détecter.

La boucle infinie lit simplement la valeur de *RB14* et l'assigne à *RB15*.  
Cela équivaut en fait à connecter directement *RB14* et *RB15*.
