# Exemples de codes utilisant les PIO

Le fonctionnement des PIO est décrit dans [../Hardware/Les PIO.md](../Hardware/Les%20PIO.md).

Vous trouverez dans ce dossier 4 exemples de code utilisant les *PIO* du dsPIC.

Les deux premiers exemples sont expliqués dans [pio1.md](PIO/pio1.md) et [pio2.md](PIO/pio2.md).

Vous devriez être capable de comprendre les deux autres exemples sans aide.

## pio1.X

Cet exemple est l'équivalent du *blink* d'*Arduino*.  Il sert en général de "*hello world*" pour les systèmes embarqués.  
Ce code fait clignoter une LED à 1 Hz.

## pio2.X

Dans cet exemple, on ajoute un bouton au projet précédent.  
On veut que la LED s'allume lorsque le bouton est enfoncé et s'éteigne lorsqu'il est relâché.

## pio3.X

On utilise le même schéma que *pio2.X*.

* Par défaut, la LED est éteinte.  Lorsque le bouton est enfoncé, la LED doit s'allumer et rester allumée pendant 5 secondes, quelque soit le moment où le bouton est relâché.
* Si le bouton est à nouveau enfoncé pendant ces 5 secondes, cela n'a pas d'effet.
* Si le bouton est enfoncé à la fin des 5 secondes, la LED est à nouveau allumée 5 secondes.

## pio4.X

On utilise le même schéma que *pio2.X*.

* Au reset, la LED est éteinte.
* Chaque fois que le bouton est enfoncé, la LED doit changer d'état.
